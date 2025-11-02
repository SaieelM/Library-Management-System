#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_BOOKS 500
#define MAX_MEMBERS 200
#define MAX_TRANSACTIONS 1000
#define MAX_STRING 100
#define FINE_PER_DAY 2.0
#define MAX_BORROW_DAYS 14
#define BOOKS_FILE "books.dat"
#define MEMBERS_FILE "members.dat"
#define TRANSACTIONS_FILE "transactions.dat"
#define ADMIN_FILE "admin.dat"

typedef struct {
    int bookID;
    char title[MAX_STRING];
    char author[MAX_STRING];
    char ISBN[20];
    char category[MAX_STRING];
    int totalCopies;
    int availableCopies;
    float price;
    int isActive;
} Book;

typedef struct {
    int memberID;
    char name[MAX_STRING];
    char email[MAX_STRING];
    char phone[15];
    char address[MAX_STRING];
    int booksIssued;
    float totalFines;
    int isActive;
} Member;

typedef struct {
    int transactionID;
    int bookID;
    int memberID;
    time_t issueDate;
    time_t dueDate;
    time_t returnDate;
    float fine;
    int isReturned;
} Transaction;

typedef struct {
    char username[50];
    char password[50];
} Admin;

Book books[MAX_BOOKS];
Member members[MAX_MEMBERS];
Transaction transactions[MAX_TRANSACTIONS];
int bookCount = 0;
int memberCount = 0;
int transactionCount = 0;

int fileExists(const char *filename);
void loadBooks();
void saveBooks();
void loadMembers();
void saveMembers();
void loadTransactions();
void saveTransactions();
void initializeAdmin();
int verifyAdmin(char *username, char *password);

void clearScreen();
void pauseScreen();
void toLowerCase(char *str);
int generateBookID();
int generateMemberID();
int generateTransactionID();
void printHeader(const char *title);
int getDaysDifference(time_t date1, time_t date2);

void addBook();
void viewAllBooks();
void searchBook();
void updateBook();
void deleteBook();
void bookManagementMenu();

void addMember();
void viewAllMembers();
void searchMember();
void updateMember();
void deleteMember();
void memberManagementMenu();

void issueBook();
void returnBook();
void viewIssuedBooks();
void viewMemberHistory(int memberID);
void calculateFine(Transaction *trans);
void transactionMenu();

void displayMenuRecursive(int menuType);
int searchBookRecursive(int id, int left, int right);
int searchMemberRecursive(int id, int left, int right);

void adminMenu();
void memberLoginMenu();
int mainMenu();

int main() {
    loadBooks();
    loadMembers();
    loadTransactions();
    initializeAdmin();
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║          LIBRARY MANAGEMENT SYSTEM                         ║\n");
    printf("║          Developed in C Language                           ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    pauseScreen();
    
    while (1) {
        int choice = mainMenu();
        if (choice == 3) {
            printf("\n✓ Thank you for using Library Management System!\n");
            printf("  Goodbye!\n\n");
            break;
        }
    }
    
    return 0;
}

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

void loadBooks() {
    if (!fileExists(BOOKS_FILE)) {
        bookCount = 0;
        return;
    }
    
    FILE *file = fopen(BOOKS_FILE, "rb");
    if (file == NULL) {
        printf("Error loading books!\n");
        return;
    }
    
    fread(&bookCount, sizeof(int), 1, file);
    fread(books, sizeof(Book), bookCount, file);
    fclose(file);
}

void saveBooks() {
    FILE *file = fopen(BOOKS_FILE, "wb");
    if (file == NULL) {
        printf("Error saving books!\n");
        return;
    }
    
    fwrite(&bookCount, sizeof(int), 1, file);
    fwrite(books, sizeof(Book), bookCount, file);
    fclose(file);
}

void loadMembers() {
    if (!fileExists(MEMBERS_FILE)) {
        memberCount = 0;
        return;
    }
    
    FILE *file = fopen(MEMBERS_FILE, "rb");
    if (file == NULL) {
        printf("Error loading members!\n");
        return;
    }
    
    fread(&memberCount, sizeof(int), 1, file);
    fread(members, sizeof(Member), memberCount, file);
    fclose(file);
}

void saveMembers() {
    FILE *file = fopen(MEMBERS_FILE, "wb");
    if (file == NULL) {
        printf("Error saving members!\n");
        return;
    }
    
    fwrite(&memberCount, sizeof(int), 1, file);
    fwrite(members, sizeof(Member), memberCount, file);
    fclose(file);
}

void loadTransactions() {
    if (!fileExists(TRANSACTIONS_FILE)) {
        transactionCount = 0;
        return;
    }
    
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    if (file == NULL) {
        printf("Error loading transactions!\n");
        return;
    }
    
    fread(&transactionCount, sizeof(int), 1, file);
    fread(transactions, sizeof(Transaction), transactionCount, file);
    fclose(file);
}

void saveTransactions() {
    FILE *file = fopen(TRANSACTIONS_FILE, "wb");
    if (file == NULL) {
        printf("Error saving transactions!\n");
        return;
    }
    
    fwrite(&transactionCount, sizeof(int), 1, file);
    fwrite(transactions, sizeof(Transaction), transactionCount, file);
    fclose(file);
}

void initializeAdmin() {
    if (!fileExists(ADMIN_FILE)) {
        FILE *file = fopen(ADMIN_FILE, "wb");
        if (file == NULL) {
            printf("Error creating admin file!\n");
            return;
        }
        
        Admin admin;
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin123");
        fwrite(&admin, sizeof(Admin), 1, file);
        fclose(file);
    }
}

int verifyAdmin(char *username, char *password) {
    FILE *file = fopen(ADMIN_FILE, "rb");
    if (file == NULL) {
        return 0;
    }
    
    Admin admin;
    fread(&admin, sizeof(Admin), 1, file);
    fclose(file);
    
    if (strcmp(admin.username, username) == 0 && 
        strcmp(admin.password, password) == 0) {
        return 1;
    }
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int generateBookID() {
    int maxID = 1000;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].bookID > maxID) {
            maxID = books[i].bookID;
        }
    }
    return maxID + 1;
}

