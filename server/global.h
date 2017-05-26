//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_SERVER_GLOBAL_H
#define CAB403_SERVER_GLOBAL_H

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



#endif //CAB403_GLOBAL_H

