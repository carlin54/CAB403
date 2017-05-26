//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_SERVER_LOAD_FILES_H
#define CAB403_SERVER_LOAD_FILES_H

#include "global.h"
#include "utility.h"

int load_accounts();
int load_clients();

int load_authentications();
int load_transactions();

void load_data(void);


#endif //CAB403_LOAD_FILES_H
