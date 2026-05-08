#include "Shoppingheader.h"
using namespace std;
//start
void initialisegame() {
    ifstream infile("data.txt");
    ofstream temp("temp.txt");

    if (!infile || !temp) {
        cout << "Error opening files.\n";
        return;
    }

    string line;

    while (getline(infile, line)) {
        temp << line << "\n";            // username

        if (!getline(infile, line)) break;
        temp << line << "\n";            // encrypted password

        // Copy existing game data
        for (int i = 0; i < MAX_GAMES; i++) {
            if (getline(infile, line))
                temp << line << "\n";
        }

        // Add NEW game defaults
        temp << "0 0 0\n";
    }

    infile.close();
    temp.close();

    remove("data.txt");
    rename("temp.txt", "data.txt");


    // Now Initialising the global Highscores of the new game in Highscore file
    cout << "New game initialized for all players!" << endl;

    ofstream file("highscore.txt", ios::app);  // append mode

    if (!file) {
        cout << "Error opening highscore.txt!\n";
        return;
    }

    string gamename;
    cout << "Enter the name of the new game:" << endl;
    getline(cin, gamename);

    // Write header
    file << gamename << "\n";

    // Write 10 default highscore entries
    for (int i = 0; i < 10; i++) {
        file << "0 ---" << endl;    // score 0, username placeholder (---)
    }

    file.close();

    cout << "Highscore initialized for new game: " << gamename << endl;
}

bool Checkplayerdataintegrity() {
    ifstream file("data.txt");
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

        // Check data lines
        for (int i = 0; i < MAX_GAMES; i++) {
            if (!getline(file, line)) {
                cout << "Error: missing data line " << i + 1
                    << " for user " << username << endl;
                return false;
            }
            lineNum++;
            int w, l, h;
            istringstream iss(line);
            if (!(iss >> w >> l >> h)) {
                cout << "Error: invalid data line format at line " << lineNum << endl;
                return false;
            }
        }
    }

    cout << "Player data integrity check passed.\n";
    return true;
}

bool checkHighscoreIntegrity(int maxGames) {
    ifstream file("highscore.txt");
    if (!file) {
        cout << "Error: cannot open highscore file.\n";
        return false;
    }

    string line;
    for (int g = 0; g < maxGames; g++) {
        // header line
        if (!getline(file, line)) {
            cout << "Error: missing header for game " << g + 1 << endl;
            return false;
        }

        for (int i = 0; i < 10; i++) {
            if (!getline(file, line)) {
                cout << "Error: missing score line " << i + 1
                    << " for game " << g + 1 << endl;
                return false;
            }
            int score;
            string uname;
            istringstream iss(line);
            if (!(iss >> score >> uname) || uname.empty()) {
                cout << "Error: invalid score line format at game " << g + 1
                    << ", line " << i + 1 << endl;
                return false;
            }
        }
    }

    cout << "Highscore file integrity check passed.\n";
    return true;
}

void DEBUGMENU() {
    int option;
    while (true) {
        system("cls");
        cout << "===== DEBUG MENU =====\n";
        cout << "1. Initialize New Game\n";
        cout << "2. Check Player Data Integrity\n";
        cout << "3. Check Highscore Integrity\n";
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
            if (Checkplayerdataintegrity())
                cout << "Player data is OK.\n";
            else
                cout << "Player data has errors.\n";
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }
        case 3: {
            if (checkHighscoreIntegrity(MAX_GAMES))
                cout << "Highscore file is OK.\n";
            else
                cout << "Highscore file has errors.\n";
            cout << "Press Enter to continue...";
            cin.get();
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