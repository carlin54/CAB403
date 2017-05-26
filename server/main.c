//
// Created by Richard on 27/09/2016.
//


#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define REQUIRED_NUMBER_OF_ARGC 2
#define PORT_NUMBER_ARGV_INDEX 1
#define SERVER_STATUS_SERVER_NOT_FULL (0)
#define SERVER_STATUS_SERVER_FULL (1)

#define SAVE_SUCCESS                     0
#define LOAD_SUCCESS                     0
#define LOAD_ERROR_COULD_NOT_LOAD_FILE  -1


#define CLIENT_DISCONNECTED -1
#define CLIENT_RETURNED 0

#define NUM_MIN_COLUMNS_CLIENT 3
#define NUM_COLUMNS_AUTHENTICATION 3
#define NUM_COLUMNS_TRANSACTION 4
#define NUM_COLUMNS_ACCOUNT 3

#define FIND_NEXT_CHAR_EOF 0
#define FIND_NEXT_CHAR_SUCCESS 1

#define TRANSACTION_DEPOSIT     2
#define TRANSACTION_WITHDRAWAL  3
#define TRANSACTION_TRANSFER    4

#define ACCOUNT_INDEX_SAVINGS   0
#define ACCOUNT_INDEX_LOAN      1
#define ACCOUNT_INDEX_CREDIT    2

#define LOGIN_SUCCESS            0
#define LOGIN_INCORRECT         -1
#define LOGIN_TOO_MANY_ATTEMTPS -2

#define SELECTION_ACCOUNT_BALANCE 1
#define SELECTION_WITHDRAWAL 2

#define MAX_THREADS 10

typedef float Money;
typedef int AccountNumber;
typedef int ClientNumber;
typedef int TransactionType;

struct Account {
    AccountNumber AccountNo;
    Money OpeningBal;
    Money ClosingBal;
};

struct Client {
    char FirstName[15];
    char LastName[15];
    ClientNumber ClientNo;
    AccountNumber Accounts[3];
};

struct Authentication {
    char Username[12];
    int Pin;
    ClientNumber ClientNo;
};

struct Transaction {
    AccountNumber FromAccount;
    AccountNumber ToAccount;
    TransactionType TranType;
    Money Amount;
    struct Transaction* pNext;
};
struct request {
    int number;
    int connectFd;
    struct request* next;
};

struct Link {
    struct Link* pNext;
    void* pData;
};

struct LinkedList {
    struct Link* pHead;
    struct Link* pTail;
    int length;
};


#define READ_SUCCESS        0
#define READ_UNSUCCESSFUL   1
#define READ_INCORRECT_SIZE 2
static int thr_id[MAX_THREADS];
static pthread_t  p_threads[MAX_THREADS];

static struct request* requests = NULL;
static struct request* last_request = NULL;

static int listenFd = -1;
static int __PORT_NUMBER__ = -1;
static struct sockaddr_in serverAddr;
static struct sockaddr *serverAddrCast;
static bool listening_for_clients = true;
static int num_requests = 0;
static int requests_handling = 0;
static bool close_all_threads = false;

static pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static pthread_cond_t  got_request   = PTHREAD_COND_INITIALIZER;

static const char* file_account_location = "Accounts.txt";
static struct LinkedList accounts_list;
static const char* file_client_location = "Client_Details.txt";
static struct LinkedList clients_list;
static const char* file_authentication_location = "Authentication.txt";
static struct LinkedList authentications_list;
static const char* file_transactions_location = "Transactions.txt";
static struct LinkedList transactions_list;

bool is_list_empty(struct LinkedList* pList);
void list_add_tail(struct LinkedList* pList, void* pData);
void list_remove_head(struct LinkedList* pList);

bool read_status(int read, int expected);

int read_username(int connectFd, char* username);
int read_pin(int connectFd, int* pin);
int read_selection(int connectFd, int* selection);
int read_return(int connectFd, bool* user_return);
int read_account_index(int connectFd, int* account_index);
int read_amount(int connectFd, float* amount);
int load_accounts();
int load_clients();
int save_accounts();
int save_clients();
bool are_equal(const char *s1, const char *s2);
bool isNegative(int val);
bool isZero(int val);
int number_of_char_in_file(const char c, const char* file_location);
void int_to_char(int val, char* str, int size);
int find_next_char(char c, FILE* file);
int find_next_char_n(char c, int n, FILE* file);

#define WRITE_SUCCESS            0
#define WRITE_INCORRECT_SIZE     1
#define WRITE_DISCONNECTED      -1


bool write_status(int written, int expected);

int write_savings_account(int connectFd, struct Account* account);
int write_loan_account(int connectFd, struct Account* account);
int write_credit_account(int connectFd, struct Account* account);
int write_accounts(int connectFd, struct Account* accounts);
int write_accepted_withdraw(int connectFd, bool accepted);
int write_login_status(int connectFd, int login_status);
int write_client_number(int connectFd, int client_number);
int write_firstname(int connectFd, const char* firstname);
int write_lastname(int connectFd, const char* lastname);
int write_server_status(int connectFd, int server_status);

bool Authenticate(struct Authentication* auth);

int clean_up_dynamic_memory();

struct Account* find_accounts(int clientNo);

char* find_firstname(int clientNo);

char* find_lastname(int clientNo);

bool can_withdrawal(struct Account* account, int account_index, float amount);

void withdraw(struct Account* account, float amount);


int save_authentication();
int save_transactions();
struct Authentication* serve_client_login(int connectFd);

int server_client_account_balance(int connectFd, struct Authentication* auth);

int server_client_withdrawal(int connectFd, struct Authentication* auth);

void server_client_main_menu(int connectFd, struct Authentication* auth);

void disconnect_from_client(int connectFd);

void add_request(int request_num, int connectFd, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var);

struct request* get_request(pthread_mutex_t* p_mutex);

