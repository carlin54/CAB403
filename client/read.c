//
// Created by carlin on 10/24/16.
//

#include "read.h"

int read_status(int read, int expected){
    if(read == -1)
        return READ_UNSUCCESSFUL;
    if(read != expected)
        return READ_INCORRECT_SIZE;
    return READ_SUCCESS;
}

int read_client_number(int* client_number){
    int payload_size = sizeof(int);
    int read_result = read(sockFd, (void*)client_number, payload_size);
    return read_status(read_result, payload_size);
}
int read_firstname(char* firstname){
    int payload_size = sizeof(char) * 15;
    int read_result = read(sockFd, (void*)&firstname[0], payload_size);
    return read_status(read_result, payload_size);
}
int read_lastname(char* lastname){
    int payload_size = sizeof(char) * 15;
    int read_result = read(sockFd, (void*)&lastname[0], payload_size);
    return read_status(read_result, payload_size);
}
int read_login_status(int* login_status){
    int payload_size = sizeof(int);
    int read_result = read(sockFd, (void*)login_status, payload_size);
    return read_status(read_result, payload_size);
}
int read_server_status(int* server_status){
    int payload_size = sizeof(int);
    int read_result = read(sockFd, &server_status, payload_size);
    return read_status(read_result, payload_size);
}
int read_accounts(struct Account* accounts){
    int payload_size = sizeof(struct Account) * 3;
    int read_result = read(sockFd, (void*)accounts, payload_size);
    return read_status(read_result, payload_size);
}
int read_savings_account(struct Account* account){
    int payload_size = sizeof(struct Account);
    int read_result = read(sockFd, (void*)account, payload_size);
    return read_status(read_result, payload_size);
}
int read_loan_account(struct Account* account){
    int payload_size = sizeof(struct Account);
    int read_result = read(sockFd, (void*)account, payload_size);
    return read_status(read_result, payload_size);
}
int read_credit_account(struct Account* account){
    int payload_size = sizeof(struct Account);
    int read_result = read(sockFd, (void*)account, payload_size);
    return read_status(read_result, payload_size);
}
int read_successful_withdrawal(bool* withdrawal){
    int payload_size = sizeof(bool);
    int read_result = read(sockFd, (void*)withdrawal, payload_size);
    return read_status(read_result, payload_size);
}