int generateMemberID() {
    int maxID = 2000;
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID > maxID) {
            maxID = members[i].memberID;
        }
    }
    return maxID + 1;
}

int generateTransactionID() {
    int maxID = 5000;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].transactionID > maxID) {
            maxID = transactions[i].transactionID;
        }
    }
    return maxID + 1;
}

void printHeader(const char *title) {
    clearScreen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║  %-57s║\n", title);
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
}

int getDaysDifference(time_t date1, time_t date2) {
    return (int)difftime(date2, date1) / (60 * 60 * 24);
}

void addBook() {
    printHeader("ADD NEW BOOK");
    
    if (bookCount >= MAX_BOOKS) {
        printf("✗ Error: Maximum book limit reached!\n");
        pauseScreen();
        return;
    }
    
    Book newBook;
    newBook.bookID = generateBookID();
    newBook.isActive = 1;
    
    printf("Enter Book Details:\n");
    printf("─────────────────────\n\n");
    
    printf("Book ID (Auto-generated): %d\n", newBook.bookID);
    
    printf("Title: ");
    scanf(" %[^\n]", newBook.title);
    
    printf("Author: ");
    scanf(" %[^\n]", newBook.author);
    
    printf("ISBN: ");
    scanf(" %[^\n]", newBook.ISBN);
    
    printf("Category: ");
    scanf(" %[^\n]", newBook.category);
    
    printf("Total Copies: ");
    scanf("%d", &newBook.totalCopies);
    
    newBook.availableCopies = newBook.totalCopies;
    
    printf("Price: Rs. ");
    scanf("%f", &newBook.price);
    
    books[bookCount++] = newBook;
    saveBooks();
    
    printf("\n✓ Book added successfully with ID: %d\n", newBook.bookID);
    pauseScreen();
}

void viewAllBooks() {
    printHeader("ALL BOOKS");
    
    if (bookCount == 0) {
        printf("No books available in the library.\n");
        pauseScreen();
        return;
    }
    
    printf("%-8s %-30s %-25s %-15s %-8s\n", 
           "ID", "Title", "Author", "ISBN", "Avail");
    printf("────────────────────────────────────────────────────────────────────────────────\n");
    
    int activeCount = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive) {
            printf("%-8d %-30s %-25s %-15s %d/%d\n",
                   books[i].bookID,
                   books[i].title,
                   books[i].author,
                   books[i].ISBN,
                   books[i].availableCopies,
                   books[i].totalCopies);
            activeCount++;
        }
    }
    
    printf("\nTotal Active Books: %d\n", activeCount);
    pauseScreen();
}

