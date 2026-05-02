#include "project.h"
using namespace std;

string player1;
string password1;
bool won;

int main() {
    // Initial Pseudo Loading screen For file Integrity Checks
    cout << "Running integrity checks.";
    this_thread::sleep_for(chrono::milliseconds(750)); 
    cout << ".";
    this_thread::sleep_for(chrono::milliseconds(750)); 
    cout << "." << endl;

    if (!Checkplayerdataintegrity()) {
        cout << REDC << "Player data file corrupted! Please fix or restore backup.\n" << RESET;
        return 1;
    }

    if (!checkHighscoreIntegrity(MAX_GAMES)) {
        cout << REDC << "Highscore file corrupted! Please fix or restore backup.\n" << RESET;
        return 1;
    }
    this_thread::sleep_for(chrono::milliseconds(1000)); // 1-second delay

    // ------------ Main Menu Start ------------
    int option;

    while (true) {

        system("cls");

        cout << endl << "MAIN MENU: " << endl;
        cout << "1. Create New account" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter Option number to select: ";
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
            if (login(player1, password1)) {
                if (player1 == "Debug") {
                    DEBUGMENU();
                    break;
                }
                PlayerMenu(player1, password1);
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
                    system("cls");
                    cout << "Exiting program..";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
                    cout << "Exiting program...";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
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
// Allows player to play games, check stats and change account password.
// Stores player username and Password for further function(Updating statistics and highscores)
void PlayerMenu(string& username, string& password) {
    int option;
    bool logout = false;

    while (!logout) {
        // Clearing Window Before each Display
        system("cls");
        cout << "PLAYER MENU:\n";
        cout << "1. Change Password\n";
        cout << "2. Play Wordle\n";
        cout << "3. Play Minesweeper\n";
        cout << "4. Play Tic Tac Toe\n";
        cout << "5. Play Rock Paper Scissors\n";
        cout << "6. Play Casino Simulator\n";
        cout << "7. Check Player Stats\n";
        cout << "8. Check Highscore\n";
        cout << "9. Logout\n";
        cout << "Enter option: ";
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        // Input Validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option";
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        switch (option) {
        case 1: {
            changepassword(username,password);
            break;
        }

        // ------------ Wordle ------------
        case 2: {
            int wordleScore = PlayWordle();
            if (wordleScore != 0) {
                bool won = wordleScore > 0;
                updateStats(username, 0, won, wordleScore);
            }
            cout << "Thanks for playing!\n";
            string n;
            cout << "Press Enter to Exit";
            getline(cin, n);
            break;
        }

        // ------------ Minesweeper ------------
        case 3: {
            int minesweeperscore = Minesweeper(won);
            updateStats(username, 1, won, minesweeperscore);
            cout << "Thanks for playing!\n";
            string n;
            cout << "Press Enter to Exit";
            cin.ignore();
            getline(cin, n);
            break;
        }

        // ------------ Tic Tac Toe ------------
        case 4: {
            bool ticwin;
            int score = playtictactoe(ticwin,username);
            if(score != 0) updateStats(username, 2, ticwin, score);
            string n;
            cout << endl << "Press Enter to Exit";
            getline(cin, n);
            break;
        }

        // ------------ Rock Paper Scissors ------------
        case 5: {
            int score;
            bool win;
            score = RPS5(win);
            if (win) {
                updateStats(username, 3, 1, score);
            }
            else updateStats(username, 3, 0, score);
            cin.ignore();
            string n;
            cout << endl << "Press Enter to Exit";
            getline(cin, n);
            break;
        }

        // ------------ Casino ------------
        case 6: {
            int score;
            score = casino(username);
            bool won = score > 0;
            updateStats(username,4,won,score);
            cin.ignore();
            string n;
            cout << endl << "Press Enter to Exit";
            getline(cin, n);
            break;
        }

        // ------------ Player STATS ------------
        case 7: {
            showStats(username);
            string n;
            cout << "Press Enter to Exit";
            getline(cin, n);
            break;
        }

        // ------------ Game Highscores ------------
        case 8: {
            cout << endl << "Highscores" << endl;
            if (!disphighcore()) cout << "Error Accessing Highscores";
            string n;
            cout << endl << "Press Enter to Exit";
            getline(cin, n);
            break;
        }

        // ------------ LogOut ------------
        case 9: {
            username = "";
            password = "";
            logout = true;
            break;
        }
        default:
            cout << "Invalid option\n";
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}