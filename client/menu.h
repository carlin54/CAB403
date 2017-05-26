//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_CLIENT_MENU_H
#define CAB403_CLIENT_MENU_H

#include "global.h"
#include "read.h"
#include "write.h"
#include "utility.h"
#include "network.h"

int login(const char* username, int pin);

void login_screen();

int menu_account_balance();

int menu_withdrawal();

int menu_deposit();

int menu_transfer();

int menu_transaction_listing();

void main_menu();

#endif //CAB403_MENU_H
