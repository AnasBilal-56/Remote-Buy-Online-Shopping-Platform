#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ADMININVENTORY.h"
#include <QMessageBox>
#include <QApplication>
#include <fstream>
#include <string>

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
    m_catalogue.addProduct(new Product(101, "Wireless Mouse", 1500.00, "Electronics"));
    m_catalogue.addProduct(new Product(102, "Mechanical Keyboard", 4500.00, "Electronics"));
    m_catalogue.addProduct(new Product(103, "Gaming Headset", 3200.00, "Audio"));
    m_catalogue.addProduct(new Product(104, "USB-C Hub", 1200.00, "Accessories"));
}

void MainWindow::setupShoppingPages()
{
    m_gallery = new ShoppingGallery(&m_catalogue, &m_cart, this);
    m_cartPage = new CartPage(&m_cart, &m_catalogue, &m_fm, this);
    m_orderHistoryPage = new OrderHistoryPage(&m_fm, this);

    // Add to stacked widget (Make sure indices 4,5,6 are free in your UI)
    ui->stackedWidget->addWidget(m_gallery);           // Index 4
    ui->stackedWidget->addWidget(m_cartPage);          // Index 5
    ui->stackedWidget->addWidget(m_orderHistoryPage);  // Index 6

    // Signal-Slot Connections
    connect(m_gallery, &ShoppingGallery::goToCart, this, &MainWindow::goToCartSlot);
    connect(m_cartPage, &CartPage::goToCatalogue, this, &MainWindow::goToCatalogueSlot);
    connect(m_cartPage, &CartPage::goToOrderHistory, this, &MainWindow::goToHistorySlot);
    connect(m_orderHistoryPage, &OrderHistoryPage::goBack, this, &MainWindow::goToCartSlot);
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
            adminWin->show();
            this->hide();
        } else {
            goToCatalogueSlot();
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