void searchBook() {
    printHeader("SEARCH BOOK");
    
    printf("Search By:\n");
    printf("1. Book ID\n");
    printf("2. Title\n");
    printf("3. Author\n");
    printf("4. ISBN\n");
    printf("\nChoice: ");
    
    int choice;
    scanf("%d", &choice);
    
    int found = 0;
    
    switch (choice) {
        case 1: {
            int id;
            printf("Enter Book ID: ");
            scanf("%d", &id);
            
            for (int i = 0; i < bookCount; i++) {
                if (books[i].bookID == id && books[i].isActive) {
                    printf("\n✓ Book Found:\n");
                    printf("─────────────────\n");
                    printf("ID          : %d\n", books[i].bookID);
                    printf("Title       : %s\n", books[i].title);
                    printf("Author      : %s\n", books[i].author);
                    printf("ISBN        : %s\n", books[i].ISBN);
                    printf("Category    : %s\n", books[i].category);
                    printf("Available   : %d/%d\n", 
                           books[i].availableCopies, books[i].totalCopies);
                    printf("Price       : Rs. %.2f\n", books[i].price);
                    found = 1;
                    break;
                }
            }
            break;
        }
        case 2: {
            char title[MAX_STRING];
            printf("Enter Title: ");
            scanf(" %[^\n]", title);
            toLowerCase(title);
            
            printf("\n%-8s %-30s %-25s %-8s\n", "ID", "Title", "Author", "Avail");
            printf("────────────────────────────────────────────────────────────────\n");
            
            for (int i = 0; i < bookCount; i++) {
                if (books[i].isActive) {
                    char bookTitle[MAX_STRING];
                    strcpy(bookTitle, books[i].title);
                    toLowerCase(bookTitle);
                    
                    if (strstr(bookTitle, title) != NULL) {
                        printf("%-8d %-30s %-25s %d/%d\n",
                               books[i].bookID, books[i].title, 
                               books[i].author,
                               books[i].availableCopies, books[i].totalCopies);
                        found = 1;
                    }
                }
            }
            break;
        }
        case 3: {
            char author[MAX_STRING];
            printf("Enter Author: ");
            scanf(" %[^\n]", author);
            toLowerCase(author);
            
            printf("\n%-8s %-30s %-25s %-8s\n", "ID", "Title", "Author", "Avail");
            printf("────────────────────────────────────────────────────────────────\n");
            
            for (int i = 0; i < bookCount; i++) {
                if (books[i].isActive) {
                    char bookAuthor[MAX_STRING];
                    strcpy(bookAuthor, books[i].author);
                    toLowerCase(bookAuthor);
                    
                    if (strstr(bookAuthor, author) != NULL) {
                        printf("%-8d %-30s %-25s %d/%d\n",
                               books[i].bookID, books[i].title, 
                               books[i].author,
                               books[i].availableCopies, books[i].totalCopies);
                        found = 1;
                    }
                }
            }
            break;
        }
        case 4: {
            char isbn[20];
            printf("Enter ISBN: ");
            scanf(" %[^\n]", isbn);
            
            for (int i = 0; i < bookCount; i++) {
                if (strcmp(books[i].ISBN, isbn) == 0 && books[i].isActive) {
                    printf("\n✓ Book Found:\n");
                    printf("─────────────────\n");
                    printf("ID          : %d\n", books[i].bookID);
                    printf("Title       : %s\n", books[i].title);
                    printf("Author      : %s\n", books[i].author);
                    printf("ISBN        : %s\n", books[i].ISBN);
                    printf("Available   : %d/%d\n", 
                           books[i].availableCopies, books[i].totalCopies);
                    found = 1;
                    break;
                }
            }
            break;
        }
        default:
            printf("\n✗ Invalid choice!\n");
    }
    
    if (!found && choice >= 1 && choice <= 4) {
        printf("\n✗ No books found!\n");
    }
    
    pauseScreen();
}

void updateBook() {
    printHeader("UPDATE BOOK");
    
    int id;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    
    int found = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].bookID == id && books[i].isActive) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("\n✗ Book not found!\n");
        pauseScreen();
        return;
    }
    
    printf("\nCurrent Details:\n");
    printf("─────────────────\n");
    printf("Title       : %s\n", books[found].title);
    printf("Author      : %s\n", books[found].author);
    printf("ISBN        : %s\n", books[found].ISBN);
    printf("Category    : %s\n", books[found].category);
    printf("Total Copies: %d\n", books[found].totalCopies);
    printf("Price       : Rs. %.2f\n", books[found].price);
    
    printf("\nEnter New Details (press Enter to keep current):\n");
    printf("─────────────────────────────────────────────────\n");
    
    char input[MAX_STRING];
    
    printf("Title [%s]: ", books[found].title);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) strcpy(books[found].title, input);
    
    printf("Author [%s]: ", books[found].author);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) strcpy(books[found].author, input);
    
    printf("Category [%s]: ", books[found].category);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) strcpy(books[found].category, input);
    
    printf("Price [%.2f]: ", books[found].price);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) books[found].price = atof(input);
    
    saveBooks();
    printf("\n✓ Book updated successfully!\n");
    pauseScreen();
}

void deleteBook() {
    printHeader("DELETE BOOK");
    
    int id;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    
    int found = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].bookID == id && books[i].isActive) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("\n✗ Book not found!\n");
        pauseScreen();
        return;
    }
    
    if (books[found].availableCopies < books[found].totalCopies) {
        printf("\n✗ Cannot delete! Book has been issued to members.\n");
        pauseScreen();
        return;
    }
    
    printf("\nBook: %s by %s\n", books[found].title, books[found].author);
    printf("Are you sure you want to delete? (y/n): ");
    
    char confirm;
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        books[found].isActive = 0;
        saveBooks();
        printf("\n✓ Book deleted successfully!\n");
    } else {
        printf("\n✓ Deletion cancelled.\n");
    }
    
    pauseScreen();
}

