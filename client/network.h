//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_CLIENT_NETWORK_H_H
#define CAB403_CLIENT_NETWORK_H_H

#include "global.h"
#include "write.h"
#include "read.h"
#include "utility.h"

bool is_connected();

bool disconnect();

bool connect_to_server(char* serverName, int port_number);

#endif //CAB403_NETWORK_H_H
