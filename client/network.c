//
// Created by carlin on 10/24/16.
//
#include "network.h"

bool is_connected(){
    return connected_to_server;
}

bool disconnect(){
    if(is_connected()){
        shutdown(sockFd, 2);
        close(sockFd);
        connected_to_server = false;
        connectFd = -1;
        sockFd = -1;
    }
}

bool connect_to_server(char* serverName, int port_number){
    bool dont_connect_to_server = false;

    while(!connected_to_server && !dont_connect_to_server) {

        struct sockaddr_in serverAddr;
        struct sockaddr *serverAddrCast = (struct sockaddr *) &serverAddr;
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (isNegative(sockFd)) {
            fprintf(stderr, "Error: Failed to connect to the socket\n");
        }

        bzero(&serverAddr, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        inet_pton(AF_INET, serverName, &serverAddr.sin_addr);
        serverAddr.sin_port = (in_port_t) port_number;

        connectFd = connect(sockFd, serverAddrCast, sizeof(serverAddr));
        if (!isZero(connectFd)) {
            fprintf(stderr, "Error: Failed to connect to the server\n");
            connected_to_server = false;
        } else {
            printf("Status: Server connection established.\n");
        }

        int server_status = 0;
        switch(read_server_status(&server_status)){
            case READ_SUCCESS:

            case READ_INCORRECT_SIZE:

                switch(server_status){
                    case SERVER_STATUS_SERVER_NOT_FULL:
                        connected_to_server = true; /*Redundant*/
                        printf("Status: Read server not full, staying connected.\n");
                        break;
                    case SERVER_STATUS_SERVER_FULL:
                        fprintf(stderr, "Error: Read server full, disconnecting!\n");
                        dont_connect_to_server = true;
                        disconnect();
                        break;
                }

                break;
            case READ_UNSUCCESSFUL:
                fprintf(stderr, "Error: Reading server status, disconnecting!\n");
                dont_connect_to_server = true;
                disconnect();
                break;

        }
    }
}