void bookManagementMenu() {
    while (1) {
        printHeader("BOOK MANAGEMENT");
        
        printf("1. Add New Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book\n");
        printf("4. Update Book\n");
        printf("5. Delete Book\n");
        printf("6. Back to Main Menu\n");
        printf("\nChoice: ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addBook(); break;
            case 2: viewAllBooks(); break;
            case 3: searchBook(); break;
            case 4: updateBook(); break;
            case 5: deleteBook(); break;
            case 6: return;
            default:
                printf("\n✗ Invalid choice!\n");
                pauseScreen();
        }
    }
}

void addMember() {
    printHeader("ADD NEW MEMBER");
    
    if (memberCount >= MAX_MEMBERS) {
        printf("✗ Error: Maximum member limit reached!\n");
        pauseScreen();
        return;
    }
    
    Member newMember;
    newMember.memberID = generateMemberID();
    newMember.booksIssued = 0;
    newMember.totalFines = 0.0;
    newMember.isActive = 1;
    
    printf("Enter Member Details:\n");
    printf("─────────────────────\n\n");
    
    printf("Member ID (Auto-generated): %d\n", newMember.memberID);
    
    printf("Name: ");
    scanf(" %[^\n]", newMember.name);
    
    printf("Email: ");
    scanf(" %[^\n]", newMember.email);
    
    printf("Phone: ");
    scanf(" %[^\n]", newMember.phone);
    
    printf("Address: ");
    scanf(" %[^\n]", newMember.address);
    
    members[memberCount++] = newMember;
    saveMembers();
    
    printf("\n✓ Member registered successfully with ID: %d\n", newMember.memberID);
    pauseScreen();
}

void viewAllMembers() {
    printHeader("ALL MEMBERS");
    
    if (memberCount == 0) {
        printf("No members registered.\n");
        pauseScreen();
        return;
    }
    
    printf("%-8s %-25s %-30s %-15s %-8s\n", 
           "ID", "Name", "Email", "Phone", "Issued");
    printf("────────────────────────────────────────────────────────────────────────────────\n");
    
    int activeCount = 0;
    for (int i = 0; i < memberCount; i++) {
        if (members[i].isActive) {
            printf("%-8d %-25s %-30s %-15s %-8d\n",
                   members[i].memberID,
                   members[i].name,
                   members[i].email,
                   members[i].phone,
                   members[i].booksIssued);
            activeCount++;
        }
    }
    
    printf("\nTotal Active Members: %d\n", activeCount);
    pauseScreen();
}

void searchMember() {
    printHeader("SEARCH MEMBER");
    
    printf("Search By:\n");
    printf("1. Member ID\n");
    printf("2. Name\n");
    printf("\nChoice: ");
    
    int choice;
    scanf("%d", &choice);
    
    int found = 0;
    
    switch (choice) {
        case 1: {
            int id;
            printf("Enter Member ID: ");
            scanf("%d", &id);
            
            for (int i = 0; i < memberCount; i++) {
                if (members[i].memberID == id && members[i].isActive) {
                    printf("\n✓ Member Found:\n");
                    printf("─────────────────\n");
                    printf("ID          : %d\n", members[i].memberID);
                    printf("Name        : %s\n", members[i].name);
                    printf("Email       : %s\n", members[i].email);
                    printf("Phone       : %s\n", members[i].phone);
                    printf("Address     : %s\n", members[i].address);
                    printf("Books Issued: %d\n", members[i].booksIssued);
                    printf("Total Fines : Rs. %.2f\n", members[i].totalFines);
                    found = 1;
                    break;
                }
            }
            break;
        }
        case 2: {
            char name[MAX_STRING];
            printf("Enter Name: ");
            scanf(" %[^\n]", name);
            toLowerCase(name);
            
            printf("\n%-8s %-25s %-30s %-8s\n", "ID", "Name", "Email", "Issued");
            printf("────────────────────────────────────────────────────────────────\n");
            
            for (int i = 0; i < memberCount; i++) {
                if (members[i].isActive) {
                    char memberName[MAX_STRING];
                    strcpy(memberName, members[i].name);
                    toLowerCase(memberName);
                    
                    if (strstr(memberName, name) != NULL) {
                        printf("%-8d %-25s %-30s %-8d\n",
                               members[i].memberID, members[i].name,
                               members[i].email, members[i].booksIssued);
                        found = 1;
                    }
                }
            }
            break;
        }
        default:
            printf("\n✗ Invalid choice!\n");
    }
    
    if (!found && (choice == 1 || choice == 2)) {
        printf("\n✗ No members found!\n");
    }
    
    pauseScreen();
}

