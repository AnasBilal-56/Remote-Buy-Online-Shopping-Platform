#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "ADMININVENTORY.h"
#include "ShoppingGallery.h"
#include "CartPage.h"
#include "OrderHistory.h"
#include "catalogue.h"
#include "cart.h"
#include "filemanager.h"
#include "Checkout.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void returnToLogin();
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString currentUser;
    QString currentPassword;
    //Ui::MainWindow *ui;

private slots:
    void on_btnLoginMenu_clicked();
    void on_btnCreateAccountMenu_clicked();
    void on_btnBack_clicked();
    void on_btnBackCreate_clicked();
    void on_btnExit_clicked();
    void on_btnLogout_clicked();
    void on_btnLoginSubmit_clicked();
    void on_btnCreateSubmit_clicked();
    void on_btnChangePassword_clicked();
    void refreshCatalogue();
    void goBackToCustomerMenu();
    void goToCatalogueSlot();
    void goToCartSlot();
    void goToHistorySlot();
    // Inside MainWindow class (private slots)
    void on_btnGoToShop_clicked();     // NEW: Button to go to Shopping Gallery
public slots:
    void goToCheckoutSlot();
private:
    Ui::MainWindow *ui;
    bool updatePasswordInFile(const std::string& username, const std::string& newPassword);
    Catalogue m_catalogue;           // Now works with default constructor
    Cart m_cart;
    FileManager m_fm;
    Checkout* m_checkoutPage = nullptr;

    ShoppingGallery *m_gallery = nullptr;
    CartPage *m_cartPage = nullptr;
    OrderHistoryPage *m_orderHistoryPage = nullptr;

    void initializeTestProducts();
    void setupShoppingPages();

    // Helper functions
    bool usernameExists(const std::string &name);
    std::string encrypt(std::string text);
    std::string decrypt(std::string text);
    bool isValidLength(const std::string &str, int minLen, int maxLen);
    bool isValidPassword(const std::string &s);
};

#endif // MAINWINDOW_H