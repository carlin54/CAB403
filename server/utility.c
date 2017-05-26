//
// Created by carlin on 10/24/16.
//

#include "utility.h"

bool are_equal(const char *s1, const char *s2) {
    if(strlen(s1) != strlen(s2)) return false;
    for(int i = 0; i < strlen(s1); i++){
        if(s1[i] != s2[i]) return false;
    }
    return true;
}
bool isNegative(int val){return val < 0;};
bool isZero(int val){return val == 0;};
int number_of_char_in_file(const char c, const char* file_location){
    FILE* file = fopen(file_location, "r");
    if(file == NULL) return -1;
    int num_char = 0;
    char ch;
    //printf("-------%s-------\n",file_location);
    while(!feof(file)){
        ch = (char)fgetc(file);
        //printf("%c", ch);
        if(c == ch){
            num_char++;
        }
    }
    //printf("\n-------%s-------\n",file_location);
    fclose(file);
    return num_char;
}
void int_to_char(int val, char* str, int size){
    for(int i = 0; i < size; i++) str[i] = ' ';
    sprintf(str,"%d", val);
}
int find_next_char(char c, FILE* file){
    char ch;
    while(!feof(file)){
        ch = (char)fgetc(file);
        if(ch == c){
            return FIND_NEXT_CHAR_SUCCESS;
        }
    }
    return FIND_NEXT_CHAR_EOF;
}
int find_next_char_n(char c, int n, FILE* file){
    if(file == NULL) return -1;
    int result = 0;
    for(int i = 0; i < n; i++){
        result = find_next_char(c, file);
        if(result == FIND_NEXT_CHAR_EOF){
            return FIND_NEXT_CHAR_EOF;
        }
    }
    return FIND_NEXT_CHAR_SUCCESS;
}


bool Authenticate(struct Authentication* auth){

    struct Link* pLink = authentications_list.pHead;
    while(pLink != NULL){
        struct Authentication* pAuthData = pLink->pData;
        if(are_equal(pAuthData->Username, auth->Username)){
            if(pAuthData->Pin == auth->Pin){
                auth->ClientNo = pAuthData->ClientNo;
                return true;
            }else{
                return false;
            }

        }
        pLink = pLink->pNext;
    }
    return false;
}

int clean_up_dynamic_memory(){
    while(!is_list_empty(&accounts_list)) list_remove_head(&accounts_list);
    while(!is_list_empty(&clients_list)) list_remove_head(&clients_list);
    while(!is_list_empty(&authentications_list)) list_remove_head(&authentications_list);
    while(!is_list_empty(&transactions_list)) list_remove_head(&transactions_list);
}

struct Account* find_accounts(int clientNo){
    struct Account* found_accounts = calloc(3, sizeof(struct Account));

    struct Link* pSearchClient = clients_list.pHead;
    while(pSearchClient != NULL){
        struct Client* pClient = pSearchClient->pData;
        if(pClient->ClientNo == clientNo){
            for(int i = 0; i < 3; i++){
                found_accounts[i].AccountNo = pClient->Accounts[i];
                printf("Status: Found client (%d)'s account (%d) with account number (%d).\n",
                       pClient->ClientNo, i, found_accounts[i].AccountNo);
            }
            break;
        }
        pSearchClient = pSearchClient->pNext;
    }

    struct Link* pSearchAccount = accounts_list.pHead;
    while(pSearchAccount != NULL){
        struct Account* pAccount = pSearchAccount->pData;
        for(int i = 0; i < 3; i++){
            if(found_accounts[i].AccountNo == pAccount->AccountNo){
                found_accounts[i].OpeningBal = pAccount->OpeningBal;
                found_accounts[i].ClosingBal = pAccount->ClosingBal;
                printf("Status: Found openingbal, closingbal ($%f, $%f) for account (%d).\n",
                       pAccount->OpeningBal, pAccount->ClosingBal,
                       pAccount->AccountNo);
                break;
            }
        }
        pSearchAccount = pSearchAccount->pNext;
    }

    return found_accounts;
}

bool can_withdrawal(struct Account* account, int account_index, float amount){
    switch(account_index){
        case ACCOUNT_INDEX_SAVINGS:
            return account->ClosingBal - amount >= 0.0f;
        case ACCOUNT_INDEX_LOAN:
            return account->ClosingBal - amount >= -5000.0f;
        case ACCOUNT_INDEX_CREDIT:
            return account->ClosingBal - amount >= 0.0f;
    }

}
void withdraw(struct Account* account, float amount){
    account->ClosingBal -= amount;
}
char* find_firstname(int clientNo){
    struct Link* pCheck = clients_list.pHead;
    struct Client* pData = pCheck->pData;
    while(pCheck != NULL){

        if(clientNo == pData->ClientNo){
            printf("Found: first name (%s) for client (%d).\n", pData->FirstName, pData->ClientNo);
            return pData->FirstName;
        }
        pCheck = pCheck->pNext;
    }
    fprintf(stderr, "Error: find_firstname(int clientNo) -> could not find match!\n");
    return NULL;
}
char* find_lastname(int clientNo){
    struct Link* pCheck = clients_list.pHead;
    struct Client* pData = pCheck->pData;
    while(pCheck != NULL){
        if(clientNo == pData->ClientNo){
            printf("Found: last name (%s) for client (%d).\n", pData->LastName, pData->ClientNo);
            return pData->LastName;
        }
        pCheck = pCheck->pNext;
    }
    fprintf(stderr, "Error: find_lastname(int clientNo) -> could not find match!\n");
    return NULL;
}