void updateMember() {
    printHeader("UPDATE MEMBER");
    
    int id;
    printf("Enter Member ID to update: ");
    scanf("%d", &id);
    
    int found = -1;
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID == id && members[i].isActive) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("\n✗ Member not found!\n");
        pauseScreen();
        return;
    }
    
    printf("\nCurrent Details:\n");
    printf("─────────────────\n");
    printf("Name    : %s\n", members[found].name);
    printf("Email   : %s\n", members[found].email);
    printf("Phone   : %s\n", members[found].phone);
    printf("Address : %s\n", members[found].address);
    
    printf("\nEnter New Details (press Enter to keep current):\n");
    printf("─────────────────────────────────────────────────\n");
    
    char input[MAX_STRING];
    
    printf("Name [%s]: ", members[found].name);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) strcpy(members[found].name, input);
    
    printf("Email [%s]: ", members[found].email);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) strcpy(members[found].email, input);
    
    printf("Phone [%s]: ", members[found].phone);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) strcpy(members[found].phone, input);
    
    printf("Address [%s]: ", members[found].address);
    scanf(" %[^\n]", input);
    if (strlen(input) > 0) strcpy(members[found].address, input);
    
    saveMembers();
    printf("\n✓ Member updated successfully!\n");
    pauseScreen();
}

void deleteMember() {
    printHeader("DELETE MEMBER");
    
    int id;
    printf("Enter Member ID to delete: ");
    scanf("%d", &id);
    
    int found = -1;
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID == id && members[i].isActive) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("\n✗ Member not found!\n");
        pauseScreen();
        return;
    }
    
    if (members[found].booksIssued > 0) {
        printf("\n✗ Cannot delete! Member has issued books.\n");
        pauseScreen();
        return;
    }
    
    printf("\nMember: %s (ID: %d)\n", members[found].name, members[found].memberID);
    printf("Are you sure you want to delete? (y/n): ");
    
    char confirm;
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        members[found].isActive = 0;
        saveMembers();
        printf("\n✓ Member deleted successfully!\n");
    } else {
        printf("\n✓ Deletion cancelled.\n");
    }
    
    pauseScreen();
}

void memberManagementMenu() {
    while (1) {
        printHeader("MEMBER MANAGEMENT");
        
        printf("1. Add New Member\n");
        printf("2. View All Members\n");
        printf("3. Search Member\n");
        printf("4. Update Member\n");
        printf("5. Delete Member\n");
        printf("6. Back to Main Menu\n");
        printf("\nChoice: ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addMember(); break;
            case 2: viewAllMembers(); break;
            case 3: searchMember(); break;
            case 4: updateMember(); break;
            case 5: deleteMember(); break;
            case 6: return;
            default:
                printf("\n✗ Invalid choice!\n");
                pauseScreen();
        }
    }
}

void calculateFine(Transaction *trans) {
    if (trans->isReturned) {
        time_t returnDate = trans->returnDate;
        int daysLate = getDaysDifference(trans->dueDate, returnDate);
        
        if (daysLate > 0) {
            if (daysLate <= 7) {
                trans->fine = daysLate * 2.0;
            } else if (daysLate <= 14) {
                trans->fine = (7 * 2.0) + ((daysLate - 7) * 4.0);
            } else {
                trans->fine = (7 * 2.0) + (7 * 4.0) + ((daysLate - 14) * 6.0);
            }
        } else {
            trans->fine = 0.0;
        }
    }
}

