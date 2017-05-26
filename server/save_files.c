//
// Created by carlin on 10/24/16.
//

#include "save_files.h"

int save_accounts(){
    FILE* file_accounts = fopen(file_account_location, "w");
    if(file_accounts == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Loading Accounts (%d)...\n", accounts_list.length);

    fprintf(file_accounts, "AccountNo      OpeningBal     ClosingBal     ");
    struct Link* pSave = accounts_list.pHead;
    int i = 0;
    while(pSave != NULL){

        fprintf(file_accounts, "\n%-14d %-14.2f %0.2f",
                ((struct Account*)(pSave->pData))->AccountNo,
                ((struct Account*)(pSave->pData))->OpeningBal,
                ((struct Account*)(pSave->pData))->ClosingBal
        );
        printf("Saved Account (%d) of (%d) <- %d\t\t%f\t\t%f\n",
               i+1, accounts_list.length,
               ((struct Account*)(pSave->pData))->AccountNo,
               ((struct Account*)(pSave->pData))->OpeningBal,
               ((struct Account*)(pSave->pData))->ClosingBal
        );

        pSave = pSave->pNext;
        i++;
    }
    fclose(file_accounts);

    return SAVE_SUCCESS;
}
int save_clients() {
    FILE *file_clients = fopen(file_client_location, "w");
    if (file_clients == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Saving Clients (%d)...\n", clients_list.length);

    char client_number[11];
    fprintf(file_clients, "Firstname      Lastname       ClientNo           Accounts");
    struct Link *pSave = clients_list.pHead;
    int i = 0;
    while (pSave != NULL) {
        int_to_char(((struct Client*)(pSave->pData))->ClientNo, client_number, 11);
        fprintf(file_clients, "\n%-14s %-14s %-18s %d",
                ((struct Client*)(pSave->pData))->FirstName,
                ((struct Client*)(pSave->pData))->LastName,
                client_number,
                ((struct Client*)(pSave->pData))->Accounts[0]
        );
        if (((struct Client*)(pSave->pData))->Accounts[1] != 0) {
            fprintf(file_clients, ",%d", ((struct Client*)(pSave->pData))->Accounts[1]);
        }
        if (((struct Client*)(pSave->pData))->Accounts[2] != 0) {
            fprintf(file_clients, ",%d", ((struct Client*)(pSave->pData))->Accounts[2]);
        }

        printf("Saved Client (%d) of (%d) <- %s\t%s\t%d\t%d\t%d\t%d\n",
               i+1, clients_list.length,
               ((struct Client*)(pSave->pData))->FirstName,
               ((struct Client*)(pSave->pData))->LastName,
               ((struct Client*)(pSave->pData))->ClientNo,
               ((struct Client*)(pSave->pData))->Accounts[0],
               ((struct Client*)(pSave->pData))->Accounts[1],
               ((struct Client*)(pSave->pData))->Accounts[2]);
        pSave = pSave->pNext;
        i++;
    }

    fclose(file_clients);

    return SAVE_SUCCESS;
}

int save_authentication(){

    FILE* file_authentication = fopen(file_authentication_location, "w");
    if(file_authentication == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Saving Authentications (%d)...\n", authentications_list.length);

    fprintf(file_authentication, "Username        PIN         ClientNo");
    struct Link* pSave = authentications_list.pHead;
    int i = 0;
    while(pSave != NULL){

        fprintf(file_authentication, "\n%-15s %-11d %d",
                ((struct Authentication*)(pSave->pData))->Username,
                ((struct Authentication*)(pSave->pData))->Pin,
                ((struct Authentication*)(pSave->pData))->ClientNo
        );
        printf("Saved Authentication (%d) of (%d) <- %s\t%d\t%d\n",
               i+1, authentications_list.length,
               ((struct Authentication*)(pSave->pData))->Username,
               ((struct Authentication*)(pSave->pData))->Pin,
               ((struct Authentication*)(pSave->pData))->ClientNo
        );

        pSave = pSave->pNext;
        i++;
    }
    fclose(file_authentication);

    return SAVE_SUCCESS;
}

int save_transactions(){
    FILE* file_transactions = fopen(file_transactions_location, "w");
    if(file_transactions == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    printf("Saving Transactions (%d)...\n", transactions_list.length);

    fprintf(file_transactions, "FromAccount      ToAccount       TranType    Amount");
    struct Link* pSave = transactions_list.pHead;
    int i = 0;
    while(pSave != NULL){

        fprintf(file_transactions, "\n%-14d %-14d %-14d %0.2f",
                ((struct Transaction*)(pSave->pData))->FromAccount,
                ((struct Transaction*)(pSave->pData))->ToAccount,
                ((struct Transaction*)(pSave->pData))->TranType,
                ((struct Transaction*)(pSave->pData))->Amount
        );
        printf("Saved Transaction (%d) of (%d) <- %d\t\t%d\t\t%d\t%f\n",
               i+1, accounts_list.length,
               ((struct Transaction*)(pSave->pData))->FromAccount,
               ((struct Transaction*)(pSave->pData))->ToAccount,
               ((struct Transaction*)(pSave->pData))->TranType,
               ((struct Transaction*)(pSave->pData))->Amount
        );

        pSave = pSave->pNext;
        i++;
    }
    fclose(file_transactions);

    return SAVE_SUCCESS;
}

void save_data(void){
    save_accounts();
    save_clients();
    save_authentication();
    save_transactions();
}

