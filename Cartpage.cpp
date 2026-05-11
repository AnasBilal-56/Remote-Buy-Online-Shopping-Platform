#include "cartpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

CartPage::CartPage(Cart* cart, Catalogue* catalogue, FileManager* fm,
                   const QString& username, QWidget *parent)
    : QWidget(parent),
    m_cart(cart),
    m_catalogue(catalogue),
    m_fm(fm),
    m_username(username)
{
    setupUi();
    refreshTable();
}

void CartPage::setupUi()
{
    // Modern UI Styling
    setStyleSheet(
        "QWidget { background-color: #f5f6fa; font-family: 'Segoe UI'; font-size: 13px; color: #2d3436; }"
        "QTableWidget { background-color: white; border: none; border-radius: 8px; gridline-color: #f0f0f0; }"
        "QTableWidget::item { padding: 10px; border-bottom: 1px solid #f0f0f0; }"
        "QHeaderView::section { background-color: #2d3436; color: white; padding: 12px; font-weight: bold; border: none; }"
        "QPushButton { padding: 10px 20px; border-radius: 6px; font-weight: bold; border: none; }"
        "QPushButton#checkoutBtn { background-color: #00b894; color: white; }"
        "QPushButton#checkoutBtn:hover { background-color: #00a381; }"
        "QPushButton#removeBtn { background-color: #d63031; color: white; }"
        "QPushButton#updateBtn { background-color: #fdcb6e; color: #2d3436; }"
        "QPushButton#backBtn { background-color: white; color: #2d3436; border: 2px solid #dfe6e9; }"
        "QPushButton#historyBtn { background-color: #6c5ce7; color: white; }"
        "QLabel#totalLabel { background-color: white; border-radius: 8px; padding: 12px 20px; font-size: 18px; font-weight: bold; color: #00b894; }"
        );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(15);

    // Title
    QLabel* title = new QLabel("Your Shopping Cart");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #2d3436;");
    mainLayout->addWidget(title);

    // Table
    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"ID", "Product", "Unit Price", "Quantity"});
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);

    // Total Label
    m_totalLabel = new QLabel("Total: PKR 0.00", this);
    m_totalLabel->setObjectName("totalLabel");

    // Buttons
    m_backBtn = new QPushButton("Back to Shop", this);
    m_backBtn->setObjectName("backBtn");

    m_historyBtn = new QPushButton("Order History", this);
    m_historyBtn->setObjectName("historyBtn");

    m_removeBtn = new QPushButton("Remove Item", this);
    m_removeBtn->setObjectName("removeBtn");

    m_updateBtn = new QPushButton("Update Qty", this);
    m_updateBtn->setObjectName("updateBtn");

    m_checkoutBtn = new QPushButton("Checkout", this);
    m_checkoutBtn->setObjectName("checkoutBtn");

    // Button Layout
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(m_backBtn);
    btnRow->addWidget(m_historyBtn);
    btnRow->addStretch();
    btnRow->addWidget(m_removeBtn);
    btnRow->addWidget(m_updateBtn);
    btnRow->addWidget(m_checkoutBtn);

    mainLayout->addWidget(m_table);
    mainLayout->addWidget(m_totalLabel, 0, Qt::AlignRight);
    mainLayout->addLayout(btnRow);

    // Connections
    connect(m_backBtn, &QPushButton::clicked, this, &CartPage::goToCatalogue);
    connect(m_historyBtn, &QPushButton::clicked, this, &CartPage::goToOrderHistory);
    connect(m_removeBtn, &QPushButton::clicked, this, &CartPage::onRemoveItem);
    connect(m_updateBtn, &QPushButton::clicked, this, &CartPage::onUpdateQty);
    connect(m_checkoutBtn, &QPushButton::clicked, this, &CartPage::on_btnCheckout_clicked);
}

void CartPage::refreshTable()
{
    m_table->setRowCount(0);

    if (m_cart->isEmpty()) {
        m_totalLabel->setText("Total: PKR 0.00");
        return;
    }

    Cartitem* items = m_cart->getItems();
    int count = m_cart->getCount();

    for (int i = 0; i < count; i++) {
        int row = m_table->rowCount();
        m_table->insertRow(row);

        Cartitem ci = items[i];

        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(ci.item.getId())));
        m_table->setItem(row, 1, new QTableWidgetItem(ci.item.getName()));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::number(ci.item.getPrice(), 'f', 2)));
        m_table->setItem(row, 3, new QTableWidgetItem(QString::number(ci.quantity)));

        m_table->setRowHeight(row, 40);
    }

    m_totalLabel->setText("Total: PKR " + QString::number(m_cart->calculatetotal(), 'f', 2));
}

void CartPage::onRemoveItem()
{
    int row = m_table->currentRow();
    if (row < 0) return;

    int id = m_table->item(row, 0)->text().toInt();
    Product dummy(id, "", 0.0, "");
    m_cart->removeproduct(dummy);
    refreshTable();
}

void CartPage::onUpdateQty()
{
    int row = m_table->currentRow();
    if (row < 0) return;

    int id = m_table->item(row, 0)->text().toInt();
    bool ok;
    int qty = QInputDialog::getInt(this, "Update Quantity", "New Quantity:", 1, 0, 100, 1, &ok);

    if (ok) {
        m_cart->updateQuantity(id, qty);
        refreshTable();
    }
}

// Button Clicked → Goes to Checkout Page
void CartPage::on_btnCheckout_clicked()
{
    if (m_cart->getCount() == 0) {
        QMessageBox::warning(this, "Empty Cart",
                             "Your cart is empty! Add some products first.");
        return;
    }

    emit goToCheckout();   // This signal goes to MainWindow
}

// Optional: Direct Checkout from Cart (if you want to keep old logic)
void CartPage::onCheckout()
{
    // This function is no longer needed if you're using goToCheckout signal
    // You can remove it or keep it for future use
}