void issueBook() {
    printHeader("ISSUE BOOK");
    
    int bookID, memberID;
    
    printf("Enter Book ID: ");
    scanf("%d", &bookID);
    
    printf("Enter Member ID: ");
    scanf("%d", &memberID);
    
    int bookIndex = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].bookID == bookID && books[i].isActive) {
            bookIndex = i;
            break;
        }
    }
    
    if (bookIndex == -1) {
        printf("\n✗ Book not found!\n");
        pauseScreen();
        return;
    }
    
    if (books[bookIndex].availableCopies <= 0) {
        printf("\n✗ Book not available! All copies issued.\n");
        pauseScreen();
        return;
    }
    
    int memberIndex = -1;
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID == memberID && members[i].isActive) {
            memberIndex = i;
            break;
        }
    }
    
    if (memberIndex == -1) {
        printf("\n✗ Member not found!\n");
        pauseScreen();
        return;
    }
    
    if (members[memberIndex].booksIssued >= 3) {
        printf("\n✗ Member has already issued maximum books (3)!\n");
        pauseScreen();
        return;
    }
    
    if (transactionCount >= MAX_TRANSACTIONS) {
        printf("\n✗ Transaction limit reached!\n");
        pauseScreen();
        return;
    }
    
    Transaction newTrans;
    newTrans.transactionID = generateTransactionID();
    newTrans.bookID = bookID;
    newTrans.memberID = memberID;
    newTrans.issueDate = time(NULL);
    newTrans.dueDate = newTrans.issueDate + (MAX_BORROW_DAYS * 24 * 60 * 60);
    newTrans.returnDate = 0;
    newTrans.fine = 0.0;
    newTrans.isReturned = 0;
    
    transactions[transactionCount++] = newTrans;
    books[bookIndex].availableCopies--;
    members[memberIndex].booksIssued++;
    
    saveTransactions();
    saveBooks();
    saveMembers();
    
    char issueStr[26], dueStr[26];
    strftime(issueStr, 26, "%Y-%m-%d", localtime(&newTrans.issueDate));
    strftime(dueStr, 26, "%Y-%m-%d", localtime(&newTrans.dueDate));
    
    printf("\n✓ Book issued successfully!\n");
    printf("─────────────────────────────\n");
    printf("Transaction ID : %d\n", newTrans.transactionID);
    printf("Book           : %s\n", books[bookIndex].title);
    printf("Member         : %s\n", members[memberIndex].name);
    printf("Issue Date     : %s\n", issueStr);
    printf("Due Date       : %s\n", dueStr);
    printf("─────────────────────────────\n");
    printf("Note: Please return within %d days to avoid fine.\n", MAX_BORROW_DAYS);
    
    pauseScreen();
}

void returnBook() {
    printHeader("RETURN BOOK");
    
    int transID;
    printf("Enter Transaction ID: ");
    scanf("%d", &transID);
    
    int transIndex = -1;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].transactionID == transID && !transactions[i].isReturned) {
            transIndex = i;
            break;
        }
    }
    
    if (transIndex == -1) {
        printf("\n✗ Transaction not found or book already returned!\n");
        pauseScreen();
        return;
    }
    
    transactions[transIndex].returnDate = time(NULL);
    transactions[transIndex].isReturned = 1;
    
    calculateFine(&transactions[transIndex]);
    
    int bookIndex = -1, memberIndex = -1;
    
    for (int i = 0; i < bookCount; i++) {
        if (books[i].bookID == transactions[transIndex].bookID) {
            bookIndex = i;
            break;
        }
    }
    
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID == transactions[transIndex].memberID) {
            memberIndex = i;
            break;
        }
    }
    
    if (bookIndex != -1) {
        books[bookIndex].availableCopies++;
    }
    
    if (memberIndex != -1) {
        members[memberIndex].booksIssued--;
        members[memberIndex].totalFines += transactions[transIndex].fine;
    }
    
    saveTransactions();
    saveBooks();
    saveMembers();
    
    char returnStr[26];
    strftime(returnStr, 26, "%Y-%m-%d", localtime(&transactions[transIndex].returnDate));
    
    printf("\n✓ Book returned successfully!\n");
    printf("─────────────────────────────\n");
    printf("Transaction ID : %d\n", transactions[transIndex].transactionID);
    if (bookIndex != -1) {
        printf("Book           : %s\n", books[bookIndex].title);
    }
    if (memberIndex != -1) {
        printf("Member         : %s\n", members[memberIndex].name);
    }
    printf("Return Date    : %s\n", returnStr);
    
    if (transactions[transIndex].fine > 0) {
        printf("\n⚠ FINE: Rs. %.2f\n", transactions[transIndex].fine);
        printf("  (Late by %d days)\n", 
               getDaysDifference(transactions[transIndex].dueDate, 
                               transactions[transIndex].returnDate));
    } else {
        printf("\n✓ Returned on time. No fine!\n");
    }
    
    pauseScreen();
}

void viewIssuedBooks() {
    printHeader("CURRENTLY ISSUED BOOKS");
    
    int issuedCount = 0;
    
    printf("%-8s %-30s %-20s %-12s %-12s\n",
           "Trans ID", "Book Title", "Member Name", "Issue Date", "Due Date");
    printf("────────────────────────────────────────────────────────────────────────────────\n");
    
    for (int i = 0; i < transactionCount; i++) {
        if (!transactions[i].isReturned) {
            char bookTitle[MAX_STRING] = "Unknown";
            char memberName[MAX_STRING] = "Unknown";
            
            for (int j = 0; j < bookCount; j++) {
                if (books[j].bookID == transactions[i].bookID) {
                    strcpy(bookTitle, books[j].title);
                    break;
                }
            }
            
            for (int j = 0; j < memberCount; j++) {
                if (members[j].memberID == transactions[i].memberID) {
                    strcpy(memberName, members[j].name);
                    break;
                }
            }
            
            char issueStr[12], dueStr[12];
            strftime(issueStr, 12, "%Y-%m-%d", localtime(&transactions[i].issueDate));
            strftime(dueStr, 12, "%Y-%m-%d", localtime(&transactions[i].dueDate));
            
            printf("%-8d %-30s %-20s %-12s %-12s",
                   transactions[i].transactionID, bookTitle, memberName,
                   issueStr, dueStr);
            
            time_t now = time(NULL);
            if (now > transactions[i].dueDate) {
                int daysOverdue = getDaysDifference(transactions[i].dueDate, now);
                printf(" ⚠ OVERDUE (%d days)", daysOverdue);
            }
            
            printf("\n");
            issuedCount++;
        }
    }
    
    printf("\nTotal Issued Books: %d\n", issuedCount);
    pauseScreen();
}

