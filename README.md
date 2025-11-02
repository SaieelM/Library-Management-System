# Library Management System (C Language) – CLI Version

A comprehensive, fully-featured library management software for educational or small library use. Built using C programming concepts: functions, recursion, arrays, pointers, strings, structures, and file handling. Entirely CLI-based – no GUI, runs in any standard C console.

***

## Features

- **Admin and Member Login** (with role-based access).
- **Book Management:** Add, View, Search, Edit, Delete.
- **Member Management:** Register, View, Search, Edit, Delete.
- **Issue and Return Books** (with fine calculation).
- **Persistent Storage:** All data saved to files (`.dat`), survives restart.
- **Recursive and Binary Search** (for books/members).
- **Statistics:** Real-time data summary for admin.
- **User-friendly Menus** and error feedback.

***

## Installation & Compilation

### Requirements

- GCC Compiler (Linux/Windows/Mac)
- Standard C library

### Compile

Linux/macOS:
```bash
gcc -o library_system library_system.c
./library_system
```

Windows (MinGW):
```cmd
gcc -o library_system.exe library_system.c
library_system.exe
```

If using Code::Blocks:
- Create a new C console project
- Paste the code
- Build and run

***

## Usage Instructions

1. **Start the Program**
   - Launch from terminal: `./library_system` (Linux/Mac) or `library_system.exe` (Windows)

2. **Admin Login**
   - Username: `admin`
   - Password: `admin123`

3. **Member Access**
   - Enter your **Member ID** after registration (no password needed)

4. **Main Menus**
   - Admin: Manage books, members, issue/return books, view statistics
   - Member: View issued books, history, search library

5. **Data Storage**
   - Data files: `books.dat`, `members.dat`, `transactions.dat`, `admin.dat`
   - Delete these to reset the system

***

## Credential Details

### Admin Account  
- **Username:** `admin`  
- **Password:** `admin123`

### Member Accounts  
- Register new members from Admin dashboard  
- System auto-generates **Member IDs** (starting from 2001)  
- Log in as member using assigned Member ID

***

## Sample Interaction

```
╔══════════════════════════════════╗
║    LIBRARY MANAGEMENT SYSTEM     ║
╚══════════════════════════════════╝
Main Menu:
1. Admin Login
2. Member Portal
3. Exit
```

***

## Troubleshooting

- **Compile errors:** Ensure all brackets and file includes are intact. Use GCC version 7+ if possible.
- **File errors:** Check permissions if running from restricted folders.
- **Input stuck:** Always hit Enter twice after prompts if input is not accepted.

***

## Advanced Usage & Extensions

- Supports up to 500 books, 200 members, 1000 transactions
- Can be extended for email notification, PDF exports, multiple admin accounts, book categories, etc.

***

## License

This project is free and open-source for educational and non-commercial use.

***

**For any queries or help, please refer to the project documentation or contact the developer.**
