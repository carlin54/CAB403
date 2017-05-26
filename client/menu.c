//
// Created by carlin on 10/24/16.
//
#include "menu.h"

int login(const char* username, int pin){

    if(is_connected()) {
        int login_status = -1;


        switch (write_username(username)) {
            case WRITE_SUCCESS:
                printf("Status: Write username (%s).\n", username);
                break;
            case WRITE_INCORRECT_SIZE:
                fprintf(stderr, "Error: Write username incorrectly (%s)!\n", username);
                break;
            case WRITE_DISCONNECTED:
                fprintf(stderr, "Error: Write server disconnected on username (%s)!\n", username);
                return LOGIN_COULD_NOT_WRITE;
        }

        switch (write_pin(pin)) {
            case WRITE_SUCCESS:
                printf("Status: Write pin (%d).\n", pin);
                break;
            case WRITE_INCORRECT_SIZE:
                fprintf(stderr, "Error: Write pin incorrectly (%d)!\n", pin);
                break;
            case WRITE_DISCONNECTED:
                fprintf(stderr, "Error: Write server disconnected on pin (%d)!\n", pin);
                return LOGIN_COULD_NOT_WRITE;
        }

        switch(read_login_status(&login_status)){
            case READ_SUCCESS:
                printf("Status: Read login status successfully (%d)!\n", login_status);
                return login_status;
            case READ_INCORRECT_SIZE:
                fprintf(stderr, "Error: Read login status incorrect size (%d)!\n", login_status);
                return login_status;
            case READ_UNSUCCESSFUL:
                fprintf(stderr, "Error: Read login status unsuccessfully (%d)!\n", login_status);
                return LOGIN_COULD_NOT_WRITE;
        }

    }else{
        fprintf(stderr, "Error: Cannot login because you are not connected!\n");
    }

}