void viewMemberHistory(int memberID) {
    printHeader("MEMBER TRANSACTION HISTORY");
    
    printf("Member ID: %d\n\n", memberID);
    
    printf("%-8s %-30s %-12s %-12s %-10s %-10s\n",
           "Trans ID", "Book", "Issue Date", "Return Date", "Status", "Fine");
    printf("────────────────────────────────────────────────────────────────────────────────\n");
    
    int count = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].memberID == memberID) {
            char bookTitle[MAX_STRING] = "Unknown";
            
            for (int j = 0; j < bookCount; j++) {
                if (books[j].bookID == transactions[i].bookID) {
                    strcpy(bookTitle, books[j].title);
                    break;
                }
            }
            
            char issueStr[12], returnStr[12];
            strftime(issueStr, 12, "%Y-%m-%d", localtime(&transactions[i].issueDate));
            
            if (transactions[i].isReturned) {
                strftime(returnStr, 12, "%Y-%m-%d", localtime(&transactions[i].returnDate));
            } else {
                strcpy(returnStr, "Not Yet");
            }
            
            printf("%-8d %-30s %-12s %-12s %-10s Rs. %.2f\n",
                   transactions[i].transactionID, bookTitle, issueStr, returnStr,
                   transactions[i].isReturned ? "Returned" : "Issued",
                   transactions[i].fine);
            count++;
        }
    }
    
    if (count == 0) {
        printf("No transaction history found.\n");
    }
    
    pauseScreen();
}

void transactionMenu() {
    while (1) {
        printHeader("TRANSACTION MANAGEMENT");
        
        printf("1. Issue Book\n");
        printf("2. Return Book\n");
        printf("3. View Currently Issued Books\n");
        printf("4. View Member History\n");
        printf("5. Back to Main Menu\n");
        printf("\nChoice: ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: issueBook(); break;
            case 2: returnBook(); break;
            case 3: viewIssuedBooks(); break;
            case 4: {
                int memberID;
                printf("\nEnter Member ID: ");
                scanf("%d", &memberID);
                viewMemberHistory(memberID);
                break;
            }
            case 5: return;
            default:
                printf("\n✗ Invalid choice!\n");
                pauseScreen();
        }
    }
}

void displayMenuRecursive(int menuType) {
    if (menuType == 0) return;
    
    if (menuType == 1) {
        printf("→ Admin Menu\n");
    } else if (menuType == 2) {
        printf("  → Book Management\n");
    } else if (menuType == 3) {
        printf("    → View Books\n");
    }
    
    displayMenuRecursive(menuType - 1);
}

int searchBookRecursive(int id, int left, int right) {
    if (left > right) {
        return -1;
    }
    
    int mid = left + (right - left) / 2;
    
    if (books[mid].bookID == id && books[mid].isActive) {
        return mid;
    }
    
    if (books[mid].bookID > id) {
        return searchBookRecursive(id, left, mid - 1);
    } else {
        return searchBookRecursive(id, mid + 1, right);
    }
}

int searchMemberRecursive(int id, int left, int right) {
    if (left > right) {
        return -1;
    }
    
    int mid = left + (right - left) / 2;
    
    if (members[mid].memberID == id && members[mid].isActive) {
        return mid;
    }
    
    if (members[mid].memberID > id) {
        return searchMemberRecursive(id, left, mid - 1);
    } else {
        return searchMemberRecursive(id, mid + 1, right);
    }
}

