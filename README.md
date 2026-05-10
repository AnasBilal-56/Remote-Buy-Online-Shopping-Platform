# Online Shopping Cart - Object-Oriented Programming Project

## Group Members

- Muhammad Anas Bilal (25L-0778)
- Ali Khan (25L-0848)
- Osama Bin Tahir (25L-0689)
- Muhammad Hamdaan (25L-0911)

## Project Description & Features

The **Online Shopping Cart System** is a UI-based console program developed using Object-Oriented Programming concepts in C++. It simulates the basic functionality of an online store, allowing users to browse products, manage a shopping cart, and complete purchases.

### Key Features

- Browse and view products easily
- Add/remove items from the shopping cart
- Checkout process with purchase history saving
- Automatic total cost calculation (including taxes/discounts)
- User authentication / login system
- Simple encrypted password storage
- Admin account with elevated privileges
- Search products by category or criteria
- Automated stock updates and validation on purchase
- User-friendly menu interface

## Files Included

### Source Code
- `Main.cpp` — Entry point, menu system, program initialization
- `project.h` — Function declarations, constants, libraries
- `customer_management.cpp` — Login, register, encryption, password management
- `manager.cpp` — Database integrity, loyalty points, item management tools
- `CMakeLists.txt` — Build configuration

### Data Files (Must be in project directory)
- `Logindata.txt` — User credentials, cart history, loyalty status
- `itemCatalogue array` — Product database (stock, categories, prices)

## Setup & How to Run

1. Open the project in **Visual Studio**.
2. Set configuration to **Debug** or **Release** and platform to **x64** (or x86).
3. Press `Ctrl + F5` (Start Without Debugging) to build and run.
4. On first run, the program will perform an integrity check on the data files.

> **Note**: Make sure `Logindata.txt` and othe required files are present in the project directory.

## Admin / Manager Features

**Hidden Maintenance Menu:**

1. Select **Login** from the main menu.
2. Use the following credentials:
    - **Username**: `Manager`
    - **Password**: `123456789`

### Available Admin Options
- Edit item stocks
- Change prices
- Add/remove items and categories

---

**Project Status**: README is a work in progress — update file list and setup instructions as needed.