int menu_account_balance(){
    switch(write_menu_account_balance()){
        case WRITE_SUCCESS:
            printf("Status: Write menu account balance!\n");
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write incorrect size account balance!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write incorrect size account balance!\n");
            return SERVER_DISCONNECTED;
    };

    switch(read_savings_account(&__Accounts__[ACCOUNT_INDEX_SAVINGS])){
        case READ_SUCCESS:
            printf("Status: Read savings account (%d, %f, %f)!\n",
                   __Accounts__[ACCOUNT_INDEX_SAVINGS].AccountNo,
                   __Accounts__[ACCOUNT_INDEX_SAVINGS].OpeningBal,
                   __Accounts__[ACCOUNT_INDEX_SAVINGS].ClosingBal);
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read savings account incorrect size !\n");
            fflush(stderr);
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read savings account disconnected!\n");
            return SERVER_DISCONNECTED;
    }

    switch(read_loan_account(&__Accounts__[ACCOUNT_INDEX_LOAN])){
        case READ_SUCCESS:
            printf("Status: Read loan account (%d, %f, %f)!\n",
                   __Accounts__[ACCOUNT_INDEX_LOAN].AccountNo,
                   __Accounts__[ACCOUNT_INDEX_LOAN].OpeningBal,
                   __Accounts__[ACCOUNT_INDEX_LOAN].ClosingBal);
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read loan account incorrect size !\n");
            fflush(stderr);
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read loan account disconnected!\n");
            return SERVER_DISCONNECTED;
    }

    switch(read_credit_account(&__Accounts__[ACCOUNT_INDEX_CREDIT])){
        case READ_SUCCESS:
            printf("Status: Read credit account (%d, %f, %f)!\n",
                   __Accounts__[ACCOUNT_INDEX_CREDIT].AccountNo,
                   __Accounts__[ACCOUNT_INDEX_CREDIT].OpeningBal,
                   __Accounts__[ACCOUNT_INDEX_CREDIT].ClosingBal);
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read credit account incorrect size !\n");
            fflush(stderr);
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read credit account disconnected!\n");
            return SERVER_DISCONNECTED;
    }

    int input = 0;
    while(input != -1){
        switch(write_return(false)){
            case WRITE_SUCCESS:
                printf("Status: Write return!\n");
                return SERVER_RETURNED;
            case WRITE_INCORRECT_SIZE:
                fprintf(stderr, "Error: Write return incorrect size!\n");
                fflush(stderr);
                return SERVER_RETURNED;
            case WRITE_DISCONNECTED:
                fprintf(stderr, "Error: Write disconnect!\n");
                return SERVER_DISCONNECTED;
        }

        printf("Select Account Type\n");
        print_available_accounts();
        print_empty_line();

        input = get_int("Enter your selection (E/e to exit) -> ");
        int index = get_index_from_selection(input);
        switch (input) {
            case ACCOUNT_INDEX_SAVINGS:
            case ACCOUNT_INDEX_LOAN:
            case ACCOUNT_INDEX_CREDIT:
                print_account(index);
        }
        switch(read_savings_account(&__Accounts__[ACCOUNT_INDEX_SAVINGS])){
            case READ_SUCCESS:
                printf("Status: Read savings account (%d, %f, %f)!\n",
                       __Accounts__[ACCOUNT_INDEX_SAVINGS].AccountNo,
                       __Accounts__[ACCOUNT_INDEX_SAVINGS].OpeningBal,
                       __Accounts__[ACCOUNT_INDEX_SAVINGS].ClosingBal);
                break;
            case READ_INCORRECT_SIZE:
                fprintf(stderr, "Error: Read savings account incorrect size !\n");
                fflush(stderr);
                break;
            case READ_UNSUCCESSFUL:
                fprintf(stderr, "Error: Read savings account disconnected!\n");
                return SERVER_DISCONNECTED;
        }

        switch(read_loan_account(&__Accounts__[ACCOUNT_INDEX_LOAN])){
            case READ_SUCCESS:
                printf("Status: Read loan account (%d, %f, %f)!\n",
                       __Accounts__[ACCOUNT_INDEX_LOAN].AccountNo,
                       __Accounts__[ACCOUNT_INDEX_LOAN].OpeningBal,
                       __Accounts__[ACCOUNT_INDEX_LOAN].ClosingBal);
                break;
            case READ_INCORRECT_SIZE:
                fprintf(stderr, "Error: Read loan account incorrect size !\n");
                fflush(stderr);
                break;
            case READ_UNSUCCESSFUL:
                fprintf(stderr, "Error: Read loan account disconnected!\n");
                return SERVER_DISCONNECTED;
        }

        switch(read_credit_account(&__Accounts__[ACCOUNT_INDEX_CREDIT])){
            case READ_SUCCESS:
                printf("Status: Read credit account (%d, %f, %f)!\n",
                       __Accounts__[ACCOUNT_INDEX_CREDIT].AccountNo,
                       __Accounts__[ACCOUNT_INDEX_CREDIT].OpeningBal,
                       __Accounts__[ACCOUNT_INDEX_CREDIT].ClosingBal);
                break;
            case READ_INCORRECT_SIZE:
                fprintf(stderr, "Error: Read credit account incorrect size !\n");
                fflush(stderr);
                break;
            case READ_UNSUCCESSFUL:
                fprintf(stderr, "Error: Read credit account disconnected!\n");
                return SERVER_DISCONNECTED;
        }
    }

    switch(write_return(true)){
        case WRITE_SUCCESS:
            printf("Status: Write return!\n");
            return SERVER_RETURNED;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write return incorrect size!\n");
            fflush(stderr);
            return SERVER_RETURNED;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write disconnect!\n");
            return SERVER_DISCONNECTED;
    }

    return SERVER_RETURNED;

}