void adminMenu() {
    while (1) {
        printHeader("ADMIN DASHBOARD");
        
        printf("1. Book Management\n");
        printf("2. Member Management\n");
        printf("3. Transaction Management\n");
        printf("4. View Statistics\n");
        printf("5. Logout\n");
        printf("\nChoice: ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                bookManagementMenu();
                break;
            case 2:
                memberManagementMenu();
                break;
            case 3:
                transactionMenu();
                break;
            case 4: {
                printHeader("LIBRARY STATISTICS");
                
                int activeBooks = 0, activeMembers = 0, issuedBooks = 0;
                float totalFines = 0.0;
                
                for (int i = 0; i < bookCount; i++) {
                    if (books[i].isActive) activeBooks++;
                }
                
                for (int i = 0; i < memberCount; i++) {
                    if (members[i].isActive) {
                        activeMembers++;
                        totalFines += members[i].totalFines;
                    }
                }
                
                for (int i = 0; i < transactionCount; i++) {
                    if (!transactions[i].isReturned) issuedBooks++;
                }
                
                printf("Total Books       : %d\n", activeBooks);
                printf("Total Members     : %d\n", activeMembers);
                printf("Currently Issued  : %d\n", issuedBooks);
                printf("Total Transactions: %d\n", transactionCount);
                printf("Total Fines       : Rs. %.2f\n", totalFines);
                
                pauseScreen();
                break;
            }
            case 5:
                printf("\n✓ Logged out successfully!\n");
                pauseScreen();
                return;
            default:
                printf("\n✗ Invalid choice!\n");
                pauseScreen();
        }
    }
}

int mainMenu() {
    printHeader("MAIN MENU");
    
    printf("1. Admin Login\n");
    printf("2. Member Portal\n");
    printf("3. Exit\n");
    printf("\nChoice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            char username[50], password[50];
            
            printHeader("ADMIN LOGIN");
            printf("Username: ");
            scanf("%s", username);
            printf("Password: ");
            scanf("%s", password);
            
            if (verifyAdmin(username, password)) {
                printf("\n✓ Login successful!\n");
                pauseScreen();
                adminMenu();
            } else {
                printf("\n✗ Invalid credentials!\n");
                pauseScreen();
            }
            break;
        }
        case 2: {
            int memberID;
            printHeader("MEMBER PORTAL");
            printf("Enter Member ID: ");
            scanf("%d", &memberID);
            
            int found = -1;
            for (int i = 0; i < memberCount; i++) {
                if (members[i].memberID == memberID && members[i].isActive) {
                    found = i;
                    break;
                }
            }
            
            if (found != -1) {
                printf("\n✓ Welcome, %s!\n", members[found].name);
                pauseScreen();
                
                while (1) {
                    printHeader("MEMBER DASHBOARD");
                    printf("Member: %s (ID: %d)\n\n", members[found].name, memberID);
                    
                    printf("1. View My Issued Books\n");
                    printf("2. View Transaction History\n");
                    printf("3. Search Books\n");
                    printf("4. Logout\n");
                    printf("\nChoice: ");
                    
                    int memberChoice;
                    scanf("%d", &memberChoice);
                    
                    switch (memberChoice) {
                        case 1: {
                            printHeader("MY ISSUED BOOKS");
                            
                            printf("%-30s %-12s %-12s %-8s\n",
                                   "Book Title", "Issue Date", "Due Date", "Status");
                            printf("────────────────────────────────────────────────────────────────\n");
                            
                            int count = 0;
                            time_t now = time(NULL);
                            
                            for (int i = 0; i < transactionCount; i++) {
                                if (transactions[i].memberID == memberID && 
                                    !transactions[i].isReturned) {
                                    
                                    char bookTitle[MAX_STRING] = "Unknown";
                                    for (int j = 0; j < bookCount; j++) {
                                        if (books[j].bookID == transactions[i].bookID) {
                                            strcpy(bookTitle, books[j].title);
                                            break;
                                        }
                                    }
                                    
                                    char issueStr[12], dueStr[12];
                                    strftime(issueStr, 12, "%Y-%m-%d", 
                                            localtime(&transactions[i].issueDate));
                                    strftime(dueStr, 12, "%Y-%m-%d", 
                                            localtime(&transactions[i].dueDate));
                                    
                                    printf("%-30s %-12s %-12s ", bookTitle, issueStr, dueStr);
                                    
                                    if (now > transactions[i].dueDate) {
                                        int daysOverdue = getDaysDifference(
                                            transactions[i].dueDate, now);
                                        printf("⚠ OVERDUE (%d days)", daysOverdue);
                                    } else {
                                        printf("✓ Active");
                                    }
                                    
                                    printf("\n");
                                    count++;
                                }
                            }
                            
                            if (count == 0) {
                                printf("No books currently issued.\n");
                            }
                            
                            pauseScreen();
                            break;
                        }
                        case 2:
                            viewMemberHistory(memberID);
                            break;
                        case 3:
                            searchBook();
                            break;
                        case 4:
                            printf("\n✓ Logged out!\n");
                            pauseScreen();
                            goto member_exit;
                        default:
                            printf("\n✗ Invalid choice!\n");
                            pauseScreen();
                    }
                }
                member_exit:;
            } else {
                printf("\n✗ Member ID not found!\n");
                pauseScreen();
            }
            break;
        }
        case 3:
            return 3;
        default:
            printf("\n✗ Invalid choice!\n");
            pauseScreen();
    }
    
    return 0;
}
