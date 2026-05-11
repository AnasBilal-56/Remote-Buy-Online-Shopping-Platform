#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include "cart.h"
#include "Catalogue.h"

class Checkout : public QWidget
{
    Q_OBJECT

public:
    explicit Checkout(Cart* cart, Catalogue* catalogue, QString username, QWidget *parent = nullptr);
    ~Checkout();

signals:
    void checkoutCompleted();   // Notify MainWindow after successful purchase

private slots:
    void on_btnApplyPromo_clicked();
    void on_btnConfirmPurchase_clicked();
    void on_btnCancel_clicked();

private:
    // UI Elements
    QTableWidget* tableCart = nullptr;
    QLabel* lblLoyaltyStatus = nullptr;
    QLineEdit* txtPromoCode = nullptr;
    QLabel* lblSubtotal = nullptr;
    QLabel* lblDiscount = nullptr;
    QLabel* lblTotal = nullptr;

    // Data
    Cart* myCart = nullptr;
    Catalogue* myCatalogue = nullptr;
    QString currentUsername;

    double subtotal = 0.0;
    double discountAmount = 0.0;
    double finalTotal = 0.0;

    void setupUI();
    void loadCartToTable();
    void calculateTotals();
    void updateStock();
    void saveHistory();
    void generateReceipt();
    int getLoyaltyStatus();
};

#endif // CHECKOUT_H