void handle_request(struct request* a_request, int thread_id);

void* handle_requests_loop(void* data);

void init_threads();

void shutdown_threads();

int listen_for_clients();
void save_data(void);
int load_authentications();
int load_transactions();

void load_data(void);

bool server_full();

void init_network();

void shutdown_network();

void signal_exit(void){
    shutdown_threads();
    shutdown_network();
    save_data();
    clean_up_dynamic_memory();
}

int main(int argc, char* argv[]) {

    if(argc > REQUIRED_NUMBER_OF_ARGC || argc < PORT_NUMBER_ARGV_INDEX){
        printf("Error: Usage ./Server [port number]\n");
        exit(EXIT_FAILURE);
    }else{
        sscanf(argv[PORT_NUMBER_ARGV_INDEX], "%d", &__PORT_NUMBER__);
    }

    atexit(signal_exit);

    load_data();

    init_threads();

    init_network();

    listen_for_clients();

    shutdown_threads();

    shutdown_network();

    save_data();

    clean_up_dynamic_memory();

    return EXIT_SUCCESS;
}

bool is_list_empty(struct LinkedList* pList){
    return pList->pHead == NULL;
}
void list_add_tail(struct LinkedList* pList, void* pData){
    assert(pList != NULL);
    assert(pData != NULL);
    struct Link* pLink = (struct Link*)malloc(sizeof(struct Link));
    pLink->pNext = NULL;
    pLink->pData = pData;
    if(pList->pHead == NULL) {
        assert(pList->pTail == NULL);
        pList->pHead = pLink;
        pList->pTail = pLink;
    } else {
        pList->pTail->pNext = pLink;
        pList->pTail = pLink;
    }
    pList->length++;
}
void list_remove_head(struct LinkedList* pList){
    assert(pList != NULL);
    if(pList->pHead != NULL){
        struct Link* old_head = pList->pHead;
        pList->pHead = pList->pHead->pNext;
        if(pList->pHead == NULL) pList->pTail = NULL;
        free(old_head->pData);
        free(old_head);
        pList->length--;
    }else{
        assert(pList->pTail == NULL);
    }

}

