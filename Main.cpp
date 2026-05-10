#include "Shoppingheader.h"
using namespace std;

string user1;
string password1;
bool won; //remove?

int main() {
    Catalogue ItemCatalogue;
    Product P1;
    // ItemCatalogue.addProduct();

    // Initial Pseudo Loading screen For file Integrity Checks
    cout << "Running integrity checks.";
    this_thread::sleep_for(chrono::milliseconds(750)); 
    cout << ".";
    this_thread::sleep_for(chrono::milliseconds(750)); 
    cout << "." << endl;

    if (!CheckCustomerdataintegrity()) {
        cout << REDC << "Customer data file corrupted! Please fix or restore backup.\n" << RESET;
        return 1;
    }

    // if (!checkHighscoreIntegrity(MAX_GAMES)) {
    //     cout << REDC << "Highscore file corrupted! Please fix or restore backup.\n" << RESET;
    //     return 1;
    // }                     //replace with check on other files e.g history.txt otherwise remove



    this_thread::sleep_for(chrono::milliseconds(1000)); // 1-second delay

    // ------------ Main Menu Start ------------
    int option;
    while (true) {
        clearScreen();
        cout << endl << "MAIN MENU: \n" <<"1. Create New account\n";
        cout << "2. Login\n" <<"3. Exit\n"<<"Enter Option number to select: ";
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Clearing Input Buffer Incase multiple Inputs made by User

        if (cin.fail()) {   // Input handling in case User entered Invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input";
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }

        switch (option) {
        case 1: {      
            CreateAccount();
            this_thread::sleep_for(chrono::seconds(1));
            break;
        }

        case 2: {
            // Calling login If Login Successful Opens player Menu
            if (login(user1, password1)) {
                if (user1 == "Admin") {
                    ManagerMenu();
                    break;
                }
                CustomerMenu(user1, password1);
                cout << endl << "Logging out";
                this_thread::sleep_for(chrono::milliseconds(350));
                cout << ".";
                this_thread::sleep_for(chrono::milliseconds(350));
                cout << ".";
                this_thread::sleep_for(chrono::milliseconds(350));
                cout << ".";
                this_thread::sleep_for(chrono::milliseconds(350));
                break;
            }
            else {
                cout << "Login Unsuccessful";
                this_thread::sleep_for(chrono::seconds(2));
            }
            break;
        }

        case 3: {
            char in;
            while (true) {

                cout << "Are you sure? (Y/N)";
                cin >> in;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (in == 'Y' || in == 'y') {
                    system("cls");
                    cout << "Exiting program.";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    clearScreen();
                    cout << "Exiting program..";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    clearScreen();
                    cout << "Exiting program...";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    clearScreen();
                    cout << "";
                    return 1;
                }
                else if (in == 'N' || in == 'n') break;
                else cout << "Enter a valid input" << endl;
            }
            break;
        }

        default: {
            cout << "InValid choice";
            this_thread::sleep_for(chrono::seconds(2));
        }

        }
    }
}


// Player Menu
// Stores username and Password for further function(Updating statistics and highscores)
void CustomerMenu(string& username, string& password) {
    int option;
    bool logout = false;

    while (!logout) {
        // Clearing Window Before each Display
        clearScreen();
        cout << "CUSTOMER MENU:\n";
        cout<<"1. Change Password\n"<< "2. Logout\n";
        cout<<"Enter option: ";
        cin >> option;          //^  FULLY EXANDABLE PLEASE EXPAND WITH YOUR FUNCTIONS
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (cin.fail()) {        // Input Validation
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option";
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        switch (option) { //{PASS CHNAGE
        case 1: {
            changepassword(username,password);
            break;
        }

        case 2: { //LOGOUT
            username = ""; password = "";
            logout = true;
            break;
        }

        default:
            cout << "Invalid option\n";
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}