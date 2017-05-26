//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_CLIENT_READ_H
#define CAB403_CLIENT_READ_H

#include "global.h"

#define READ_SUCCESS        0
#define READ_UNSUCCESSFUL   1
#define READ_INCORRECT_SIZE 2

int read_status(int read, int expected);

int read_client_number(int* client_number);
int read_firstname(char* firstname);
int read_lastname(char* lastname);
int read_login_status(int* login_status);
int read_server_status(int* server_status);
int read_accounts(struct Account* accounts);
int read_savings_account(struct Account* account);
int read_loan_account(struct Account* account);
int read_credit_account(struct Account* account);
int read_successful_withdrawal(bool* withdrawal);

#endif //CAB403_READ_H
