//
// Created by carlin on 10/24/16.
//

#include "threadpool.h"

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