int load_accounts(){

    int num_of_accounts_in_file = number_of_char_in_file('\n', file_account_location);

    FILE* file_accounts = fopen(file_account_location, "r");

    if(file_accounts == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    /* moves the file cursor past the column definition line */
    find_next_char('\n', file_accounts); //TODO:Error check here

    assert(is_list_empty(&accounts_list));

    printf("Loading Accounts (%d)...\n", num_of_accounts_in_file);
    struct Account temporary;
    for(int i = 0; i < num_of_accounts_in_file; i++){
        int loaded_correctly = fscanf(file_accounts,"%d %f %f",
                                      &temporary.AccountNo,
                                      &temporary.OpeningBal,
                                      &temporary.ClosingBal);
        if(loaded_correctly == NUM_COLUMNS_ACCOUNT){

            struct Account* pAccount = (struct Account*)malloc(sizeof(struct Account));
            assert(pAccount != NULL);
            pAccount->AccountNo = temporary.AccountNo;
            pAccount->OpeningBal = temporary.OpeningBal;
            pAccount->ClosingBal = temporary.ClosingBal;

            printf("Loaded Account (%d) of (%d) <- %d\t\t%f\t\t%f\n",
                   i+1, num_of_accounts_in_file,
                   pAccount->AccountNo,
                   pAccount->OpeningBal,
                   pAccount->ClosingBal
            );

            list_add_tail(&accounts_list, pAccount);
        } else
            fprintf(stderr, "Failed to Load Account (%d) of (%d)\n",
                    i+1, num_of_accounts_in_file);

    }

    fclose(file_accounts);



    return LOAD_SUCCESS;
}

int load_clients(){

    int num_of_clients_in_file = number_of_char_in_file('\n', file_client_location);
    FILE* file_clients = fopen(file_client_location, "r");

    if(file_clients == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    /* moves the file cursor past the column definition line */
    find_next_char('\n', file_clients);


    assert(is_list_empty(&clients_list));

    printf("Loading Clients (%d)...\n", num_of_clients_in_file);
    struct Client temporary;
    for(int i = 0; i < num_of_clients_in_file; i++){

        int loaded_correctly = fscanf(file_clients,"%15s %15s %d %d,%d,%d",
                                      temporary.FirstName,
                                      temporary.LastName,
                                      &temporary.ClientNo,
                                      &temporary.Accounts[0],
                                      &temporary.Accounts[1],
                                      &temporary.Accounts[2]);

        if(loaded_correctly > NUM_MIN_COLUMNS_CLIENT){
            for(int j = loaded_correctly; j < 6; j++){
                temporary.Accounts[j-3] = 0;
            }

            struct Client* pClient = (struct Client*)malloc(sizeof(struct Client));
            assert(pClient != NULL);
            memcpy(pClient->FirstName, temporary.FirstName, sizeof(pClient->FirstName));
            memcpy(pClient->LastName, temporary.LastName, sizeof(pClient->LastName));
            pClient->ClientNo = temporary.ClientNo;
            pClient->Accounts[0] = temporary.Accounts[0];
            pClient->Accounts[1] = temporary.Accounts[1];
            pClient->Accounts[2] = temporary.Accounts[2];
            list_add_tail(&clients_list, pClient);
            printf("Loaded Client (%d) of (%d) <- %s\t%s\t%d\t%d\t%d\t%d\n",
                   i+1, num_of_clients_in_file,
                   pClient->FirstName,
                   pClient->LastName,
                   pClient->ClientNo,
                   pClient->Accounts[0],
                   pClient->Accounts[1],
                   pClient->Accounts[2]);
        }else {
            fprintf(stderr, "Failed to Load Client (%d) of (%d)\n",
                    i + 1, num_of_clients_in_file);
        }
    }

    fclose(file_clients);

    return LOAD_SUCCESS;
}

int load_authentications(){

    int num_authentications_in_file = number_of_char_in_file('\n', file_authentication_location);
    FILE* file_authentications = fopen(file_authentication_location, "r");
    if(file_authentications == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    find_next_char('\n', file_authentications);

    printf("Loading Authentications (%d)...\n", num_authentications_in_file);
    struct Authentication temporary;
    for(int i = 0; i < num_authentications_in_file; i++) {
        int loaded_correctly = fscanf(file_authentications, "%12s %d %d",
                                      temporary.Username,
                                      &temporary.Pin,
                                      &temporary.ClientNo);
        if (loaded_correctly == NUM_COLUMNS_AUTHENTICATION){
            struct Authentication* pAuthentication = (struct Authentication*)malloc(sizeof(struct Authentication));
            memcpy(pAuthentication->Username, temporary.Username, sizeof(pAuthentication->Username));
            pAuthentication->Pin = temporary.Pin;
            pAuthentication->ClientNo = temporary.ClientNo;
            printf("Loaded Authentication (%d) of (%d) <- \t%s\t%d\t%d\n",
                   i+1, num_authentications_in_file,
                   pAuthentication->Username,
                   pAuthentication->Pin,
                   pAuthentication->ClientNo);
            list_add_tail(&authentications_list, pAuthentication);
        } else
            fprintf(stderr, "Failed to Load Authentication (%d) of (%d)\n",
                    i+1, num_authentications_in_file);

    }


    fclose(file_authentications);

    return LOAD_SUCCESS;
}

int load_transactions(){

    int num_transactions_in_file = number_of_char_in_file('\n', file_transactions_location);
    FILE* file_transactions = fopen(file_transactions_location, "r");
    if(file_transactions == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    /* moves the file cursor past the column definition line */
    find_next_char('\n', file_transactions);

    printf("Loading Transactions (%d)...\n", num_transactions_in_file);


    struct Transaction temporary;
    for(int i = 0; i < num_transactions_in_file; i++) {

        int loaded_correctly = fscanf(file_transactions, "%d %d %d %f",
                                      &temporary.FromAccount,
                                      &temporary.ToAccount,
                                      &temporary.TranType,
                                      &temporary.Amount);
        if (loaded_correctly == NUM_COLUMNS_TRANSACTION){
            struct Transaction* pTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
            assert(pTransaction != NULL);
            pTransaction->FromAccount = temporary.FromAccount;
            pTransaction->ToAccount = temporary.ToAccount;
            pTransaction->TranType = temporary.TranType;
            pTransaction->Amount = temporary.Amount;
            printf("Loaded Transaction (%d) of (%d) <- \t%d\t%d\t%d\t%f\n",
                   i+1, num_transactions_in_file,
                   pTransaction->FromAccount,
                   pTransaction->ToAccount,
                   pTransaction->TranType,
                   pTransaction->Amount);
            list_add_tail(&transactions_list, pTransaction);
        } else
            fprintf(stderr, "Failed to Load Transactions (%d) of (%d)\n",
                    i+1, num_transactions_in_file);

    }

    fclose(file_transactions);


    return LOAD_SUCCESS;
}

void load_data(void){

    int load_account_result = load_accounts();

    int load_clients_result = load_clients();

    int load_authentications_result = load_authentications();

    int load_transactions_result = load_transactions();

}

bool server_full(){
    return requests_handling < MAX_THREADS;
}

void init_network(){


    printf("int %d\n", __PORT_NUMBER__);

    assert(__PORT_NUMBER__ != -1);

    serverAddrCast = (struct sockaddr*) &serverAddr;

    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if(isNegative(listenFd)){
        fprintf(stderr, "Error: Failed to load socket!\n");
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = (in_port_t) __PORT_NUMBER__;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_result = bind(listenFd, serverAddrCast, sizeof(serverAddr));
    if(!isZero(bind_result)){
        fprintf(stderr, "Error: Failed to bind socket!\n");
    }

    int listen_result = listen(listenFd, 5);
    if(!isZero(listen_result)){
        fprintf(stderr, "Error: Failed to listen to the socket!\n");
    }

}
void shutdown_network(){
    // graceful AF
    listening_for_clients = false;
    close(listenFd);
}

bool read_status(int read, int expected){
    if(read == -1)
        return READ_UNSUCCESSFUL;
    if(read != expected)
        return READ_INCORRECT_SIZE;
    return READ_SUCCESS;
}

int read_username(int connectFd, char* username){

    int payload_size = sizeof(char) * 12;
    int read_result = read(connectFd, username, payload_size);
    return read_status(read_result, payload_size);
}
int read_pin(int connectFd, int* pin){

    int payload_size = sizeof(*pin);
    int read_result = read(connectFd, pin, payload_size);
    return read_status(read_result, payload_size);
}
int read_selection(int connectFd, int* selection){
    int payload_size = sizeof(int);
    int read_result = read(connectFd, selection, payload_size);
    return read_status(read_result, payload_size);
}
int read_return(int connectFd, bool* user_return){
    int payload_size = sizeof(bool);
    int read_result = read(connectFd, user_return, payload_size);
    return read_status(read_result, payload_size);
}
int read_account_index(int connectFd, int* account_index){
    int payload_size = sizeof(int);
    int read_result = read(connectFd, account_index, payload_size);
    return read_status(read_result, payload_size);
}
int read_amount(int connectFd, float* amount){
    int payload_size = sizeof(float);
    int read_result = read(connectFd, &amount, payload_size);
    return read_status(read_result, payload_size);
}
int save_accounts(){
    FILE* file_accounts = fopen(file_account_location, "w");
    if(file_accounts == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Loading Accounts (%d)...\n", accounts_list.length);

    fprintf(file_accounts, "AccountNo      OpeningBal     ClosingBal     ");
    struct Link* pSave = accounts_list.pHead;
    int i = 0;
    while(pSave != NULL){

        fprintf(file_accounts, "\n%-14d %-14.2f %0.2f",
                ((struct Account*)(pSave->pData))->AccountNo,
                ((struct Account*)(pSave->pData))->OpeningBal,
                ((struct Account*)(pSave->pData))->ClosingBal
        );
        printf("Saved Account (%d) of (%d) <- %d\t\t%f\t\t%f\n",
               i+1, accounts_list.length,
               ((struct Account*)(pSave->pData))->AccountNo,
               ((struct Account*)(pSave->pData))->OpeningBal,
               ((struct Account*)(pSave->pData))->ClosingBal
        );

        pSave = pSave->pNext;
        i++;
    }
    fclose(file_accounts);

    return SAVE_SUCCESS;
}
int save_clients() {
    FILE *file_clients = fopen(file_client_location, "w");
    if (file_clients == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Saving Clients (%d)...\n", clients_list.length);

    char client_number[11];
    fprintf(file_clients, "Firstname      Lastname       ClientNo           Accounts");
    struct Link *pSave = clients_list.pHead;
    int i = 0;
    while (pSave != NULL) {
        int_to_char(((struct Client*)(pSave->pData))->ClientNo, client_number, 11);
        fprintf(file_clients, "\n%-14s %-14s %-18s %d",
                ((struct Client*)(pSave->pData))->FirstName,
                ((struct Client*)(pSave->pData))->LastName,
                client_number,
                ((struct Client*)(pSave->pData))->Accounts[0]
        );
        if (((struct Client*)(pSave->pData))->Accounts[1] != 0) {
            fprintf(file_clients, ",%d", ((struct Client*)(pSave->pData))->Accounts[1]);
        }
        if (((struct Client*)(pSave->pData))->Accounts[2] != 0) {
            fprintf(file_clients, ",%d", ((struct Client*)(pSave->pData))->Accounts[2]);
        }

        printf("Saved Client (%d) of (%d) <- %s\t%s\t%d\t%d\t%d\t%d\n",
               i+1, clients_list.length,
               ((struct Client*)(pSave->pData))->FirstName,
               ((struct Client*)(pSave->pData))->LastName,
               ((struct Client*)(pSave->pData))->ClientNo,
               ((struct Client*)(pSave->pData))->Accounts[0],
               ((struct Client*)(pSave->pData))->Accounts[1],
               ((struct Client*)(pSave->pData))->Accounts[2]);
        pSave = pSave->pNext;
        i++;
    }

    fclose(file_clients);

    return SAVE_SUCCESS;
}

int save_authentication(){

    FILE* file_authentication = fopen(file_authentication_location, "w");
    if(file_authentication == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Saving Authentications (%d)...\n", authentications_list.length);

    fprintf(file_authentication, "Username        PIN         ClientNo");
    struct Link* pSave = authentications_list.pHead;
    int i = 0;
    while(pSave != NULL){

        fprintf(file_authentication, "\n%-15s %-11d %d",
                ((struct Authentication*)(pSave->pData))->Username,
                ((struct Authentication*)(pSave->pData))->Pin,
                ((struct Authentication*)(pSave->pData))->ClientNo
        );
        printf("Saved Authentication (%d) of (%d) <- %s\t%d\t%d\n",
               i+1, authentications_list.length,
               ((struct Authentication*)(pSave->pData))->Username,
               ((struct Authentication*)(pSave->pData))->Pin,
               ((struct Authentication*)(pSave->pData))->ClientNo
        );

        pSave = pSave->pNext;
        i++;
    }
    fclose(file_authentication);

    return SAVE_SUCCESS;
}

int save_transactions(){
    FILE* file_transactions = fopen(file_transactions_location, "w");
    if(file_transactions == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Saving Transactions (%d)...\n", transactions_list.length);

    fprintf(file_transactions, "FromAccount      ToAccount       TranType    Amount");
    struct Link* pSave = transactions_list.pHead;
    int i = 0;
    while(pSave != NULL){

        fprintf(file_transactions, "\n%-14d %-14d %-14d %0.2f",
                ((struct Transaction*)(pSave->pData))->FromAccount,
                ((struct Transaction*)(pSave->pData))->ToAccount,
                ((struct Transaction*)(pSave->pData))->TranType,
                ((struct Transaction*)(pSave->pData))->Amount
        );
        printf("Saved Transaction (%d) of (%d) <- %d\t\t%d\t\t%d\t%f\n",
               i+1, accounts_list.length,
               ((struct Transaction*)(pSave->pData))->FromAccount,
               ((struct Transaction*)(pSave->pData))->ToAccount,
               ((struct Transaction*)(pSave->pData))->TranType,
               ((struct Transaction*)(pSave->pData))->Amount
        );

        pSave = pSave->pNext;
        i++;
    }
    fclose(file_transactions);

    return SAVE_SUCCESS;
}

void save_data(void){
    save_accounts();
    save_clients();
    save_authentication();
    save_transactions();
}
struct Authentication* serve_client_login(int connectFd){

    struct Authentication* auth = malloc(sizeof(struct Authentication));
    for(int i = 0; i < 12; i++) auth->Username[i] = '\0';


    switch (read_username(connectFd, auth->Username)) {
        case READ_SUCCESS:
            printf("Status: Read username successfully (%s).\n", auth->Username);
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read username incorrect size (%s)!\n", auth->Username);
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read username unsuccessfully (%s)!\n", auth->Username);
            free(auth);
            return NULL;
    }



    switch (read_pin(connectFd, &auth->Pin)) {
        case READ_SUCCESS:
            printf("Status: Read pin successfully (%d).\n", auth->Pin);
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read pin incorrect size (%d)!\n", auth->Pin);
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read pin unsuccessfully (%d)!\n", auth->Pin);
            free(auth);
            return NULL;
    }



    bool authenticated = Authenticate(auth);
    int login_ret, write_result;
    if (authenticated) {
        login_ret = LOGIN_SUCCESS;
        printf("Status: Authenticated correctly (%d)!\n", LOGIN_SUCCESS);
        write_result = write_login_status(connectFd, LOGIN_SUCCESS);
    } else {
        login_ret = LOGIN_INCORRECT;
        fprintf(stderr, "Status: Incorrect login attempt (%d)!\n", LOGIN_INCORRECT);
        write_result = write_login_status(connectFd, LOGIN_INCORRECT);
    }

    switch (write_result) {
        case WRITE_SUCCESS:
            printf("Status: Write login status successfully (%d)!\n", login_ret);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write login status incorrect size (%d)!\n", login_ret);
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write login status disconnected (%d)!\n", login_ret);
            free(auth);
            return NULL;
    }

    if(login_ret == LOGIN_INCORRECT){
        free(auth);
        return NULL;
    }





    switch (write_client_number(connectFd, auth->ClientNo)) {
        case WRITE_SUCCESS:
            printf("Status: Write client number successfully (%d)!\n", auth->ClientNo);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write client number incorrect size (%d)!\n", auth->ClientNo);
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write client number disconnected (%d)!\n", auth->ClientNo);
            free(auth);
            return NULL;
    }



    assert(auth->ClientNo != 0);
    char* firstname = find_firstname(auth->ClientNo); /*DONT FREE*/
    assert(auth->ClientNo != 0);

    switch (write_firstname(connectFd, firstname)) {
        case WRITE_SUCCESS:
            printf("Status: Write first name successfully (%s)!\n", firstname);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write first name incorrect size (%s)!\n", firstname);
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write first name disconnected (%s)!\n", firstname);
            free(auth);
            return NULL;
    }


    char* lastname = find_lastname(auth->ClientNo); /*DONT FREE*/
    assert(lastname != NULL);

    switch (write_lastname(connectFd, lastname)) {
        case WRITE_SUCCESS:
            printf("Status: Write last name successfully! (%s)\n", lastname);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write last name incorrect size (%s)!\n", lastname);
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write last name disconnected (%s)!\n", lastname);
            free(auth);
            return NULL;
    }

    return auth;

}

int server_client_account_balance(int connectFd, struct Authentication* auth){
    printf("Menu: Account balance for user (%s, %d, %d)\n", auth->Username, auth->Pin, auth->ClientNo);
    struct Account* accounts = find_accounts(auth->ClientNo);

    switch (write_savings_account(connectFd, &accounts[ACCOUNT_INDEX_SAVINGS])){
        case WRITE_SUCCESS:
            printf("Status: Write saving account successfully (%d, %f, %f)!\n",
                   accounts[ACCOUNT_INDEX_SAVINGS].AccountNo,
                   accounts[ACCOUNT_INDEX_SAVINGS].OpeningBal,
                   accounts[ACCOUNT_INDEX_SAVINGS].ClosingBal);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write savings account incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Read savings account disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    };

    switch (write_loan_account(connectFd, &accounts[ACCOUNT_INDEX_LOAN])){
        case WRITE_SUCCESS:
            printf("Status: Write loan accounts successfully (%d, %f, %f)!\n",
                   accounts[ACCOUNT_INDEX_LOAN].AccountNo,
                   accounts[ACCOUNT_INDEX_LOAN].OpeningBal,
                   accounts[ACCOUNT_INDEX_LOAN].ClosingBal);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write loan account incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Read loan account disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    };

    switch (write_credit_account(connectFd, &accounts[ACCOUNT_INDEX_CREDIT])){
        case WRITE_SUCCESS:
            printf("Status: Write credit account successfully (%d, %f, %f)!\n",
                   accounts[ACCOUNT_INDEX_CREDIT].AccountNo,
                   accounts[ACCOUNT_INDEX_CREDIT].OpeningBal,
                   accounts[ACCOUNT_INDEX_CREDIT].ClosingBal);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write credit account incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Read credit account disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    };

    bool user_return;
    switch(read_return(connectFd, &user_return)){
        case READ_SUCCESS:
            printf("Status: Read return successfully!\n");
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read return incorrect size!\n");
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read return disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    }
    while(!user_return){
        switch (write_savings_account(connectFd, &accounts[ACCOUNT_INDEX_SAVINGS])){
            case WRITE_SUCCESS:
                printf("Status: Write saving account successfully (%d, %f, %f)!\n",
                       accounts[ACCOUNT_INDEX_SAVINGS].AccountNo,
                       accounts[ACCOUNT_INDEX_SAVINGS].OpeningBal,
                       accounts[ACCOUNT_INDEX_SAVINGS].ClosingBal);
                break;
            case WRITE_INCORRECT_SIZE:
                fprintf(stderr, "Error: Write savings account incorrect size!\n");
                break;
            case WRITE_DISCONNECTED:
                fprintf(stderr, "Error: Read savings account disconnected!\n");
                free(accounts);
                return CLIENT_DISCONNECTED;
        };

        switch (write_loan_account(connectFd, &accounts[ACCOUNT_INDEX_LOAN])){
            case WRITE_SUCCESS:
                printf("Status: Write loan accounts successfully (%d, %f, %f)!\n",
                       accounts[ACCOUNT_INDEX_LOAN].AccountNo,
                       accounts[ACCOUNT_INDEX_LOAN].OpeningBal,
                       accounts[ACCOUNT_INDEX_LOAN].ClosingBal);
                break;
            case WRITE_INCORRECT_SIZE:
                fprintf(stderr, "Error: Write loan account incorrect size!\n");
                break;
            case WRITE_DISCONNECTED:
                fprintf(stderr, "Error: Read loan account disconnected!\n");
                free(accounts);
                return CLIENT_DISCONNECTED;
        };

        switch (write_credit_account(connectFd, &accounts[ACCOUNT_INDEX_CREDIT])){
            case WRITE_SUCCESS:
                printf("Status: Write credit account successfully (%d, %f, %f)!\n",
                       accounts[ACCOUNT_INDEX_CREDIT].AccountNo,
                       accounts[ACCOUNT_INDEX_CREDIT].OpeningBal,
                       accounts[ACCOUNT_INDEX_CREDIT].ClosingBal);
                break;
            case WRITE_INCORRECT_SIZE:
                fprintf(stderr, "Error: Write credit account incorrect size!\n");
                break;
            case WRITE_DISCONNECTED:
                fprintf(stderr, "Error: Read credit account disconnected!\n");
                free(accounts);
                return CLIENT_DISCONNECTED;
        };

        switch(read_return(connectFd, &user_return)){
            case READ_SUCCESS:
                printf("Status: Read return successfully!\n");
                break;
            case READ_INCORRECT_SIZE:
                fprintf(stderr, "Error: Read return incorrect size!\n");
                break;
            case READ_UNSUCCESSFUL:
                fprintf(stderr, "Error: Read return disconnected!\n");
                free(accounts);
                return CLIENT_DISCONNECTED;
        }
    }

    free(accounts);
    return CLIENT_RETURNED;

}

int server_client_withdrawal(int connectFd, struct Authentication* auth){
    printf("Menu: Withdrawal for user (%s, %d, %d)\n", auth->Username, auth->Pin, auth->ClientNo);
    struct Account* accounts = find_accounts(auth->ClientNo);

    switch (write_accounts(connectFd, accounts)){
        case WRITE_SUCCESS:
            printf("Status: Write accounts successfully!\n");
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write accounts incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write accounts disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    }


    bool user_return;
    switch(read_return(connectFd, &user_return)){
        case READ_SUCCESS:
            printf("Status: Read return successfully!\n");
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read return incorrect size!\n");
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read return disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    }

    if(user_return) return CLIENT_RETURNED;

    int account_index;
    switch(read_account_index(connectFd, &account_index)){
        case READ_SUCCESS:
            printf("Status: Read account index successfully (%d)!\n", account_index);
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read account index incorrect size (%d)!\n", account_index);
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read account index disconnected (%d)!\n", account_index);
            free(accounts);
            return CLIENT_DISCONNECTED;
    }

    user_return;
    switch(read_return(connectFd, &user_return)){
        case READ_SUCCESS:
            printf("Status: Read return successfully!\n");
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read return incorrect size!\n");
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read return disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    }
    if(user_return) return CLIENT_RETURNED;

    float withdrawal_amount;
    switch(read_amount(connectFd, &withdrawal_amount)){
        case READ_SUCCESS:
            printf("Status: Read withdrawal index successfully (%f)!\n", withdrawal_amount);
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read withdrawal index incorrect size (%f)!\n", withdrawal_amount);
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read withdrawal index disconnected (%f)!\n", withdrawal_amount);
            free(accounts);
            return CLIENT_DISCONNECTED;
    }

    bool successful_withdrawal = can_withdrawal(&accounts[account_index], account_index, withdrawal_amount);
    if(successful_withdrawal)
        withdraw(&accounts[account_index], withdrawal_amount);

    switch(write_accepted_withdraw(connectFd, &successful_withdrawal)){
        case WRITE_SUCCESS:
            printf("Status: Write withdrawal status successfully!\n");
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write withdrawal status incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write withdrawal status disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    }

    free(accounts);
    accounts = find_accounts(auth->ClientNo);
    switch (write_accounts(connectFd, accounts)){
        case WRITE_SUCCESS:
            printf("Status: Write accounts successfully!\n");
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write accounts incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write accounts disconnected!\n");
            free(accounts);
            return CLIENT_DISCONNECTED;
    }





    return CLIENT_RETURNED;
}

void server_client_main_menu(int connectFd, struct Authentication* auth){

    int selection = 0, client_status;
    bool exit_main_menu = false;
    do{
        printf("Menu: Main menu for user (%s, %d, %d)\n", auth->Username, auth->Pin, auth->ClientNo);
        switch(read_selection(connectFd, &selection)){
            case READ_SUCCESS:
                printf("Status: Read selection success (%d).\n", selection);
                break;
            case READ_INCORRECT_SIZE:
                fprintf(stderr, "Error: Read selection error (%d).\n", selection);
                break;
            case READ_UNSUCCESSFUL:
                fprintf(stderr, "Error: Read selection unsuccessful (%d).\n", selection);
                client_status = CLIENT_DISCONNECTED;
                break;
        }

        switch(selection){
            case SELECTION_ACCOUNT_BALANCE:
                client_status = server_client_account_balance(connectFd, auth);
                break;
            case SELECTION_WITHDRAWAL:
                client_status = server_client_withdrawal(connectFd, auth);
                break;
        }


    }while(selection != 6 && client_status != CLIENT_DISCONNECTED);


}


void add_request(int request_num, int connectFd, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var) {
    int rc;                         /* return code of pthreads functions.  */
    struct request* a_request;      /* pointer to newly added request.     */

    a_request = (struct request*)malloc(sizeof(struct request));
    if (!a_request) { /* malloc failed?? */
        fprintf(stderr, "add_request: out of memory\n");
        exit(1);
    }
    a_request->number = request_num;
    a_request->connectFd = connectFd;
    a_request->next = NULL;

    rc = pthread_mutex_lock(p_mutex);

    if (num_requests == 0) { /* special case - list is empty */
        requests = a_request;
        last_request = a_request;
    }
    else {
        last_request->next = a_request;
        last_request = a_request;
    }

    num_requests++;

    rc = pthread_mutex_unlock(p_mutex);

    rc = pthread_cond_signal(p_cond_var);
}

struct request* get_request(pthread_mutex_t* p_mutex) {
    int rc;
    struct request* a_request;

    rc = pthread_mutex_lock(p_mutex);

    if (num_requests > 0) {
        a_request = requests;
        requests = a_request->next;
        if (requests == NULL) {
            last_request = NULL;
        }
        num_requests--;
    }
    else {
        a_request = NULL;
    }

    rc = pthread_mutex_unlock(p_mutex);

    return a_request;
}

void handle_request(struct request* a_request, int thread_id) {
    requests_handling++;
    if (a_request) {
        printf("Status: Thread '%d' is handling request '%d' of '%d' being currently handled.\n",
               thread_id, a_request->number, requests_handling);
        fflush(stdout);
    }
    struct Authentication* auth = serve_client_login(a_request->connectFd);
    if(auth != NULL){
        server_client_main_menu(a_request->connectFd, auth);
        free(auth);
    }else{
        printf("Status: Client failed to login.\n");
    }
    printf("Status: Thread '%d' is disconnecting from client with request id '%d'!\n",
           thread_id, a_request->number);
    disconnect_from_client(a_request->connectFd);
    requests_handling--;
}

void* handle_requests_loop(void* data){
    int rc;
    struct request* a_request;
    int thread_id = *((int*)data);

    rc = pthread_mutex_lock(&request_mutex);

    while (!close_all_threads) {

        if (num_requests > 0) {
            a_request = get_request(&request_mutex);
            if (a_request) {
                rc = pthread_mutex_unlock(&request_mutex);
                handle_request(a_request, thread_id);
                free(a_request);
                rc = pthread_mutex_lock(&request_mutex);
            }
        } else {
            rc = pthread_cond_wait(&got_request, &request_mutex);
        }
    }
}

void init_threads(){
    close_all_threads = false;
    for (int i=0; i<MAX_THREADS; i++) {
        thr_id[i] = i;
        pthread_create(&p_threads[i], NULL, handle_requests_loop, (void*)&thr_id[i]);
        printf("Status: Created thread %d for handle_requests_loop(void*)\n", thr_id[i]);
    }
}

void shutdown_threads(){
    close_all_threads = true;
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_join(thr_id[i], NULL);
    }
}

void disconnect_from_client(int connectFd){
    shutdown(connectFd, 2);
    close(connectFd);
}


int listen_for_clients(){

    int i = 1, connectFd;

    while(listening_for_clients){

        connectFd = accept(listenFd, (struct sockaddr*)NULL, NULL);

        if(server_full()){

            int write_result = write_server_status(connectFd, SERVER_STATUS_SERVER_NOT_FULL);
            switch(write_result){
                case WRITE_SUCCESS:
                    printf("Status: Write server status successfully.\n");
                    add_request(i, connectFd, &request_mutex, &got_request);
                    i++;
                    break;
                case WRITE_INCORRECT_SIZE:
                    fprintf(stderr, "Error: Write server status, disconnecting client!\n");
                    disconnect_from_client(connectFd);
                    break;
                case WRITE_DISCONNECTED:
                    fprintf(stderr, "Error: Write server status disconnected!\n");
                    disconnect_from_client(connectFd);
                    break;
            }


        }else{

            int write_result = write_server_status(connectFd, SERVER_STATUS_SERVER_FULL);
            switch(write_result){
                case WRITE_SUCCESS:
                    printf("Status: Server full, disconnecting new client!\n");
                    disconnect_from_client(connectFd);
                    break;
                case WRITE_INCORRECT_SIZE:
                    fprintf(stderr, "Error: Server Write incorrect size for server status, disconnecting client!\n");
                    disconnect_from_client(connectFd);
                    break;
                case WRITE_DISCONNECTED:
                    fprintf(stderr, "Error: Client disconnected!\n");
                    disconnect_from_client(connectFd);
                    break;
            }

        }

    }

}


bool are_equal(const char *s1, const char *s2) {
    if(strlen(s1) != strlen(s2)) return false;
    for(int i = 0; i < strlen(s1); i++){
        if(s1[i] != s2[i]) return false;
    }
    return true;
}
bool isNegative(int val){return val < 0;};
bool isZero(int val){return val == 0;};
int number_of_char_in_file(const char c, const char* file_location){
    FILE* file = fopen(file_location, "r");
    if(file == NULL) return -1;
    int num_char = 0;
    char ch;
    //printf("-------%s-------\n",file_location);
    while(!feof(file)){
        ch = (char)fgetc(file);
        //printf("%c", ch);
        if(c == ch){
            num_char++;
        }
    }
    //printf("\n-------%s-------\n",file_location);
    fclose(file);
    return num_char;
}
void int_to_char(int val, char* str, int size){
    for(int i = 0; i < size; i++) str[i] = ' ';
    sprintf(str,"%d", val);
}
int find_next_char(char c, FILE* file){
    char ch;
    while(!feof(file)){
        ch = (char)fgetc(file);
        if(ch == c){
            return FIND_NEXT_CHAR_SUCCESS;
        }
    }
    return FIND_NEXT_CHAR_EOF;
}
int find_next_char_n(char c, int n, FILE* file){
    if(file == NULL) return -1;
    int result = 0;
    for(int i = 0; i < n; i++){
        result = find_next_char(c, file);
        if(result == FIND_NEXT_CHAR_EOF){
            return FIND_NEXT_CHAR_EOF;
        }
    }
    return FIND_NEXT_CHAR_SUCCESS;
}


bool Authenticate(struct Authentication* auth){

    struct Link* pLink = authentications_list.pHead;
    while(pLink != NULL){
        struct Authentication* pAuthData = pLink->pData;
        if(are_equal(pAuthData->Username, auth->Username)){
            if(pAuthData->Pin == auth->Pin){
                auth->ClientNo = pAuthData->ClientNo;
                return true;
            }else{
                return false;
            }

        }
        pLink = pLink->pNext;
    }
    return false;
}

int clean_up_dynamic_memory(){
    while(!is_list_empty(&accounts_list)) list_remove_head(&accounts_list);
    while(!is_list_empty(&clients_list)) list_remove_head(&clients_list);
    while(!is_list_empty(&authentications_list)) list_remove_head(&authentications_list);
    while(!is_list_empty(&transactions_list)) list_remove_head(&transactions_list);
}

struct Account* find_accounts(int clientNo){
    struct Account* found_accounts = calloc(3, sizeof(struct Account));

    struct Link* pSearchClient = clients_list.pHead;
    while(pSearchClient != NULL){
        struct Client* pClient = pSearchClient->pData;
        if(pClient->ClientNo == clientNo){
            for(int i = 0; i < 3; i++){
                found_accounts[i].AccountNo = pClient->Accounts[i];
                printf("Status: Found client (%d)'s account (%d) with account number (%d).\n",
                       pClient->ClientNo, i, found_accounts[i].AccountNo);
            }
            break;
        }
        pSearchClient = pSearchClient->pNext;
    }

    struct Link* pSearchAccount = accounts_list.pHead;
    while(pSearchAccount != NULL){
        struct Account* pAccount = pSearchAccount->pData;
        for(int i = 0; i < 3; i++){
            if(found_accounts[i].AccountNo == pAccount->AccountNo){
                found_accounts[i].OpeningBal = pAccount->OpeningBal;
                found_accounts[i].ClosingBal = pAccount->ClosingBal;
                printf("Status: Found openingbal, closingbal ($%f, $%f) for account (%d).\n",
                       pAccount->OpeningBal, pAccount->ClosingBal,
                       pAccount->AccountNo);
                break;
            }
        }
        pSearchAccount = pSearchAccount->pNext;
    }

    return found_accounts;
}

bool can_withdrawal(struct Account* account, int account_index, float amount){
    switch(account_index){
        case ACCOUNT_INDEX_SAVINGS:
            return account->ClosingBal - amount >= 0.0f;
        case ACCOUNT_INDEX_LOAN:
            return account->ClosingBal - amount >= -5000.0f;
        case ACCOUNT_INDEX_CREDIT:
            return account->ClosingBal - amount >= 0.0f;
    }

}
void withdraw(struct Account* account, float amount){
    account->ClosingBal -= amount;
}
char* find_firstname(int clientNo){
    struct Link* pCheck = clients_list.pHead;
    struct Client* pData = pCheck->pData;
    while(pCheck != NULL){

        if(clientNo == pData->ClientNo){
            printf("Found: first name (%s) for client (%d).\n", pData->FirstName, pData->ClientNo);
            return pData->FirstName;
        }
        pCheck = pCheck->pNext;
    }
    fprintf(stderr, "Error: find_firstname(int clientNo) -> could not find match!\n");
    return NULL;
}
char* find_lastname(int clientNo){
    struct Link* pCheck = clients_list.pHead;
    struct Client* pData = pCheck->pData;
    while(pCheck != NULL){
        if(clientNo == pData->ClientNo){
            printf("Found: last name (%s) for client (%d).\n", pData->LastName, pData->ClientNo);
            return pData->LastName;
        }
        pCheck = pCheck->pNext;
    }
    fprintf(stderr, "Error: find_lastname(int clientNo) -> could not find match!\n");
    return NULL;
}

bool write_status(int written, int expected){
    if(written == -1)
        return WRITE_DISCONNECTED;
    if(written != expected)
        return WRITE_INCORRECT_SIZE;
    return WRITE_SUCCESS;
}

int write_savings_account(int connectFd, struct Account* account){
    int payload_size = sizeof(struct Account);
    int write_result = write(connectFd, &account, payload_size);
    return write_status(write_result, payload_size);
}
int write_loan_account(int connectFd, struct Account* account){
    int payload_size = sizeof(struct Account);
    int write_result = write(connectFd, &account, payload_size);
    return write_status(write_result, payload_size);
}
int write_credit_account(int connectFd, struct Account* account){
    int payload_size = sizeof(struct Account);
    int write_result = write(connectFd, &account, payload_size);
    return write_status(write_result, payload_size);
}
int write_accounts(int connectFd, struct Account* accounts){
    int payload_size = sizeof(struct Account) * 3;
    int write_result = write(connectFd, &accounts, payload_size);
    return write_status(write_result, payload_size);
}
int write_accepted_withdraw(int connectFd, bool accepted){
    int payload_size = sizeof(bool);
    int write_result = write(connectFd, &accepted, payload_size);
    return write_status(write_result, payload_size);
}
int write_login_status(int connectFd, int login_status){
    int payload_size = sizeof(int);
    int write_result = write(connectFd, &login_status, payload_size);
    return write_status(write_result, payload_size);
}
int write_client_number(int connectFd, int client_number){
    int payload_size = sizeof(int);
    int write_result = write(connectFd, &client_number, payload_size);
    return write_status(write_result, payload_size);
}
int write_firstname(int connectFd, const char* firstname){
    int payload_size = sizeof(char) * 15;
    int write_result = write(connectFd, firstname, payload_size);
    return write_status(write_result, payload_size);
}
int write_lastname(int connectFd, const char* lastname){
    int payload_size = sizeof(char) * 15;
    int write_result = write(connectFd, lastname, payload_size);
    return write_status(write_result, payload_size);
}
int write_server_status(int connectFd, int server_status){
    int payload_size = sizeof(server_status);
    int server_status_write_result = write(connectFd, &server_status, payload_size);
    return write_status(server_status_write_result, payload_size);
}