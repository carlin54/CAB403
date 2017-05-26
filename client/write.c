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

int write_menu_account_balance(){
    int payload = 1;
    int payload_size = sizeof(int);
    int write_result = write(sockFd, &payload, payload_size);
    return write_status(write_result, payload_size);
}
int write_menu_withdraw(){
    int payload = 2;
    int payload_size = sizeof(int);
    int write_result = write(sockFd, &payload, payload_size);
    return write_status(write_result, payload_size);
}
int write_menu_deposit(){
    int payload = 3;
    int payload_size = sizeof(int);
    int write_result = write(sockFd, &payload, payload_size);
    return write_status(write_result, payload_size);
}
int write_menu_transfer(){
    int payload = 4;
    int payload_size = sizeof(int);
    int write_result = write(sockFd, &payload, payload_size);
    return write_status(write_result, payload_size);
}
int write_menu_transactions(){
    int payload = 5;
    int payload_size = sizeof(int);
    int write_result = write(sockFd, &payload, payload_size);
    return write_status(write_result, payload_size);
}
int write_menu_exit(){
    int payload = 6;
    int payload_size = sizeof(int);
    int write_result = write(sockFd, &payload, payload_size);
    return write_status(write_result, payload_size);
}
int write_return(bool user_return){
    int payload_size = sizeof(bool);
    int write_result = write(sockFd, &user_return, payload_size);
    return write_status(write_result, payload_size);
}
int write_account_index(int account_index){
    int payload_size = sizeof(int);
    int write_result = write(sockFd, &account_index, payload_size);
    return write_status(write_result, payload_size);
}
int write_amount(float amount){
    int payload_size = sizeof(float);
    int write_result = write(sockFd, &amount, payload_size);
    return write_status(write_result, payload_size);
}
int write_username(const char* username){
    int payload_size = sizeof(__UserName__);
    int write_result = write(sockFd, username, payload_size);
    return write_status(write_result, payload_size);
}
int write_pin(int pin){
    int payload_size = sizeof(pin);
    int write_result = write(sockFd, &pin, payload_size);
    return write_status(write_result, payload_size);
}