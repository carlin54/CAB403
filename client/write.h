//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_CLIENT_WRITE_H
#define CAB403_CLIENT_WRITE_H

#include "global.h"

#define WRITE_SUCCESS            0
#define WRITE_INCORRECT_SIZE     1
#define WRITE_DISCONNECTED       2

bool write_status(int written, int expected);

int write_menu_account_balance();
int write_menu_withdraw();
int write_menu_deposit();
int write_menu_transfer();
int write_menu_transactions();
int write_menu_exit();
int write_return(bool user_return);
int write_account_index(int account_index);
int write_amount(float amount);
int write_username(const char* username);
int write_pin(int pin);


#endif //CAB403_WRITE_H
