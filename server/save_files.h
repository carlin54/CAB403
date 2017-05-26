//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_SERVER_SAVE_FILES_H
#define CAB403_SERVER_SAVE_FILES_H

#include "global.h"
#include "utility.h"
int save_accounts();
int save_clients();

int save_authentication();
int save_transactions();

void save_data(void);


#endif //CAB403_SAVE_FILES_H
