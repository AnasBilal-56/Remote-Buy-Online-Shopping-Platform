#include "Shoppingheader.h"
using namespace std;

// Function to create a new account
void CreateAccount(){
    cout << endl << "Create New Account" << endl;
    string username, password;
    fstream file("data.txt", ios::in | ios::out | ios::app);

    if (!file) {
        cout << "file not found!" << endl;
        return;}

    // Username
    cout << "Enter name (3-32 char): ";
    getline(cin, username);

    // Validation- No spaces, existing etc
    while (!isValidLength(username, 3, 32) || usernameExists(file, username)
           || username.find_first_of(" \t\n") != string::npos) {
        if (!isValidLength(username, 3, 32)) {
            cout << "Enter a valid length Username (3-32 characters): ";}
        else if (usernameExists(file, username)) {
            cout << "Username already exists! Enter a different Username (3-32 characters): ";
        } else if (username.find_first_of(" \t\n") != string::npos) {
            cout << "Username cannot contain white spaces. Enter a valid username: ";
        }
        getline(cin, username);
    }

    cout << "Username is available!" << endl;

    file.clear();
    file.seekp(0, ios::end);
    // Username is stored UNENCRYPTED
    writeLine(file, username);

    // Password
    cout << "Enter your password (3-32 characters): ";
    getline(cin, password);

    while (!isValidLength(password, 3, 32) || !isValidPassword(password)) {
        cout << endl << "Your password may only contain:" << endl;
        cout << "- Uppercase and Lower case letters: A-Z and a-z" << endl;
        cout << "- Numbers: 0�9" << endl;
        cout << "- Allowed symbols: ! # $ % & * - + = @ _ " << endl;
        cout << "Spaces and other characters are not allowed." << endl;
        cout << "Please enter a valid password (3-32 characters): ";
        getline(cin, password);
    }

    // Store encrypted password
    writeLine(file, encrypt(password));

    // Data lines (not encrypted)
    for (int i = 0; i < MAX_GAMES; i++) {
        writeLine(file, "0 0 0"); // initial wins, losses, highscore
    }

    file.close();

    cout << "Account created succesfully";
}

// Writes a line into the file
void writeLine(fstream &file, const string &text)
{
    file << text << "\n";
}

// Checks if a username already exists (usernames stored unencrypted)
bool usernameExists(fstream &file, const string &name)
{
    file.clear();
    file.seekg(0);

    string line;
    while (getline(file, line)) {
        if (line == name) {
            return true; // Username found
        }

        // Skip password line
        getline(file, line);

        // Skip all stats lines
        for (int i = 0; i < MAX_GAMES; i++) {
            getline(file, line);
        }
    }
    return false;
}

// Check string length
bool isValidLength(const string &str, int minLen, int maxLen)
{
    return (str.length() >= minLen && str.length() <= maxLen);
}

// Login function
bool login(string &usernameRef, string &passwordRef)
{
    string username, password;

    fstream file("data.txt", ios::in);
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

void changepassword(string username, string &password)
{
    cout << endl << "Change Account Password" << endl;

    string newPassword;
    cout << "Enter new password (3-32 characters): ";
    getline(cin, newPassword);

    while (!isValidLength(newPassword, 3, 32) || !isValidPassword(newPassword)) {
        cout << endl << "Your password may only contain:" << endl;
        cout << "- Uppercase and Lower case letters: A-Z and a-z" << endl;
        cout << "- Numbers: 0�9" << endl;
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
    while (getline(infile, line)) {
        temp << line << "\n";

        if (line == username) {
            // Skip old password and write encrypted new password
            if (getline(infile, line)) {
                temp << encrypt(newPassword) << "\n";
            }

            for (int i = 0; i < 2; i++) {
                if (getline(infile, line)) {
                    temp << line << "\n";
                }
            }
        }
    }

    infile.close();
    temp.close();

    remove("data.txt");
    rename("temp.txt", "data.txt");

    password = newPassword;

    cout << "Password changed successfully!\n";
}

// Encrypt (simple shift)
// Note: for encryption and decryption unsigned char is not needed as max range(127) is less than 128
string encrypt(string text)
{
    for (int i = 0; i < text.length(); i++)
        text[i] = char(int(text[i]) + 5);
    return text;
}

// Decrypt
string decrypt(string text)
{
    for (int i = 0; i < text.length(); i++)
        text[i] = char(int(text[i]) - 5);
    return text;
}

bool isValidPassword(const string &s)
{
    for (char c : s) {
        // some compilors store ascii values from -128 to -127
        // Using unsigned char cariable makes sure that the value is unsigned(0 to 255)
        // Remove logic errors in range comparisons
        unsigned char ascii = (unsigned char) c;

        // Reject space and anything outside printable ASCII (33�122)
        if (ascii < 33 || ascii > 122)
            return false;

        // Allowed: A�Z
        if (ascii >= 'A' && ascii <= 'Z')
            continue;

        // Allowed: a�z
        if (ascii >= 'a' && ascii <= 'z')
            continue;

        // Allowed: 0�9
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
