//
// Created by carlin on 10/24/16.
//

#include "network.h"

bool server_full(){
    return requests_handling < MAX_THREADS;
}

void init_network(){


    printf("int %d\n", __PORT_NUMBER__);

    assert(__PORT_NUMBER__ != -1);

    serverAddrCast = (struct sockaddr*) &serverAddr;

    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if(isNegative(listenFd)){
        fprintf(stderr, "Error: Failed to load socket!\n");
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = (in_port_t) __PORT_NUMBER__;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_result = bind(listenFd, serverAddrCast, sizeof(serverAddr));
    if(!isZero(bind_result)){
        fprintf(stderr, "Error: Failed to bind socket!\n");
    }

    int listen_result = listen(listenFd, 5);
    if(!isZero(listen_result)){
        fprintf(stderr, "Error: Failed to listen to the socket!\n");
    }

}
void shutdown_network(){
    // graceful AF
    listening_for_clients = false;
    close(listenFd);
}