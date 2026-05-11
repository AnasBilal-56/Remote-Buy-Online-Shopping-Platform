#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ADMININVENTORY.h"
#include <QMessageBox>
#include <QApplication>
#include <fstream>
#include <string>
#include <QInputDialog>
#include <vector>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0); // Start at Main Menu

    initializeTestProducts();
    setupShoppingPages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ====================== INITIALIZATION ======================
void MainWindow::initializeTestProducts()
{
   m_catalogue.loadFromFile("inventory.txt");
}
    void MainWindow::setupShoppingPages()
{
    m_gallery = new ShoppingGallery(&m_catalogue, &m_cart, this);
    m_cartPage = new CartPage(&m_cart, &m_catalogue, &m_fm, currentUser, this);
    m_orderHistoryPage = new OrderHistoryPage(&m_fm, this);

    ui->stackedWidget->addWidget(m_gallery);
    ui->stackedWidget->addWidget(m_cartPage);
    ui->stackedWidget->addWidget(m_orderHistoryPage);

    // Gallery connections...
    connect(m_gallery, &ShoppingGallery::goToCart, this, &MainWindow::goToCartSlot);
    connect(m_gallery, &ShoppingGallery::goBackToMenu, this, &MainWindow::goBackToCustomerMenu);

    // CartPage connections
    connect(m_cartPage, &CartPage::goToCatalogue, this, &MainWindow::goToCatalogueSlot);
    connect(m_cartPage, &CartPage::goToOrderHistory, this, &MainWindow::goToHistorySlot);

    // NEW: Checkout connection
    connect(m_cartPage, &CartPage::goToCheckout, this, &MainWindow::goToCheckoutSlot);
}

// ====================== NAVIGATION ======================
void MainWindow::goToCatalogueSlot()
{
    if (m_gallery) m_gallery->refreshGallery();
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::goToCartSlot()
{
    if (m_cartPage) m_cartPage->refreshTable();
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::goToHistorySlot()
{
    if (m_orderHistoryPage) m_orderHistoryPage->refreshOrders();
    ui->stackedWidget->setCurrentIndex(6);
}

// ====================== LOGIN / ACCOUNT ======================
void MainWindow::on_btnLoginMenu_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
    ui->txtUsername->clear();
    ui->txtPassword->clear();
}

void MainWindow::on_btnCreateAccountMenu_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
    ui->txtNewUsername->clear();
    ui->txtNewPassword->clear();
}

void MainWindow::on_btnBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnBackCreate_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnExit_clicked() {
    QApplication::quit();
}

void MainWindow::on_btnLogout_clicked() {
    currentUser.clear();
    currentPassword.clear();
    ui->stackedWidget->setCurrentIndex(0);
    QMessageBox::information(this, "Logout", "Logged out successfully.");
}

// ====================== LOGIN LOGIC ======================
void MainWindow::on_btnLoginSubmit_clicked()
{
    std::string username = ui->txtUsername->text().toStdString();
    std::string password = ui->txtPassword->text().toStdString();

    if (!isValidLength(username, 3, 32) || !isValidLength(password, 3, 32)) {
        QMessageBox::warning(this, "Error", "Username and Password must be 3-32 characters.");
        return;
    }

    std::ifstream file("Logindata.txt");
    if (!file) {
        QMessageBox::critical(this, "Error", "No database found!");
        return;
    }

    std::string line, storedPassEnc;
    bool foundUser = false;

    while (getline(file, line)) {
        if (line == username) {
            if (getline(file, storedPassEnc)) {
                foundUser = true;
            }
            break;
        }
        getline(file, line); // skip password
        getline(file, line); // skip loyalty
    }
    file.close();

    if (foundUser && password == decrypt(storedPassEnc)) {
        currentUser = QString::fromStdString(username);

        if (username == "Admin") {
            AdminInventory *adminWin = new AdminInventory();
            adminWin->loginWindowPtr = this;

            // Connect signal
            connect(adminWin, &AdminInventory::inventoryChanged,
                    this, &MainWindow::refreshCatalogue);

            adminWin->show();
            this->hide();
        }else {
            currentUser = QString::fromStdString(username);
            ui->stackedWidget->setCurrentIndex(2);   // Go to Customer Menu (Index 2)
        }
    } else {
        QMessageBox::warning(this, "Error", "Invalid username or password.");
    }
}

// ====================== CREATE ACCOUNT ======================
void MainWindow::on_btnCreateSubmit_clicked()
{
    string newUsername = ui->txtNewUsername->text().toStdString();
    string newPassword = ui->txtNewPassword->text().toStdString();

    if (!isValidLength(newUsername, 3, 32) || !isValidLength(newPassword, 3, 32)) {
        QMessageBox::warning(this, "Error", "Must be 3-32 characters long.");
        return;
    }
    if (newUsername.find_first_of(" \t\n") != string::npos) {
        QMessageBox::warning(this, "Error", "Username cannot contain spaces.");
        return;
    }
    if (!isValidPassword(newPassword)) {
        QMessageBox::warning(this, "Error", "Password contains invalid characters or spaces.");
        return;
    }
    if (usernameExists(newUsername)) {
        QMessageBox::warning(this, "Error", "Username already exists!");
        return;
    }

    ofstream file("Logindata.txt", ios::app);
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Could not create database file!");
        return;
    }

    string encryptedPass = encrypt(newPassword);
    file << newUsername << "\n" << encryptedPass << "\n0\n";
    file.close();

    currentUser = QString::fromStdString(newUsername);
    QMessageBox::information(this, "Success", "Account created! Welcome " + currentUser);

    goToCatalogueSlot();
}

