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

// We use the pointer to Account so we don't waste memory copying it
void save_account(struct Account *acc) {
    // 1. Open a file called account.txt.
    // "w" means "Write" mode. (If the file exists, it overwrites it. If not, it creates it).
    FILE *file = fopen("account.txt", "w");

    // Always check if the OS allowed us to open the file (e.g., permission errors)
    if (file == NULL) {
        printf("Error: Could not open file to save!\n");
        return;
    }

    // 2. Write to the file. fprintf works exactly like printf, but needs the file pointer first.
    // We save data separated by spaces so it's easy to read back later.
    fprintf(file, "%d %s %.2f\n", acc->id, acc->name, acc->balance);

    // 3. CRITICAL: Always close the file. If you don't, the data might stay trapped in the OS cache and not write to the drive.
    fclose(file);
    printf("Account saved successfully.\n");
}


// Returns 1 if successful, 0 if no file was found
int load_account(struct Account *acc) {
    // "r" means "Read" mode.
    FILE *file = fopen("account.txt", "r");

    if (file == NULL) {
        // No account exists yet.
        return 0;
    }

    // fscanf reads data FROM the file.
    // Notice we use '&' for id and balance because it's exactly like scanf!
    // (Strings like 'name' don't need '&' because arrays act as pointers automatically in C).
    fscanf(file, "%d %s %f", &acc->id, acc->name, &acc->balance);

    fclose(file);
    return 1;
}

int main() {
    struct Account my_account;

    // 1. Try to load the account from the hard drive
    if (load_account(&my_account)) {
        printf("Account loaded from disk! Welcome back, %s.\n", my_account.name);
    } else {
        // 2. If it fails (first time running), create a new one
        printf("No existing account found. Creating new profile...\n");
        my_account.id = 101;
        strcpy(my_account.name, "Soultane");
        my_account.balance = 500.50;
    }

    int choice = 0;
    float amount = 0.0;

    while (1) {
        printf("\n========================\n");
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Exit (Save Data)\n");
        printf("========================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        if (choice == 1) {
            printf("\nCurrent Balance: $%.2f\n", my_account.balance);
        }
        else if (choice == 2) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            deposit(&my_account, amount);
        }
        else if (choice == 3) {
            // 3. Save to disk right before the program exits
            save_account(&my_account);
            printf("Thank you for banking with us. Goodbye!\n");
            break;
        }
        else {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    }

    return 0;
}