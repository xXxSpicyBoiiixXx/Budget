#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_CATEGORY_LENGTH 20
#define MAX_ENTRIES 100

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    float balance;
} User;

typedef struct {
    char category[MAX_CATEGORY_LENGTH];
    float amount;
    time_t date;
} Transaction;

int readLine(FILE *file, char *buffer, int bufferSize) {
    if (fgets(buffer, bufferSize, file) == NULL) {
        return 0;
    }

    int length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }

    return 1;
}

int findUser(const char *username, User *users, int numUsers) {
    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(username, users[i].username) == 0) {
            return i;
        }
    }
    return -1;
}

int authenticateUser(User *users, int numUsers) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);

    int userIndex = findUser(username, users, numUsers);

    if (userIndex != -1) {
        printf("Enter password: ");
        scanf("%s", password);

        if (strcmp(password, users[userIndex].password) == 0) {
            printf("Login successful!\n");
            return userIndex;
        } else {
            printf("Incorrect password. Login failed.\n");
        }
    } else {
        printf("User not found. Login failed.\n");
    }

    return -1;
}

void addIncome(float *balance) {
    float income;
    printf("Enter income: $");
    scanf("%f", &income);
    *balance += income;
    printf("Income added successfully!\n");
}

void addExpense(User *user, Transaction *transactions, int *numTransactions) {
    char category[MAX_CATEGORY_LENGTH];
    float expense;

    printf("Enter category for expense: ");
    scanf("%s", category);

    printf("Enter expense amount: $");
    scanf("%f", &expense);

    // Check if there is enough balance to cover the expense
    if (user->balance >= expense) {
        user->balance -= expense;

        // Record the expense in transactions
        strcpy(transactions[*numTransactions].category, category);
        transactions[*numTransactions].amount = expense;
        transactions[*numTransactions].date = time(NULL);

        (*numTransactions)++;

        printf("Expense added successfully!\n");
    } else {
        printf("Insufficient funds! Cannot add expense.\n");
    }
}

void displayBalance(User *user) {
    printf("Current Balance: $%.2f\n", user->balance);
}

void displayTransactions(Transaction *transactions, int numTransactions) {
    printf("\n--- Transaction History ---\n");

    if (numTransactions == 0) {
        printf("No transactions yet.\n");
    } else {
        printf("%-20s%-10s%-20s\n", "Date", "Category", "Amount");
        printf("---------------------------------------------\n");

        for (int i = 0; i < numTransactions; ++i) {
            printf("%-20s%-10s$%-20.2f\n", ctime(&transactions[i].date), transactions[i].category, transactions[i].amount);
        }
    }
}

int loadUsers(User *users) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numUsers = 0;

    while (readLine(file, users[numUsers].username, MAX_USERNAME_LENGTH) &&
            readLine(file, users[numUsers].password, MAX_PASSWORD_LENGTH)) {
        fscanf(file, "%f", &users[numUsers].balance);
        ++numUsers;
    }

    fclose(file);

    return numUsers;
}

void saveUsers(User *users, int numUsers) {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numUsers; ++i) {
        fprintf(file, "%s\n%s\n%.2f\n", users[i].username, users[i].password, users[i].balance);
    }

    fclose(file);
}

int loadTransactions(Transaction *transactions) {
    FILE *file = fopen("transactions.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numTransactions = 0;

    while (fscanf(file, "%s%f%ld", transactions[numTransactions].category,
                &transactions[numTransactions].amount, &transactions[numTransactions].date) == 3) {
        ++numTransactions;
    }

    fclose(file);

    return numTransactions;
}

void saveTransactions(Transaction *transactions, int numTransactions) {
    FILE *file = fopen("transactions.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numTransactions; ++i) {
        fprintf(file, "%s %.2f %ld\n", transactions[i].category,
                transactions[i].amount, transactions[i].date);
    }

    fclose(file);
}

int main() {
    User users[MAX_USERS];
    Transaction transactions[MAX_ENTRIES];
    int numUsers = loadUsers(users);
    int numTransactions = loadTransactions(transactions);

    int currentUserIndex = -1;

    do {
        printf("\n*** Budget Application ***\n");

        if (currentUserIndex == -1) {
            printf("1. Login\n");
        } else {
            printf("1. Logout\n");
            printf("2. Add Income\n");
            printf("3. Add Expense\n");
            printf("4. Display Balance\n");
            printf("5. View Transactions\n");
        }

        printf("6. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (currentUserIndex == -1) {
                    currentUserIndex = authenticateUser(users, numUsers);
                } else {
                    printf("Logging out...\n");
                    currentUserIndex = -1;
                }
                break;
            case 2:
                if (currentUserIndex != -1) {
                    addIncome(&users[currentUserIndex].balance);
                }  else {
                    printf("Please login first.\n");
                }
                break;
            case 3:
                if (currentUserIndex != -1) {
                    addExpense(&users[currentUserIndex], transactions, &numTransactions);
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 4:
                if (currentUserIndex != -1) {
                    displayBalance(&users[currentUserIndex]);
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 5:
                if (currentUserIndex != -1) {
                    displayTransactions(transactions, numTransactions);
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 6:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

    } while (currentUserIndex != -1);

    // Save user data and transactions before exiting
    saveUsers(users, numUsers);
    saveTransactions(transactions, numTransactions);

    return 0;
}
