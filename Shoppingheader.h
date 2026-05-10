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

// --- Account Management Functions ---
void CreateAccount();
void changepassword(string username, string &password);
bool login(string &usernameRef, string &passwordRef);

// --- Menu Function
void CustomerMenu(string &username, string &password);

// --- File/String Utilities ---
void writeLine(fstream &file, const string &text);
bool usernameExists( const string &name);
bool isValidLength(const string &str, int minLen, int maxLen);
bool isValidPassword(const string &s);
void clearScreen();

// --- Simple Encryption ---
string encrypt(string text);
string decrypt(string text);

// --- Score Saving ---   ////replace with cart history?
void updateStats(string username, int gameIndex, bool won, double score);
void Highscore(string username, int inscore, int gameIndex);
int disphighcore();
void showStats(string username);

// --- Scalability/Debug ---
void initialisegame();  //replace with additem/restock?
bool CheckCustomerdataintegrity();  //remove?
bool checkHighscoreIntegrity(int maxGames);  //^
void ManagerMenu();


// -----CLASSES------
class Product{
private:
    int id, stock; double price;
    string name, category;

public:
    Product(){id = 0,stock =0, price =0,name = "",category="";}
    Product(int i,int s, string n, double p, string c) {
        id = i,name = n, price = p, category =c, stock = s;
    }

    // void Display() const { cout << id << "|" << name << "|" << price << "|" << endl; } //useless?
    string getname() const { return name; }
    int getId() const { return id; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }
    int getStock() const { return stock; }

    void modifyName(string name) { this->name = name; }
    void modifyPrice(double price) { this->price = price; }
    void modifyStock(int stock) { this->stock = stock; }
};

class Catalogue{
private:
    Product *products; int size;

public:
    Catalogue(){ size = 0; products = new Product[100];}
    void addProduct(Product p){
        if (size < 100) {products[size++] = p;}
        else {cout << "Catalogue is Full!" << endl;}   ///TERMINAL OUTPUT CHANGE TO QT UI PLS
    }

    int getsize() const { return size; }
    bool isempty() const { return size == 0; }

    // void showProducts(){
    //     for (int i = 0; i < size; i++) {products[i].Display();}
    // }

    Product getProduct(int id){
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id)
                return products[i];
        }
    }

    Product getProductByName(string name){
        for (int i = 0; i < size; i++) {
            if (products[i].getname() == name)
                return products[i];
        }
    }

    bool ProductExists(int id){
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id)
                return true;
        }
        return false;
    }

    void removeProduct(int id){
        int index = -1;
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Product Not Found" << endl;
            return;
        }

        for (int i = index; i < size; i++) {products[i] = products[i + 1];}

        size--;
        cout << "Product Removed Successfully" << endl;
    }

    void UpdateProduct(int id, string name, double price){
        bool found = false;
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id) {
                products[i].modifyName(name);
                products[i].modifyPrice(price);
                cout << "Product Updated Successfully" << endl;
                found = true;
                break;
            }
        }

        if (!found) {cout << "Product Not Found" << endl;}
    }

    Product getProductAtIndex(int index){if (index >= 0 && index < size){return products[index];}}
    ~Catalogue() { delete[] products; }
};


// --- COLORS ---
#define REDC "\033[31m"
#define GREENC "\033[32m"
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
