//
// Created by carlin on 10/24/16.
//

#ifndef CAB403_SERVER_UTILITY_H
#define CAB403_SERVER_UTILITY_H

#include "global.h"

bool are_equal(const char *s1, const char *s2);
bool isNegative(int val);
bool isZero(int val);
int number_of_char_in_file(const char c, const char* file_location);
void int_to_char(int val, char* str, int size);
int find_next_char(char c, FILE* file);
int find_next_char_n(char c, int n, FILE* file);


bool Authenticate(struct Authentication* auth);

int clean_up_dynamic_memory();

struct Account* find_accounts(int clientNo);

char* find_firstname(int clientNo);

char* find_lastname(int clientNo);

bool can_withdrawal(struct Account* account, int account_index, float amount);

void withdraw(struct Account* account, float amount);



#endif //CAB403_UTILITY_H
