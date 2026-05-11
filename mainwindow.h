#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "ADMININVENTORY.h"
#include "ShoppingGallery.h"
#include "CartPage.h"
#include "OrderHistory.h"
#include "Catalogue.h"
#include "Cart.h"
#include "FileManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString currentUser;
    QString currentPassword;

private slots:
    void on_btnLoginMenu_clicked();
    void on_btnCreateAccountMenu_clicked();
    void on_btnBack_clicked();
    void on_btnBackCreate_clicked();
    void on_btnExit_clicked();
    void on_btnLogout_clicked();
    void on_btnLoginSubmit_clicked();
    void on_btnCreateSubmit_clicked();

    void goToCatalogueSlot();
    void goToCartSlot();
    void goToHistorySlot();

private:
    Ui::MainWindow *ui;

    Catalogue m_catalogue;           // Now works with default constructor
    Cart m_cart;
    FileManager m_fm;

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