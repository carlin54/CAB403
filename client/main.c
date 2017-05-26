//
// Created by Richard on 27/09/2016.
//

#include "global.h"
#include "read.h"
#include "write.h"
#include "menu.h"
#include "network.h"

int main(int argc, char* argv[]) {

    if(argc > REQUIRED_NUMBER_OF_ARGC || argc < REQUIRED_NUMBER_OF_ARGC){
        printf("Error: Usage ./Client [ip address] [port number]\n");
        exit(EXIT_FAILURE);
    }else{
        sscanf(argv[PORT_NUMBER_ARGV_INDEX], "%d", &port_number);
    }

    connect_to_server(argv[IP_ADDRESS_ARGV_INDEX], port_number);

    if(is_connected()){
        login_screen();
        if(is_logged_in){
            main_menu();
        }else{
            fprintf(stderr, "Error: failed to login disconnecting!\n");
        }
        disconnect();
    }


    return EXIT_SUCCESS;
}