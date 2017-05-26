//
// Created by carlin on 10/24/16.
//

#include "read.h"

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