int menu_withdrawal(){

    switch(write_menu_withdraw()){
        case WRITE_SUCCESS:
            printf("Status: Write menu withdrawal success.");
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write menu withdrawal incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write menu withdrawal disconnected!\n");
            return SERVER_DISCONNECTED;
    }

    switch(read_accounts(__Accounts__)){
        case READ_SUCCESS:
            printf("Status: Read accounts successfully!\n");
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read accounts incorrect size!\n");
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read accounts disconnected!\n");
            return SERVER_DISCONNECTED;
    }

    print_bar();
    print_empty_line();
    print_empty_line();
    printf("Select Account Type\n");
    print_available_accounts();
    print_bar();

    int account_type = get_selection("Selection Account Type (E/e to exit) - ");
    bool user_return = account_type == -1;
    switch(write_return(&user_return)){
        case WRITE_SUCCESS:
            printf("Status: Read return successfully!\n");
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read return incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Read return disconnected!\n");
            return SERVER_RETURNED;
    }

    if(user_return) return SERVER_RETURNED;

    account_type = get_index_from_selection(account_type);
    switch(write_account_index(account_type)){
        case WRITE_SUCCESS:
            printf("Status: Write account index successfully (%d)!\n", account_type);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write account index incorrect size (%d)!\n", account_type);
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write account index disconnected (%d)!\n", account_type);
            return SERVER_RETURNED;
    }

    float withdrawal_amount = get_float("Enter the amount to withdraw (E/e) : $");

    user_return = withdrawal_amount == -1;
    switch(write_return(&user_return)){
        case WRITE_SUCCESS:
            printf("Status: Write withdrawal successfully!\n");
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write return incorrect size!\n");
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write return disconnected!\n");
            return SERVER_RETURNED;
    }

    switch(write_amount(withdrawal_amount)){
        case WRITE_SUCCESS:
            printf("Status: Write withdrawal amount successfully (%f)!\n", withdrawal_amount);
            break;
        case WRITE_INCORRECT_SIZE:
            fprintf(stderr, "Error: Write withdrawal amount incorrect size (%f)!\n", withdrawal_amount);
            break;
        case WRITE_DISCONNECTED:
            fprintf(stderr, "Error: Write withdrawal amount disconnected (%f)!\n", withdrawal_amount);
            return SERVER_RETURNED;
    }

    bool successful_withdrawal = false;
    switch(read_successful_withdrawal(&successful_withdrawal)){
        case READ_SUCCESS:
            printf("Status: Read withdrawal status successfully!\n");
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read withdrawal status incorrect size!\n");
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read withdrawal status disconnected!\n");
            return SERVER_DISCONNECTED;
    }

    switch(read_accounts(__Accounts__)){
        case READ_SUCCESS:
            printf("Status: Read accounts successfully!\n");
            break;
        case READ_INCORRECT_SIZE:
            fprintf(stderr, "Error: Read accounts incorrect size!\n");
            break;
        case READ_UNSUCCESSFUL:
            fprintf(stderr, "Error: Read accounts disconnected!\n");
            return SERVER_DISCONNECTED;
    }

    printf("Withdrawal Completed: Closing Balance : $%f\n", __Accounts__[account_type].ClosingBal);
    print_empty_line();
    print_bar();


}

int menu_deposit(){

}

int menu_transfer(){

}

int menu_transaction_listing(){

}

