//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_SERVER_READ_H
#define CAB403_SERVER_READ_H

#include "global.h"

#define READ_SUCCESS        0
#define READ_UNSUCCESSFUL   1
#define READ_INCORRECT_SIZE 2

bool read_status(int read, int expected);

int read_username(int connectFd, char* username);
int read_pin(int connectFd, int* pin);
int read_selection(int connectFd, int* selection);
int read_return(int connectFd, bool* user_return);
int read_account_index(int connectFd, int* account_index);
int read_amount(int connectFd, float* amount);

#endif //CAB403_READ_H
