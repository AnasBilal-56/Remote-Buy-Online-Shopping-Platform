#include "Shoppingheader.h"
using namespace std;

bool CheckCustomerdataintegrity() {
    ifstream file("Logindata.txt");
    if (!file) {
        cout << "Error: cannot open player data file.\n";
        return false;
    }

    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        string username = line;
        if (username.empty()) {
            cout << "Error: empty username at line " << lineNum << endl;
            return false;
        }

        // Read Encrypted Password
        if (!getline(file, line)) {
            cout << "Error: missing password for user " << username << endl;
            return false;
        }
        lineNum++;
        string passwordEnc = line;
        if (passwordEnc.empty()) {
            cout << "Error: empty password for user " << username << endl;
            return false;
        }

        // Read Loyalty
        if (!getline(file, line)) {
            cout << "Error: missing loyalty value for user " << username << endl;
            return false;
        }
        lineNum++;
        string loyaltyStr = line;
        if (loyaltyStr.empty()) {
            cout << "Error: empty loyalty field for user " << username << endl;
            return false;
        }
        int loyalty;
        istringstream iss(loyaltyStr);
        if (!(iss >> loyalty) || !iss.eof()) {
            cout << "Error: invalid loyalty value (must be integer) at line "
                 << lineNum << " for user " << username << endl;
            return false;
        }
    }

    cout << "Player data integrity check passed.\n";
    return true;
}

// bool checkHighscoreIntegrity(int maxGames) {
//     ifstream file("highscore.txt");
//     if (!file) {
//         cout << "Error: cannot open highscore file.\n";
//         return false;
//     }
//
//     string line;
//     for (int g = 0; g < maxGames; g++) {
//         // header line
//         if (!getline(file, line)) {
//             cout << "Error: missing header for game " << g + 1 << endl;
//             return false;
//         }
//
//         for (int i = 0; i < 10; i++) {
//             if (!getline(file, line)) {
//                 cout << "Error: missing score line " << i + 1
//                     << " for game " << g + 1 << endl;
//                 return false;
//             }
//             int score;
//             string uname;
//             istringstream iss(line);
//             if (!(iss >> score >> uname) || uname.empty()) {
//                 cout << "Error: invalid score line format at game " << g + 1
//                     << ", line " << i + 1 << endl;
//                 return false;
//             }
//         }
//     }
//
//     cout << "Highscore file integrity check passed.\n";
//     return true;
// }

/*the above function can be remade for our project e.g shoppinghistory.txt etc
otherwise remove it*/

void ManagerMenu() {
    int option;
    while (true) {
        system("cls");
        cout << "===== DEBUG MENU =====\n";
        cout << "1. restock items\n";
        cout << "2. Check Customer Data Integrity\n";
        cout << "3. add/remove items from catalogue\n";
        cout << "4. Exit Debug Menu\n";
        cout << "======================\n";
        cout << "Enter option: ";

        if (!(cin >> option)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Press Enter to continue...";
            cin.get();
            cin.get();
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline

        switch (option) {
        case 1: {
            initialisegame();
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }

        case 2: {
            if (CheckCustomerdataintegrity())
                cout << "Player data is OK.\n";
            else
                cout << "Player data has errors.\n";
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }
        case 3: {

            break;
        }
        case 4:
            return; // exit menu

        default: {
            cout << "Invalid option. Press Enter to continue...";
            cin.get();
            break;
        }
        }
    }
}