// ====================== HELPER FUNCTIONS ======================
bool MainWindow::usernameExists(const string &name)
{
    ifstream file("Logindata.txt");
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        if (line == name) {
            file.close();
            return true;
        }
        getline(file, line); // skip pass
        getline(file, line); // skip loyalty
    }
    file.close();
    return false;
}

string MainWindow::encrypt(string text)
{
    for (char &c : text)
        c = char(int(c) + 5);
    return text;
}

string MainWindow::decrypt(string text)
{
    for (char &c : text)
        c = char(int(c) - 5);
    return text;
}

bool MainWindow::isValidLength(const string &str, int minLen, int maxLen)
{
    return (str.length() >= minLen && str.length() <= maxLen);
}

bool MainWindow::isValidPassword(const string &s)
{
    for (char c : s) {
        unsigned char ascii = (unsigned char)c;
        if (ascii < 33 || ascii > 122) return false;
        if (isalnum(c)) continue;
        const string allowed = "!#$%&*-+=@_";
        if (allowed.find(c) != string::npos) continue;
        return false;
    }
    return true;
}
void MainWindow::on_btnGoToShop_clicked()
{
    goToCatalogueSlot();     // Go to Shopping Gallery
}
// In mainwindow.cpp
void MainWindow::returnToLogin()
{
    currentUser.clear();
    currentPassword.clear();
    ui->stackedWidget->setCurrentIndex(0);
    this->show();
}
void MainWindow::on_btnChangePassword_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "Error", "No user logged in!");
        return;
    }

    bool ok;
    QString newPass = QInputDialog::getText(this,
                                            "Change Password",
                                            "Enter your NEW password:",
                                            QLineEdit::Password, "", &ok);

    if (!ok || newPass.isEmpty()) return;

    QString confirmPass = QInputDialog::getText(this,
                                                "Confirm Password",
                                                "Re-enter your NEW password:",
                                                QLineEdit::Password, "", &ok);

    if (!ok || confirmPass.isEmpty()) return;

    // Check if passwords match
    if (newPass != confirmPass) {
        QMessageBox::warning(this, "Mismatch", "Passwords do not match!");
        return;
    }

    // Validate password
    std::string newPassword = newPass.toStdString();
    if (!isValidLength(newPassword, 3, 32)) {
        QMessageBox::warning(this, "Invalid", "Password must be 3-32 characters long.");
        return;
    }
    if (!isValidPassword(newPassword)) {
        QMessageBox::warning(this, "Invalid", "Password contains invalid characters.");
        return;
    }

    // Update password in file
    if (updatePasswordInFile(currentUser.toStdString(), newPassword)) {
        QMessageBox::information(this, "Success", "Password changed successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update password!");
    }
}
bool MainWindow::updatePasswordInFile(const std::string& username, const std::string& newPassword)
{
    std::ifstream inFile("Logindata.txt");
    if (!inFile) return false;

    std::vector<std::string> lines;
    std::string line;
    bool userFound = false;

    while (getline(inFile, line)) {
        lines.push_back(line);
        if (line == username) {
            userFound = true;
            // Skip old password
            getline(inFile, line);
            // Read loyalty points
            std::string loyalty;
            getline(inFile, loyalty);

            lines.push_back(encrypt(newPassword));  // Add new encrypted password
            lines.push_back(loyalty);               // Keep loyalty points
        } else {
            // Copy password and loyalty for other users
            if (getline(inFile, line)) lines.push_back(line);
            if (getline(inFile, line)) lines.push_back(line);
        }
    }
    inFile.close();

    if (!userFound) return false;

    // Write back to file
    std::ofstream outFile("Logindata.txt");
    if (!outFile) return false;

    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    return true;
}
void MainWindow::refreshCatalogue()
{
    m_catalogue.loadFromFile("inventory.txt");

    if (m_gallery) {
        m_gallery->refreshGallery();  // Refresh shopping gallery UI
    }
}
void MainWindow::goBackToCustomerMenu()
{
    ui->stackedWidget->setCurrentIndex(2);   // 2 = Customer Menu
}
void MainWindow::goToCheckoutSlot()
{
    if (m_checkoutPage) {
        delete m_checkoutPage;   // Clean previous instance
    }

    m_checkoutPage = new Checkout(&m_cart, &m_catalogue, currentUser, this);

    // Optional: Go back to cart or menu after successful checkout
    connect(m_checkoutPage, &Checkout::checkoutCompleted, this, [this]() {
        goToCartSlot();           // or goBackToCustomerMenu()
        m_checkoutPage = nullptr;
    });

    ui->stackedWidget->addWidget(m_checkoutPage);
    ui->stackedWidget->setCurrentWidget(m_checkoutPage);
}