# Library Management System — Feature Documentation

This project is a comprehensive, single-file, command-line application built in C that manages the entire workflow of a small library: from cataloging books to tracking member borrowing history. It features role-based access for Admins and Members, persistent data storage using .dat files, and an automated fine calculation system.

This document explains the major features of the Library Management System with practical examples and real-world analogies to help you understand how each component works. It synthesizes information from all project files (main.c, README.md, and DOCUMENTATION.docx).

---

## Table of Contents

1. [System Architecture Overview](#system-architecture-overview)
2. [Authentication & Access Control](#authentication--access-control)
3. [Book Management](#book-management)
4. [Member Management](#member-management)
5. [Transaction System](#transaction-system)
6. [Fine Calculation System](#fine-calculation-system)
7. [Persistent Storage](#persistent-storage)
8. [Search & Retrieval](#search--retrieval)

---

## System Architecture Overview

### What It Does
The Library Management System is a **single-file, command-line application** built in C that manages the entire workflow of a small library: from cataloging books to tracking member borrowing history.

### Analogy: The Digital Librarian
Think of this program as a **tireless digital librarian** who never forgets anything. Just like a human librarian keeps physical records in filing cabinets, this system keeps digital records in `.dat` files. When you close the program and reopen it, the librarian remembers everything — every book, every member, every transaction.

### Key Components
- **Admin Module**: The library manager's control panel
- **Member Module**: Self-service kiosk for library members
- **Data Storage**: Digital filing cabinets (binary files)
- **Transaction Engine**: The checkout/return desk

---

## Authentication & Access Control

### What It Does
The system implements **role-based access control** with two distinct user types:
- **Admin**: Full control (default credentials: `admin` / `admin123`)
- **Member**: Limited access (login with Member ID only)

### Analogy: Security Badges
Imagine a building with different security clearance levels:
- **Admin** has a **master keycard** that opens all doors (book management, member registration, reports, etc.)
- **Member** has a **visitor badge** that only opens specific doors (view their own books, search the catalog, check their history)

### Implementation Details
```
Admin credentials stored in: admin.dat
Member access uses: Member ID (auto-generated, starting from 2001)
```

When an admin logs in, the system reads the `admin.dat` file and compares the username and password. Members simply provide their ID — no password required.

---

## Book Management

### What It Does
Administrators can:
- **Add new books** to the library catalog
- **View all books** with availability status
- **Search books** by ID, title, author, or ISBN
- **Update book details** (title, author, category, price)
- **Delete books** (soft delete — only if no copies are currently issued)

### Analogy: The Card Catalog System
Remember old libraries with card catalogs? This is the **digital version**:
- Each book gets a **unique ID** (like a catalog number)
- Books have **metadata** (title, author, ISBN, category, price)
- The system tracks **total copies** and **available copies** (like knowing you have 3 copies but 1 is checked out)

### Key Features

#### Auto-Generated Book IDs
```
First book: ID 1001
Second book: ID 1002
...and so on
```
Just like how libraries use call numbers, this system automatically assigns unique IDs starting from 1001.

#### Copy Tracking
```
Total Copies: 5
Available Copies: 3
(Means 2 copies are currently issued to members)
```

### Analogy: Library Shelves
Think of the book database as **organized shelves**:
- When you add a book, you're placing it on a specific shelf
- When someone borrows it, you move it to the "checked out" section
- When returned, it goes back on the shelf
- Deleting a book is like removing it from the catalog (but only if all copies are back on the shelf)

---

## Member Management

### What It Does
Administrators can:
- **Register new members** (auto-generates Member ID)
- **View all members** with their borrowing statistics
- **Search members** by ID or name
- **Update member information** (name, email, phone, address)
- **Delete members** (only if they have no outstanding books)

### Analogy: Library Membership Cards
Each member is like someone with a **library membership card**:
- **Member ID**: The card number (unique, starting from 2001)
- **Contact Info**: How to reach them if a book is overdue
- **Books Issued**: How many books they currently have (max 3)
- **Total Fines**: Running total of all fines ever accumulated

### Member Profile Structure
```
Member ID: 2001
Name: John Doe
Email: john@example.com
Phone: 123-456-7890
Books Currently Issued: 2 (out of max 3)
Total Fines Accumulated: Rs. 24.00
Status: Active
```

### Key Constraint
**A member can borrow up to 3 books at a time** — like a lending limit on a library card. Once they have 3 books checked out, they must return at least one before borrowing another.

---

## Transaction System

### What It Does
The transaction system handles:
- **Issuing books** to members (checkout)
- **Returning books** from members (check-in)
- **Viewing currently issued books** (what's out right now)
- **Member history** (complete borrowing record)

### Analogy: The Checkout Desk
Think of this as the **library circulation desk**:
- **Issue Book** = Librarian stamps the due date card and hands you the book
- **Return Book** = You return the book, librarian checks the date and calculates any late fees
- **Transaction Record** = The permanent record of who borrowed what and when

### Transaction Lifecycle

#### 1. Issue Book (Checkout)
```
Input: Book ID + Member ID
Process:
  ✓ Verify book is available (availableCopies > 0)
  ✓ Verify member exists and is active
  ✓ Check member hasn't exceeded limit (< 3 books)
  ✓ Create transaction record
  ✓ Set due date (current date + 14 days)
  ✓ Decrease book's availableCopies
  ✓ Increase member's booksIssued counter
Output: Transaction ID and due date
```

#### 2. Return Book (Check-in)
```
Input: Transaction ID
Process:
  ✓ Find the transaction
  ✓ Set return date to today
  ✓ Calculate fine (if overdue)
  ✓ Increase book's availableCopies
  ✓ Decrease member's booksIssued counter
  ✓ Add fine to member's totalFines
Output: Return confirmation and fine amount (if any)
```

### Analogy: Package Tracking
Each transaction is like **shipping a package**:
- **Transaction ID**: Tracking number
- **Issue Date**: When the package was sent
- **Due Date**: Expected delivery date
- **Return Date**: Actual delivery date
- **Fine**: Late delivery penalty

---

## Fine Calculation System

### What It Does
The system automatically calculates fines for overdue books using a **tiered penalty structure** — the longer you're late, the more expensive each day becomes.

### Fine Structure (Tiered Penalty)
```
Days 1-7 overdue:     Rs. 2.00 per day
Days 8-14 overdue:    Rs. 4.00 per day (after first 7 days)
Days 15+ overdue:     Rs. 6.00 per day (after first 14 days)
```

### Analogy: Parking Meter
Think of this like **parking fines**:
- Park 10 minutes over: Small fine
- Park 1 hour over: Bigger fine
- Park overnight: Even bigger fine

The library wants to **encourage quick returns** by making extended lateness progressively more expensive.

### Example Calculations

#### Example 1: Returned 5 Days Late
```
Days overdue: 5
All within tier 1 (days 1-7): 5 × Rs. 2.00 = Rs. 10.00
Total Fine: Rs. 10.00
```

#### Example 2: Returned 10 Days Late
```
Days overdue: 10
First 7 days (tier 1): 7 × Rs. 2.00 = Rs. 14.00
Next 3 days (tier 2): 3 × Rs. 4.00 = Rs. 12.00
Total Fine: Rs. 26.00
```

#### Example 3: Returned 20 Days Late
```
Days overdue: 20
First 7 days (tier 1): 7 × Rs. 2.00 = Rs. 14.00
Next 7 days (tier 2): 7 × Rs. 4.00 = Rs. 28.00
Next 6 days (tier 3): 6 × Rs. 6.00 = Rs. 36.00
Total Fine: Rs. 78.00
```

### Implementation Note
The fine is calculated in the `calculateFine()` function and is **only computed when the book is returned**, not while it's still checked out. This is stored in the transaction record and added to the member's total fine history.

---

## Persistent Storage

### What It Does
All data is saved to **binary files** (`.dat`) so that when you close and reopen the program, everything is preserved — books, members, transactions, and admin credentials.

### Analogy: Filing Cabinets That Never Lose Papers
Imagine traditional filing cabinets where:
- **books.dat** = Cabinet for book records
- **members.dat** = Cabinet for member cards
- **transactions.dat** = Cabinet for checkout/return slips
- **admin.dat** = Locked drawer with admin keys

When you close the program, it's like the librarian **files all papers** into these cabinets. When you reopen it, the librarian **pulls everything back out** and continues where they left off.

### File Structure

#### books.dat
```
[Integer: bookCount]
[Book struct × bookCount]
```

#### members.dat
```
[Integer: memberCount]
[Member struct × memberCount]
```

#### transactions.dat
```
[Integer: transactionCount]
[Transaction struct × transactionCount]
```

#### admin.dat
```
[Admin struct with username and password]
```

### Analogy: Save Game Feature
Think of this like **video game save files**:
- Each `.dat` file is a save file
- When you quit the program, it auto-saves
- When you restart, it auto-loads from the save files
- Delete the `.dat` files = reset to factory settings (new game)

### Important Notes
- **Binary format** = Not human-readable (efficient storage, smaller files)
- **No encryption** = Files can be read if you know the struct format
- **Single-user** = No file locking or concurrent access handling

---

## Search & Retrieval

### What It Does
The system provides multiple search methods:
- **Book Search**: By ID, Title, Author, ISBN
- **Member Search**: By ID, Name
- **String Matching**: Case-insensitive partial matching for titles/names

### Analogy: Google for Your Library
Just like Google helps you find websites, these search functions help you find:
- **Books** (even if you only remember part of the title)
- **Members** (by name or ID)

### Search Methods

#### 1. Search by ID (Fast)
```
Use case: "I need book #1005"
Method: Direct lookup (Linear Search)
Speed: O(n)
```

#### 2. Search by Title/Name (Flexible)
```
Use case: "I'm looking for books with 'python' in the title"
Method: Case-insensitive substring matching (strstr)
Speed: Slower (O(n) — checks every book)
Result: All matching books displayed
```

### Analogy: Phone Book vs. Yellow Pages
- **Search by ID**: Like looking up someone in a **phone book** when you know their exact name.
- **Search by Title/Name**: Like browsing the **Yellow Pages** by category, or using Ctrl+F in a document.

### Recursive Search Implementation
The program includes `searchBookRecursive()` and `searchMemberRecursive()` functions that use **binary search** (a divide-and-conquer algorithm). However, note that the main user-facing search menus currently use the simpler linear search (strstr) for flexibility, as binary search would require the arrays to be perpetually sorted by ID.

### Analogy: Guessing a Number Game
You're thinking of a number between 1-100, I try to guess it.

```
Linear Search (slow):
  Is it 1? No. Is it 2? No. Is it 3? No...
  (Worst case: 100 guesses)

Binary Search (fast):
  Is it 50? Too high.
  Is it 25? Too low.
  Is it 37? Too high.
  Is it 31? Correct!
  (Worst case: 7 guesses)
```

---

## System Limits & Configuration

The system has predefined limits (defined as macros in `main.c`):

```
MAX_BOOKS: 500          // Maximum books in catalog
MAX_MEMBERS: 200        // Maximum registered members
MAX_TRANSACTIONS: 1000  // Maximum transaction records
MAX_BORROW_DAYS: 14     // Days before book is overdue
FINE_PER_DAY: 2.0       // Base fine (tier 1)
```

### Analogy: Restaurant Capacity
Think of these limits like a **restaurant**:
- **MAX_BOOKS** = Number of menu items you can offer
- **MAX_MEMBERS** = Number of loyalty card holders
- **MAX_TRANSACTIONS** = Number of orders you can track in your system
- **MAX_BORROW_DAYS** = How long you keep a reservation before canceling

If you exceed these limits, the system will display an error (like "Sorry, we're at capacity").

---

## Workflow Example: Complete User Journey

### Scenario: Adding a New Book and Issuing It

**Step 1: Admin adds a book**
```
1. Admin logs in (admin / admin123)
2. Selects "Book Management" → "Add New Book"
3. Enters details:
   - Title: "Python Programming"
   - Author: "John Smith"
   - ISBN: "978-1234567890"
   - Category: "Programming"
   - Total Copies: 3
   - Price: Rs. 500.00
4. System assigns Book ID: 1001
5. Book saved to books.dat
```

**Step 2: Admin registers a member**
```
1. Selects "Member Management" → "Add New Member"
2. Enters details:
   - Name: "Alice Johnson"
   - Email: "alice@example.com"
   - Phone: "123-456-7890"
   - Address: "123 Main St"
3. System assigns Member ID: 2001
4. Member saved to members.dat
```

**Step 3: Member borrows the book**
```
1. Admin selects "Transaction Management" → "Issue Book"
2. Enters Book ID: 1001
3. Enters Member ID: 2001
4. System creates transaction:
   - Transaction ID: 5001
   - Issue Date: 2025-11-02
   - Due Date: 2025-11-16 (14 days later)
5. Book's availableCopies: 3 → 2
6. Member's booksIssued: 0 → 1
7. Transaction saved to transactions.dat
```

**Step 4: Member returns the book (3 days late)**
```
1. Admin selects "Return Book"
2. Enters Transaction ID: 5001
3. System calculates:
   - Return Date: 2025-11-19
   - Days overdue: 3
   - Fine: 3 × Rs. 2.00 = Rs. 6.00
4. Book's availableCopies: 2 → 3
5. Member's booksIssued: 1 → 0
6. Member's totalFines: Rs. 0.00 → Rs. 6.00
7. Transaction marked as returned
```

---

## Summary of Key Concepts

| Feature | Real-World Analogy | Technical Implementation |
|---------|-------------------|--------------------------|
| **Book Management** | Library card catalog | Struct array in books.dat |
| **Member Management** | Membership card system | Struct array in members.dat |
| **Transactions** | Checkout desk records | Struct array in transactions.dat |
| **Fine Calculation** | Parking meter penalties | Tiered formula based on days overdue |
| **Persistent Storage** | Filing cabinets | Binary files (.dat) |
| **Search System** | Phone book + Yellow Pages | Direct lookup + substring matching |
| **Admin Access** | Master key | Username/password verification |
| **Member Access** | Visitor badge | Member ID verification |

---

## Next Steps for Learning

1. **Explore the Code**: Open `main.c` and look for these key functions:
   - `addBook()`, `issueBook()`, `returnBook()`
   - `calculateFine()`, `loadBooks()`, `saveBooks()`

2. **Experiment**: Try modifying the constants:
   - Change `MAX_BORROW_DAYS` from 14 to 7
   - Adjust the fine tiers in `calculateFine()`

3. **Extend the System**: Ideas for improvements:
   - Add book categories filtering
   - Email notifications for overdue books
   - Export reports to CSV
   - Add multiple admin accounts

---

**End of Feature Documentation**

For quick start instructions and compilation steps, see `README.md`.