void main_menu(){
    assert(is_connected());

    clear_screen();
    bool incorrect_input = false;
    int server_status;
    while(!exit_main_menu_screen && server_status != SERVER_DISCONNECTED){
        printf("Welcome to the ATM System\n");
        print_empty_line();
        printf("You are currently logged in as %s %s\n", __FirstName__, __LastName__);
        printf("Client Number - %d\n", __ClientNumber__);
        //while(!correct_input)
        print_empty_line();
        print_empty_line();
        if(incorrect_input){
            printf("Invalid selection. Please select option from menu!\n");
            incorrect_input = false;
        }
        print_empty_line();
        printf("Pleases enter a selection\n");
        printf("<%d> Account Balance\n", SELECTION_ACCOUNT_BALANCE);
        printf("<%d> Withdrawal\n", SELECTION_WITHDRAWAL);
        printf("<%d> Deposit\n", SELECTION_DEPOSIT);
        printf("<%d> Transfer\n", SELECTION_TRANSFER);
        printf("<%d> Transaction Listing\n", SELECTION_TRANSACTION_LISTING);
        printf("<%d> Exit\n", SELECTION_EXIT);
        print_empty_line();
        int input = get_int("Select option 1-6 ->");
        switch(input){
            case SELECTION_ACCOUNT_BALANCE:
                server_status = menu_account_balance();
                break;
            case SELECTION_WITHDRAWAL:
                server_status = menu_withdrawal();
                break;
            case SELECTION_DEPOSIT:
                server_status = menu_deposit();
                break;
            case SELECTION_TRANSFER:
                server_status = menu_transfer();
                break;
            case SELECTION_TRANSACTION_LISTING:
                server_status = menu_transaction_listing();
                break;
            case SELECTION_EXIT:
                exit_main_menu_screen = true;
                break;
            default:
                incorrect_input = true;
                break;
        }
    }
}

void login_screen() {

    clear_screen();
    char* username = NULL;
    int pin;

    print_bar();
    print_empty_line();
    printf("Welcome to the Online ATM System\n");
    print_empty_line();
    print_bar();
    print_empty_line();
    print_empty_line();
    printf("You are required to logon with your registered Username and PIN\n");
    print_empty_line();

    is_logged_in = false;
    bool failed_to_login = false;

    while(!is_logged_in && !failed_to_login) {


        username = get_string("Please enter your username -->", sizeof(__UserName__));

        pin = get_int("Please enter your pin -->");



        switch(login(username, pin)){
            case LOGIN_SUCCESS:
                memcpy(__UserName__, username, sizeof(__UserName__));
                free(username); username = NULL;
                __Pin__ = pin;
                printf("Status: Login success (%s : %d).\n", __UserName__, __Pin__);
                is_logged_in = true;
                break;
            case LOGIN_INCORRECT:
                fprintf(stderr, "Error: Login was incorrect (%s : %d)!\n", username, pin);
                return;
            case LOGIN_COULD_NOT_WRITE:
                return;
            case LOGIN_COULD_NOT_READ:
                return;

        }

        if(username != NULL && !is_logged_in){
            free(username);
            username = NULL;
        }else{
            switch(read_client_number(&__ClientNumber__)){
                case READ_SUCCESS:
                    printf("Status: Read client number successfully (%d)!\n", __ClientNumber__);
                    break;
                case READ_INCORRECT_SIZE:
                    fprintf(stderr, "Error: Read client number incorrect size (%d)!\n", __ClientNumber__);
                    break;
                case READ_UNSUCCESSFUL:
                    fprintf(stderr, "Error: Read client number unsuccessfully (%d)!\n", __ClientNumber__);
                    return;
            }

            switch(read_firstname(__FirstName__)){
                case READ_SUCCESS:
                    printf("Status: Read first name successfully (%s)!\n", __FirstName__);
                    break;
                case READ_INCORRECT_SIZE:
                    fprintf(stderr, "Error: Read first name incorrect size (%s)!\n", __FirstName__);
                    break;
                case READ_UNSUCCESSFUL:
                    fprintf(stderr, "Error: Read first name unsuccessfully (%s)!\n", __FirstName__);
                    return;
            }

            switch(read_lastname(__LastName__)){
                case READ_SUCCESS:
                    printf("Status: Read last name successfully (%s)!\n", __LastName__);
                    break;
                case READ_INCORRECT_SIZE:
                    fprintf(stderr, "Error: Read last name incorrect size (%s)!\n", __LastName__);
                    break;
                case READ_UNSUCCESSFUL:
                    fprintf(stderr, "Error: Read last name unsuccessfully (%s)!\n", __LastName__);
                    return;
            }
        }

    }

}
