//
// Created by ali on 29/04/2026.
//

#ifndef REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_SHOPPINGHEADER_H
#define REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_SHOPPINGHEADER_H

#endif //REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_SHOPPINGHEADER_H
#pragma once

#include <algorithm>
#include <chrono>  //for timer with steady_clock
#include <cstdlib> //For rand() & srand()
#include <cstring>
#include <ctime> //for time()
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread> // Needed for sleep_for
using namespace std;

// --- Minesweeper Constants ---
const int ROWS = 10, COLS = 10,
          MINS = 10; // sizw of game board & num mines is fixed so constant difficulty

// --- Constants ---
const int MAX_GAMES = 5; // e.g., 0 = Wordle, 1 = minesweeper, 3 = tic tac toe;

// --- Account Management Functions ---
void CreateAccount();
void changepassword(string username, string &password);
bool login(string &usernameRef, string &passwordRef);

// --- Player Menu Function
void PlayerMenu(string &username, string &password);

// --- File/String Utilities ---
void writeLine(fstream &file, const string &text);
bool usernameExists(fstream &file, const string &name);
bool isValidLength(const string &str, int minLen, int maxLen);
bool isValidPassword(const string &s);

// --- Simple Encryption ---
string encrypt(string text);
string decrypt(string text);

// --- Score Saving ---
void updateStats(string username, int gameIndex, bool won, double score);
void Highscore(string username, int inscore, int gameIndex);
int disphighcore();
void showStats(string username);

// --- Scalability/Debug ---
void initialisegame();
bool Checkplayerdataintegrity();
bool checkHighscoreIntegrity(int maxGames);
void DEBUGMENU();

// --- COLORS ---
#define REDC "\033[31m"
#defplayer_management.cppine GREENC "\033[32m"
#define YELLOWC "\033[33m"
#define BLUEC "\033[34m"
#define CYANC "\033[36m"
#define MAGENTAC "\033[35m"
#define LIGHTGREENC "\033[92m"
#define LIGHTREDC "\033[91m"

// ANSI color codes
#define RESET "\033[0m"
#define GREEN_BG "\033[42m"   // Green background
#define YELLOW_BG "\033[43m"  // Yellow background
#define GRAY_BG "\033[100m"   // Dark gray background
#define BLACK_TEXT "\033[30m" // Black letters
