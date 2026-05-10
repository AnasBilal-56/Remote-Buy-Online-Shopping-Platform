#include "Shoppingheader.h"
using namespace std;

// Function to create a new account
void CreateAccount() {
    cout << "\nCreate New Account\n";
    string username, password;
    // === VALIDATION PHASE (read only) ===
    while (true) {
        cout << "Enter name (3-32 char): ";
        getline(cin, username);

        if (!isValidLength(username, 3, 32) ||
            username.find_first_of(" \t\n") != string::npos) {
            cout << "Invalid username! (3-32 chars, no whitespace)\n";
            continue;
            }

        if (usernameExists(username)) {   // pass string, not fstream
            cout << "Username already exists! Try another.\n";
            continue;
        }

        break;
    }

    cout << "Username is available!\n";

    // === WRITING PHASE ===
    fstream file("Logindata.txt", ios::out | ios::app);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    // Make sure we start on a new line
    file << username << '\n';
    // file.seekp(0, ios::end);   // not needed with app mode

    cout << "Enter your password (3-32 characters): ";
    getline(cin, password);

    while (!isValidLength(password, 3, 32) || !isValidPassword(password)) {
        cout << "\nInvalid password!\n"
             << "Allowed: A-Z a-z 0-9 ! # $ % & * - + = @ _\n"
             << "No spaces or other characters.\n"
             << "Enter valid password: ";
        getline(cin, password);
    }

    string encryptedPass = encrypt(password);
    file << encryptedPass << '\n';
    file << "0\n";                    // loyalty points

    file.close();
    cout << "Account created successfully!\n";
}
// Writes a line into the file
void writeLine(fstream &file, const string &text){file << text << "\n";}

// Checks if a username already exists (usernames stored unencrypted)
bool usernameExists(const string& name) {
    ifstream file("Logindata.txt");
    string line;
    while (getline(file, line)) {if (line == name) return true;}
    return false;
}

// Check string length
bool isValidLength(const string &str, int minLen, int maxLen) {
    return (str.length() >= minLen && str.length() <= maxLen);
}

// Login function
bool login(string &usernameRef, string &passwordRef){
    string username, password;

    fstream file("Logindata.txt", ios::in);
    if (!file) {
        cout << "Error opening file!" << endl;
        return false;
    }
    cout << endl << "LOGIN" << endl;

    cout << "Enter your Username (3-32 characters): ";
    getline(cin, username);

    while (!isValidLength(username, 3, 32)) {
        cout << "Enter a valid length Username (3-32 characters): ";
        getline(cin, username);
    }

    file.clear();
    file.seekg(0);

    string line;
    bool foundUser = false;
    string storedPasswordEncrypted;

    while (getline(file, line)) {
        if (line == username) {
            foundUser = true;

            if (!getline(file, storedPasswordEncrypted)) {
                cout << "Error: user data corrupted." << endl;
                file.close();
                return false;
            }
            break;
        }
    }

    if (!foundUser) {
        cout << "Username does not exist!" << endl;
        file.close();
        return false;
    }

    // Ask password
    cout << "Enter your Password (3-32 characters): ";
    getline(cin, password);

    while (!isValidLength(password, 3, 32)) {
        cout << "Enter a valid length Password (3-32 characters): ";
        getline(cin, password);
    }

    file.close();
    // DECRYPT before comparing
    string storedPassword = decrypt(storedPasswordEncrypted);

    if (password == storedPassword) {
        cout << "Login successful!" << endl;
        usernameRef = username;
        passwordRef = password;
        return true;
    } else {
        cout << "Incorrect password!" << endl;
        return false;
    }
}

void changepassword(string username, string &password){
    cout << endl << "Change Account Password" << endl;

    string newPassword;
    cout << "Enter new password (3-32 characters): ";
    getline(cin, newPassword);

    while (!isValidLength(newPassword, 3, 32) || !isValidPassword(newPassword)) {
        cout << endl << "Your password may only contain:" << endl;
        cout << "- Uppercase and Lower case letters: A-Z and a-z" << endl;
        cout << "- Numbers: 0-9" << endl;
        cout << "- Allowed symbols: ! # $ % & * - + = @ _ " << endl;
        cout << "Spaces and other characters are not allowed." << endl;
        cout << "Please enter a valid password (3-32 characters): ";
        getline(cin, newPassword);
    }

    ifstream infile("data.txt");
    ofstream temp("temp.txt");

    if (!infile || !temp) {
        cout << "Error opening files.\n";
        return;
    }

    string line;
    bool accountFound = false;
    while (getline(infile, line)) {
        temp << line << "\n";

        if (line == username) {
            accountFound = true;

            // Skip old password line
            if (getline(infile, line)) {
                // Write new encrypted password
                temp << encrypt(newPassword) << "\n";
            }

            // Copy only the Loyalty line (1 line only)
            if (getline(infile, line)) {
                temp << line << "\n";
            }
        }
    }

    infile.close();
    temp.close();

    if (!accountFound) {
        cout << "Account not found!\n";
        remove("temp.txt");
        return;
    }

    // Replace original file
    remove("Logindata.txt");
    rename("temp.txt", "data.txt");
    password = newPassword;   // Update the reference
    cout << "Password changed successfully!\n";
}

// Encrypt (simple shift)
// Note: for encryption and decryption unsigned char is not needed as max range(127) is less than 128
string encrypt(string text){
    for (int i = 0; i < text.length(); i++)
        text[i] = char(int(text[i]) + 5);
    return text;
}

// Decrypt
string decrypt(string text){
    for (int i = 0; i < text.length(); i++)
        text[i] = char(int(text[i]) - 5);
    return text;
}

bool isValidPassword(const string &s){
    for (char c : s) {
        // some compilors store ascii values from -128 to -127
        // Using unsigned char cariable makes sure that the value is unsigned(0 to 255)
        // Remove logic errors in range comparisons
        unsigned char ascii = (unsigned char) c;

        // Reject space and anything outside printable ASCII (33-122)
        if (ascii < 33 || ascii > 122)
            return false;

        // Allowed: -�Z
        if (ascii >= 'A' && ascii <= 'Z')
            continue;

        // Allowed: a-z
        if (ascii >= 'a' && ascii <= 'z')
            continue;

        // Allowed: 0-9
        if (ascii >= '0' && ascii <= '9')
            continue;

        // Allowed limited symbols:
        // 33  !     (basic symbol)
        // 35  #     (common safe symbol)
        // 36  $     (common safe symbol)
        // 37  %     (common safe symbol)
        // 38  &     (common safe symbol)
        // 42  *     (common safe symbol)
        // 45  -     (safe hyphen)
        // 43  +     (common text symbol)
        // 61  =     (safe)
        // 64  @     (email-like)
        // 95  _     (underscore, often used in passwords)
        const string allowedSymbols = "!#$%&*-+=@_";

        // npos means not found
        if (allowedSymbols.find(c) != string::npos)
            continue;

        // If it wasn't in any allowed range:
        return false;
    }

    // If loops through succesfully without issue return true
    return true;
}
