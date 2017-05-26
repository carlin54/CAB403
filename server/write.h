//
// Created by carlin on 10/24/16.
//
#include "global.h"
#include "utility.h"

#ifndef CAB403_SERVER_WRITE_H
#define CAB403_SERVER_WRITE_H




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


#endif //CAB403_WRITE_H
