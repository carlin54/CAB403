//
// Created by carlin on 10/24/16.
//

#include "load_files.h"

int load_accounts(){

    int num_of_accounts_in_file = number_of_char_in_file('\n', file_account_location);

    FILE* file_accounts = fopen(file_account_location, "r");

    if(file_accounts == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    /* moves the file cursor past the column definition line */
    find_next_char('\n', file_accounts); //TODO:Error check here

    assert(is_list_empty(&accounts_list));

    printf("Loading Accounts (%d)...\n", num_of_accounts_in_file);
    struct Account temporary;
    for(int i = 0; i < num_of_accounts_in_file; i++){
        int loaded_correctly = fscanf(file_accounts,"%d %f %f",
                                      &temporary.AccountNo,
                                      &temporary.OpeningBal,
                                      &temporary.ClosingBal);
        if(loaded_correctly == NUM_COLUMNS_ACCOUNT){

            struct Account* pAccount = (struct Account*)malloc(sizeof(struct Account));
            assert(pAccount != NULL);
            pAccount->AccountNo = temporary.AccountNo;
            pAccount->OpeningBal = temporary.OpeningBal;
            pAccount->ClosingBal = temporary.ClosingBal;

            printf("Loaded Account (%d) of (%d) <- %d\t\t%f\t\t%f\n",
                   i+1, num_of_accounts_in_file,
                   pAccount->AccountNo,
                   pAccount->OpeningBal,
                   pAccount->ClosingBal
            );

            list_add_tail(&accounts_list, pAccount);
        } else
            fprintf(stderr, "Failed to Load Account (%d) of (%d)\n",
                    i+1, num_of_accounts_in_file);

    }

    fclose(file_accounts);



    return LOAD_SUCCESS;
}

int load_clients(){

    int num_of_clients_in_file = number_of_char_in_file('\n', file_client_location);
    FILE* file_clients = fopen(file_client_location, "r");

    if(file_clients == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    /* moves the file cursor past the column definition line */
    find_next_char('\n', file_clients);


    assert(is_list_empty(&clients_list));

    printf("Loading Clients (%d)...\n", num_of_clients_in_file);
    struct Client temporary;
    for(int i = 0; i < num_of_clients_in_file; i++){

        int loaded_correctly = fscanf(file_clients,"%15s %15s %d %d,%d,%d",
                                      temporary.FirstName,
                                      temporary.LastName,
                                      &temporary.ClientNo,
                                      &temporary.Accounts[0],
                                      &temporary.Accounts[1],
                                      &temporary.Accounts[2]);

        if(loaded_correctly > NUM_MIN_COLUMNS_CLIENT){
            for(int j = loaded_correctly; j < 6; j++){
                temporary.Accounts[j-3] = 0;
            }

            struct Client* pClient = (struct Client*)malloc(sizeof(struct Client));
            assert(pClient != NULL);
            memcpy(pClient->FirstName, temporary.FirstName, sizeof(pClient->FirstName));
            memcpy(pClient->LastName, temporary.LastName, sizeof(pClient->LastName));
            pClient->ClientNo = temporary.ClientNo;
            pClient->Accounts[0] = temporary.Accounts[0];
            pClient->Accounts[1] = temporary.Accounts[1];
            pClient->Accounts[2] = temporary.Accounts[2];
            list_add_tail(&clients_list, pClient);
            printf("Loaded Client (%d) of (%d) <- %s\t%s\t%d\t%d\t%d\t%d\n",
                   i+1, num_of_clients_in_file,
                   pClient->FirstName,
                   pClient->LastName,
                   pClient->ClientNo,
                   pClient->Accounts[0],
                   pClient->Accounts[1],
                   pClient->Accounts[2]);
        }else {
            fprintf(stderr, "Failed to Load Client (%d) of (%d)\n",
                    i + 1, num_of_clients_in_file);
        }
    }

    fclose(file_clients);

    return LOAD_SUCCESS;
}

int load_authentications(){

    int num_authentications_in_file = number_of_char_in_file('\n', file_authentication_location);
    FILE* file_authentications = fopen(file_authentication_location, "r");
    if(file_authentications == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    find_next_char('\n', file_authentications);

    printf("Loading Authentications (%d)...\n", num_authentications_in_file);
    struct Authentication temporary;
    for(int i = 0; i < num_authentications_in_file; i++) {
        int loaded_correctly = fscanf(file_authentications, "%12s %d %d",
                                      temporary.Username,
                                      &temporary.Pin,
                                      &temporary.ClientNo);
        if (loaded_correctly == NUM_COLUMNS_AUTHENTICATION){
            struct Authentication* pAuthentication = (struct Authentication*)malloc(sizeof(struct Authentication));
            memcpy(pAuthentication->Username, temporary.Username, sizeof(pAuthentication->Username));
            pAuthentication->Pin = temporary.Pin;
            pAuthentication->ClientNo = temporary.ClientNo;
            printf("Loaded Authentication (%d) of (%d) <- \t%s\t%d\t%d\n",
                   i+1, num_authentications_in_file,
                   pAuthentication->Username,
                   pAuthentication->Pin,
                   pAuthentication->ClientNo);
            list_add_tail(&authentications_list, pAuthentication);
        } else
            fprintf(stderr, "Failed to Load Authentication (%d) of (%d)\n",
                    i+1, num_authentications_in_file);

    }


    fclose(file_authentications);

    return LOAD_SUCCESS;
}

int load_transactions(){

    int num_transactions_in_file = number_of_char_in_file('\n', file_transactions_location);
    FILE* file_transactions = fopen(file_transactions_location, "r");
    if(file_transactions == NULL) {
        return LOAD_ERROR_COULD_NOT_LOAD_FILE;
    }

    /* moves the file cursor past the column definition line */
    find_next_char('\n', file_transactions);

    printf("Loading Transactions (%d)...\n", num_transactions_in_file);


    struct Transaction temporary;
    for(int i = 0; i < num_transactions_in_file; i++) {

        int loaded_correctly = fscanf(file_transactions, "%d %d %d %f",
                                      &temporary.FromAccount,
                                      &temporary.ToAccount,
                                      &temporary.TranType,
                                      &temporary.Amount);
        if (loaded_correctly == NUM_COLUMNS_TRANSACTION){
            struct Transaction* pTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
            assert(pTransaction != NULL);
            pTransaction->FromAccount = temporary.FromAccount;
            pTransaction->ToAccount = temporary.ToAccount;
            pTransaction->TranType = temporary.TranType;
            pTransaction->Amount = temporary.Amount;
            printf("Loaded Transaction (%d) of (%d) <- \t%d\t%d\t%d\t%f\n",
                   i+1, num_transactions_in_file,
                   pTransaction->FromAccount,
                   pTransaction->ToAccount,
                   pTransaction->TranType,
                   pTransaction->Amount);
            list_add_tail(&transactions_list, pTransaction);
        } else
            fprintf(stderr, "Failed to Load Transactions (%d) of (%d)\n",
                    i+1, num_transactions_in_file);

    }

    fclose(file_transactions);


    return LOAD_SUCCESS;
}

void load_data(void){

    int load_account_result = load_accounts();

    int load_clients_result = load_clients();

    int load_authentications_result = load_authentications();

    int load_transactions_result = load_transactions();

}
