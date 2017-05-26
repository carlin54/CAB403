//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_SERVER_THREADPOOL_H
#define CAB403_SERVER_THREADPOOL_H

#include "global.h"
#include "read.h"
#include "write.h"
#include "network.h"

struct Authentication* serve_client_login(int connectFd);

int server_client_account_balance(int connectFd, struct Authentication* auth);

int server_client_withdrawal(int connectFd, struct Authentication* auth);

void server_client_main_menu(int connectFd, struct Authentication* auth);

void disconnect_from_client(int connectFd);

void add_request(int request_num, int connectFd, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var);

struct request* get_request(pthread_mutex_t* p_mutex);

void handle_request(struct request* a_request, int thread_id);

void* handle_requests_loop(void* data);

void init_threads();

void shutdown_threads();

int listen_for_clients();



#endif //CAB403_THREADPOOL_H
