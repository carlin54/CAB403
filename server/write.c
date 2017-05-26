//
// Created by carlin on 10/24/16.
//

#include "write.h"


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