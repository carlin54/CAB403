//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_CLIENT_UTILITY_H
#define CAB403_CLIENT_UTILITY_H

#include "global.h"

bool isNegative(int val);
bool isZero(int val);
bool isYes(char input);
bool isNo(char input);

void print_bar();

void print_empty_line();

void print_account(int acc_index);

void print_available_accounts();

void clear_screen();

char* get_string(char* arrow, int max);

int get_int(char* arrow);

int get_selection(char* arrow);

float get_float(char* arrow);

int get_index_from_selection(int acc_index);

char* int_to_char(int val);



#endif //CAB403_UTILITY_H
