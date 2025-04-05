#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store account details
struct Account {
    int accountNumber;
    char name[50];
    float balance;
};

// Function prototypes
void createAccount();
void viewAccounts();
void depositMoney();
void withdrawMoney();
void deleteAccount();

int main() {
    int choice;
    
    while (1) {
        printf("\n===== Bank Management System =====\n");
        printf("1. Create Account\n");
        printf("2. View Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
	printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccounts(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: deleteAccount(); break; 
	    case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    
    return 0;
}

// Function to create a new account
void createAccount() {
    FILE *fp = fopen("accounts.txt", "ab"); 
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    struct Account acc;
    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accountNumber);
    printf("Enter Name: ");
    scanf(" %[^\n]", acc.name);
    acc.balance = 0; // New account starts with zero balance
    
    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);
    
    printf("Account created successfully!\n");
}

// Function to view all accounts
void viewAccounts() {
    FILE *fp = fopen("accounts.txt", "rb");
    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }
    
    struct Account acc;
    printf("\nAccount Details:\n");
    printf("-------------------------------------------------\n");
    printf("Account No   Name                      Balance\n");
    printf("-------------------------------------------------\n");
    
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        printf("%-12d %-25s %.2f\n", acc.accountNumber, acc.name, acc.balance);
    }
    
    fclose(fp);
}

// Function to deposit money
void depositMoney() {
    FILE *fp = fopen("accounts.txt", "r+b");
    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }
    
    int accNo;
    float amount;
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    printf("Enter Amount to Deposit: ");
    scanf("%f", &amount);
    
    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.accountNumber == accNo) {
            acc.balance += amount;
            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&acc, sizeof(struct Account), 1, fp);
            fclose(fp);
            printf("Deposit successful! New Balance: %.2f\n", acc.balance);
            return;
        }
    }
    
    fclose(fp);
    printf("Account not found!\n");
}

// Function to withdraw money
void withdrawMoney() {
    FILE *fp = fopen("accounts.txt", "r+b");
    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }
    
    int accNo;
    float amount;
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    printf("Enter Amount to Withdraw: ");
    scanf("%f", &amount);
    
    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.accountNumber == accNo) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                fseek(fp, -sizeof(struct Account), SEEK_CUR);
                fwrite(&acc, sizeof(struct Account), 1, fp);
                fclose(fp);
                printf("Withdrawal successful! New Balance: %.2f\n", acc.balance);
            } else {
                printf("Insufficient balance!\n");
            }
            return;
        }
    }
    
    fclose(fp);
    printf("Account not found!\n");

}

void deleteAccount() {
    FILE *fp = fopen("accounts.txt", "rb");
    FILE *temp = fopen("temp.dat", "wb");
   
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int accNo, found = 0;
    struct Account acc;
   
    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accNo);

    // Copy all accounts except the one to delete
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.accountNumber == accNo) {
            found = 1;
        } else {
            fwrite(&acc, sizeof(struct Account), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace original file with updated one
    remove("accounts.txt");
    rename("temp.dat", "accounts.txt");

    if (found)
        printf("Account deleted successfully!\n");
    else
        printf("Account not found!\n");
}
