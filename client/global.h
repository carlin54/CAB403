//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_GLOBAL_H
#define CAB403_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_RETURNED      0
#define SERVER_DISCONNECTED -1

#define SELECTION_ACCOUNT_BALANCE       1
#define SELECTION_WITHDRAWAL            2
#define SELECTION_DEPOSIT               3
#define SELECTION_TRANSFER              4
#define SELECTION_TRANSACTION_LISTING   5
#define SELECTION_EXIT                  6

#define ACCOUNT_INDEX_SAVINGS   0
#define ACCOUNT_INDEX_LOAN      1
#define ACCOUNT_INDEX_CREDIT    2


#define SERVER_STATUS_SERVER_NOT_FULL   (0)
#define SERVER_STATUS_SERVER_FULL       (1)

#define LOGIN_SUCCESS            0
#define LOGIN_INCORRECT         -1
#define LOGIN_TOO_MANY_ATTEMPTS -2
#define LOGIN_COULD_NOT_READ    -3
#define LOGIN_COULD_NOT_WRITE   -4

#define REQUIRED_NUMBER_OF_ARGC 3
#define PORT_NUMBER_ARGV_INDEX  1
#define IP_ADDRESS_ARGV_INDEX   2

typedef float Money;
typedef int AccountNumber;
typedef int ClientNumber;

struct Account {
    AccountNumber AccountNo;
    Money OpeningBal;
    Money ClosingBal;
};

int __ClientNumber__;
int __Pin__;
char __UserName__[12];
char __FirstName__[15];
char __LastName__[15];
int __NumberOfAccountsAvailable__;
struct Account __Accounts__[3];

int port_number;
bool connected_to_server;
int sockFd;
int connectFd;
bool exit_main_menu_screen;
bool is_logged_in;

#endif //CAB403_GLOBAL_H
