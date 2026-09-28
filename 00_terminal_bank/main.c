#include <stdio.h>
#include <string.h>

struct Account {
    int id;
    char name[50];
    float balance;
};

void deposit(struct Account *acc, float amount) {
    acc->balance += amount;
    printf("\n--- Deposit Successful ---\n");
    printf("Added: $%.2f\n", amount);
}

int main() {
    struct Account my_account;
    my_account.id = 101;
    my_account.balance = 500.50;
    strcpy(my_account.name, "Soultane");

    int choice = 0;
    float amount = 0.0;

    printf("Welcome to Terminal Bank, %s!\n", my_account.name);

    // while (1) creates an infinite loop. The program runs forever until we hit 'break'.
    while (1) {
        printf("\n========================\n");
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Exit\n");
        printf("========================\n");
        printf("Enter your choice: ");

        // scanf reads an integer (%d) and stores it at the memory address of 'choice'
        scanf("%d", &choice);

        if (choice == 1) {
            printf("\nCurrent Balance: $%.2f\n", my_account.balance);
        }
        else if (choice == 2) {
            printf("Enter amount to deposit: ");
            // %f reads a float. We pass the address of 'amount'.
            scanf("%f", &amount);
            deposit(&my_account, amount);
        }
        else if (choice == 3) {
            printf("Thank you for banking with us. Goodbye!\n");
            break; // This immediately stops the while loop
        }
        else {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    }

    return 0;
}