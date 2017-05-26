//
// Created by carlin on 10/24/16.
//

#include "utility.h"

bool isNegative(int val){return val < 0;};
bool isZero(int val)    {return val == 0;};
bool isYes(char input){return input == 'Y' || input == 'y';}
bool isNo(char input){return input == 'n' || input == 'n';}

const char* bar = "=================================================================";
void print_bar(){
    printf("%s\n", bar);
}

void print_empty_line(){
    printf("\n");
}

void clear_screen(){

}

char* get_string(char* arrow, int max){
    printf("%s", arrow);

    char* str = (char*)malloc(sizeof(char) * max);
    for(int i = 0; i < max; i++) str[i] = '\0';
    fflush(stdout);
    fgets (str, max, stdin);
    for(int i = 0; i < max; i++) if(str[i] == '\n') str[i] = '\0';
    return str;
}

int get_int(char* arrow){
    printf("%s", arrow);
    fflush(stdout);
    int input;
    scanf ("%d",&input);
    if(input == 'E' || input == 'e') return -1;
    return input;
}

int get_selection(char* arrow){
    printf("%s", arrow);
    fflush(stdout);
    char input = getchar();
    if(input == 'E' || input == 'e') return -1;
    return input - '0';
}

float get_float(char* arrow){
    printf("%s", arrow);
    fflush(stdout);
    float money;
    scanf("%f", &money);
    if(money == 'E' || money == 'e') return -1;
    return money;
}

int get_index_from_selection(int acc_index){
    int i = 0;
    for(; i != acc_index;){
        if(__Accounts__[i].AccountNo != 0) i++;
        if(i > 3){
            fprintf(stderr,"Error: acc_index > 3 returning 0\n");
            return 0;
        }

    }
    return i-1;
}

char* int_to_char(int val){
    char* str = malloc(sizeof(char)*12);
    for(int i = 0; i < 12; i++) str[i] = '\0';
    sprintf(str,"%ld", val);
    return str;
}

void print_account(int acc_index){
    print_bar();
    print_empty_line();
    printf("Account Name - %s %s", __FirstName__, __LastName__);
    print_empty_line();
    printf("Current Balance for Account %d : $%f\n", __Accounts__[acc_index].AccountNo, __Accounts__[acc_index].ClosingBal);
    print_empty_line();
    print_bar();
}

void print_available_accounts(){
    __NumberOfAccountsAvailable__ = 0;
    for(int i = 0; i < 3; i++){
        if(__Accounts__[i].AccountNo != 0){
            __NumberOfAccountsAvailable__++;
        }
    }
    for(int i = 1; i <= __NumberOfAccountsAvailable__; i++){
        switch(i){
            case 1:
                printf("%d. Savings Account (%d, $%f, $%f)\n", i,
                       __Accounts__[ACCOUNT_INDEX_SAVINGS].AccountNo,
                       __Accounts__[ACCOUNT_INDEX_SAVINGS].OpeningBal,
                       __Accounts__[ACCOUNT_INDEX_SAVINGS].ClosingBal);
                break;
            case 2:
                printf("%d. Loan Account (%d, $%f, $%f)\n", i,
                       __Accounts__[ACCOUNT_INDEX_LOAN].AccountNo,
                       __Accounts__[ACCOUNT_INDEX_LOAN].OpeningBal,
                       __Accounts__[ACCOUNT_INDEX_LOAN].ClosingBal);
                break;
            case 3:
                printf("%d. Credit Account  (%d, $%f, $%f)\n", i,
                       __Accounts__[ACCOUNT_INDEX_CREDIT].AccountNo,
                       __Accounts__[ACCOUNT_INDEX_CREDIT].OpeningBal,
                       __Accounts__[ACCOUNT_INDEX_CREDIT].ClosingBal);
                break;
        }
    }
}

