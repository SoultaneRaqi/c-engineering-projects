#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Needed for exit() if file operations fail critically

// --- 1. DATA STRUCTURES ---

struct Stock {
    char ticker[5];
    float price;
    int quantity_owned;
};

struct Account {
    int id;
    char name[50];
    float balance;
    struct Stock portfolio[3]; // Array of 3 stocks embedded inside the Account
};

// --- 2. FILE I/O (PERSISTENCE) ---

// Save account and portfolio data to disk
void save_data(struct Account *acc) {
    FILE *file = fopen("bank_data.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file to save!\n");
        return;
    }

    // Write account details
    fprintf(file, "%d %s %.2f\n", acc->id, acc->name, acc->balance);

    // Write stock portfolio details loop
    for (int i = 0; i < 3; i++) {
        fprintf(file, "%s %.2f %d\n", acc->portfolio[i].ticker, acc->portfolio[i].price, acc->portfolio[i].quantity_owned);
    }

    fclose(file);
    printf("Data saved to disk successfully.\n");
}

// Load account and portfolio data. Returns 1 if success, 0 if new user.
int load_data(struct Account *acc) {
    FILE *file = fopen("bank_data.txt", "r");
    if (file == NULL) return 0; // File doesn't exist yet

    // Read account details
    fscanf(file, "%d %s %f", &acc->id, acc->name, &acc->balance);

    // Read stock portfolio details loop
    for (int i = 0; i < 3; i++) {
        fscanf(file, "%s %f %d", acc->portfolio[i].ticker, &acc->portfolio[i].price, &acc->portfolio[i].quantity_owned);
    }

    fclose(file);
    return 1;
}

// --- 3. BUSINESS LOGIC ---

// Helper to clear the input buffer to prevent infinite loops on bad input
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Update balance via pointer
void deposit(struct Account *acc, float amount) {
    if (amount <= 0) {
        printf("Amount must be positive.\n");
        return;
    }
    acc->balance += amount;
    printf("\n--- Deposit Successful ---\n");
    printf("Added: $%.2f. New Balance: $%.2f\n", amount, acc->balance);
}

// Process stock purchase via pointer
void buy_stock(struct Account *acc) {
    printf("\n--- Available Stocks ---\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s - $%.2f per share (You own: %d)\n",
               i+1, acc->portfolio[i].ticker, acc->portfolio[i].price, acc->portfolio[i].quantity_owned);
    }

    int choice;
    printf("Select stock to buy (1-3) or 0 to cancel: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_buffer();
        return;
    }

    if (choice == 0) return;
    if (choice < 1 || choice > 3) {
        printf("Invalid selection.\n");
        return;
    }

    int stock_index = choice - 1; // Array indexes start at 0
    int qty;
    printf("How many shares of %s do you want to buy? ", acc->portfolio[stock_index].ticker);

    if (scanf("%d", &qty) != 1 || qty <= 0) {
        printf("Invalid quantity.\n");
        clear_buffer();
        return;
    }

    float total_cost = qty * acc->portfolio[stock_index].price;

    if (acc->balance >= total_cost) {
        acc->balance -= total_cost; // Deduct money
        acc->portfolio[stock_index].quantity_owned += qty; // Add shares
        printf("Successfully bought %d shares of %s for $%.2f.\n", qty, acc->portfolio[stock_index].ticker, total_cost);
    } else {
        printf("Insufficient funds. Cost is $%.2f, you have $%.2f.\n", total_cost, acc->balance);
    }
}

// --- 4. MAIN PROGRAM LOOP ---

int main() {
    struct Account my_account;

    // Initialize or load data
    if (load_data(&my_account)) {
        printf("Data loaded! Welcome back, %s.\n", my_account.name);
    } else {
        printf("No existing data found. Initializing new profile...\n");
        my_account.id = 101;
        strcpy(my_account.name, "Soultane");
        my_account.balance = 500.50;

        // Initialize default market
        strcpy(my_account.portfolio[0].ticker, "AAPL");
        my_account.portfolio[0].price = 150.00;
        my_account.portfolio[0].quantity_owned = 0;

        strcpy(my_account.portfolio[1].ticker, "TSLA");
        my_account.portfolio[1].price = 200.50;
        my_account.portfolio[1].quantity_owned = 0;

        strcpy(my_account.portfolio[2].ticker, "GOOG");
        my_account.portfolio[2].price = 100.25;
        my_account.portfolio[2].quantity_owned = 0;
    }

    int choice = 0;
    float amount = 0.0;

    while (1) {
        printf("\n========================\n");
        printf("1. Check Balance & Portfolio\n");
        printf("2. Deposit Funds\n");
        printf("3. Buy Stocks\n");
        printf("4. Exit & Save\n");
        printf("========================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please type a number.\n");
            clear_buffer();
            continue;
        }

        if (choice == 1) {
            printf("\nCurrent Balance: $%.2f\n", my_account.balance);
            printf("--- Your Portfolio ---\n");
            for(int i=0; i<3; i++){
                if(my_account.portfolio[i].quantity_owned > 0) {
                     printf("%s: %d shares (Value: $%.2f)\n",
                        my_account.portfolio[i].ticker,
                        my_account.portfolio[i].quantity_owned,
                        my_account.portfolio[i].quantity_owned * my_account.portfolio[i].price);
                }
            }
        }
        else if (choice == 2) {
            printf("Enter amount to deposit: ");
            if (scanf("%f", &amount) != 1) {
                printf("Invalid amount.\n");
                clear_buffer();
                continue;
            }
            deposit(&my_account, amount);
        }
        else if (choice == 3) {
            buy_stock(&my_account);
        }
        else if (choice == 4) {
            save_data(&my_account);
            printf("Thank you for banking with us. Goodbye!\n");
            break;
        }
        else {
            printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
        }
    }

    return 0;
}