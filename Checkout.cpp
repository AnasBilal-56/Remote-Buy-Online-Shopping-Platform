#include "Checkout.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

Checkout::Checkout(Cart* cart, Catalogue* catalogue, QString username, QWidget* parent)
    : QWidget(parent),
    myCart(cart),
    myCatalogue(catalogue),
    currentUsername(username)
{
    setupUI();
    loadCartToTable();
    calculateTotals();
}

Checkout::~Checkout() { }

void Checkout::setupUI()
{
    setWindowTitle("Secure Checkout - Remote Buy");
    resize(650, 680);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // Header
    QLabel* header = new QLabel("Order Summary", this);
    header->setAlignment(Qt::AlignCenter);
    QFont hfont = header->font();
    hfont.setPointSize(18);
    hfont.setBold(true);
    header->setFont(hfont);
    mainLayout->addWidget(header);

    // Table
    tableCart = new QTableWidget(this);
    tableCart->setColumnCount(4);
    tableCart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList headers = {"Product", "Unit Price", "Qty", "Total"};
    tableCart->setHorizontalHeaderLabels(headers);
    tableCart->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(tableCart);

    lblLoyaltyStatus = new QLabel("Checking loyalty status...", this);
    mainLayout->addWidget(lblLoyaltyStatus);

    // Promo
    QHBoxLayout* promoLayout = new QHBoxLayout();
    txtPromoCode = new QLineEdit(this);
    txtPromoCode->setPlaceholderText("Enter Promo Code (e.g. SAVE20)");
    QPushButton* btnApply = new QPushButton("Apply Code", this);
    promoLayout->addWidget(txtPromoCode);
    promoLayout->addWidget(btnApply);
    mainLayout->addLayout(promoLayout);
    connect(btnApply, &QPushButton::clicked, this, &Checkout::on_btnApplyPromo_clicked);

    // Separator
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(line);

    // Totals
    QFormLayout* form = new QFormLayout();
    lblSubtotal = new QLabel("$0.00");
    lblDiscount = new QLabel("-$0.00");
    lblTotal = new QLabel("$0.00");
    lblTotal->setStyleSheet("font-size: 16pt; color: #27ae60; font-weight: bold;");

    form->addRow("Subtotal:", lblSubtotal);
    form->addRow("Discount:", lblDiscount);
    form->addRow("Grand Total:", lblTotal);
    mainLayout->addLayout(form);

    // Buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnConfirm = new QPushButton("COMPLETE PURCHASE", this);
    QPushButton* btnCancel = new QPushButton("Cancel", this);

    btnConfirm->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; padding: 12px; font-size: 14px;");
    btnCancel->setStyleSheet("background-color: #e74c3c; color: white; padding: 10px;");

    btnLayout->addWidget(btnConfirm);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnConfirm, &QPushButton::clicked, this, &Checkout::on_btnConfirmPurchase_clicked);
    connect(btnCancel, &QPushButton::clicked, this, &Checkout::on_btnCancel_clicked);
}

// ==================== Existing Functions (keep as they are) ====================

void Checkout::loadCartToTable() { ... }     // Keep your original
void Checkout::calculateTotals() { ... }     // Keep your original
void Checkout::on_btnApplyPromo_clicked() { ... }
// ... keep updateStock(), saveHistory(), generateReceipt(), getLoyaltyStatus() as they are

void Checkout::on_btnConfirmPurchase_clicked()
{
    if (myCart->count == 0) {
        QMessageBox::warning(this, "Empty Cart", "Your cart is empty!");
        return;
    }

    updateStock();
    saveHistory();
    generateReceipt();

    myCart->clearcart();

    QMessageBox::information(this, "Success",
                             "Purchase Completed Successfully!\nReceipt has been saved.");

    emit checkoutCompleted();   // Notify MainWindow
    this->close();
}

void Checkout::on_btnCancel_clicked()
{
    this->close();
}