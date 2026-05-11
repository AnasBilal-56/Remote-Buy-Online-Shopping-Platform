#include "OrderHistory.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

OrderHistoryPage::OrderHistoryPage(FileManager* fm, QWidget* parent)
    : QWidget(parent), m_fm(fm) // Initialize the pointer
{
    setupUi();
    refreshOrders();
}

void OrderHistoryPage::setupUi()
{
    setStyleSheet(
        "QWidget { background-color: #f5f6fa; font-family: 'Segoe UI'; font-size: 13px; color: #2d3436; }"
        "QTableWidget { background-color: white; border: none; border-radius: 8px; gridline-color: #f0f0f0; }"
        "QHeaderView::section { background-color: #2d3436; color: white; padding: 12px 8px; font-weight: bold; border: none; }"
        "QPushButton#refreshBtn { background-color: #6c5ce7; color: white; border-radius: 6px; font-weight: bold; padding: 10px 20px; }"
        "QPushButton#backBtn { background-color: white; color: #2d3436; border: 2px solid #dfe6e9; border-radius: 6px; font-weight: bold; padding: 10px 20px; }"
        "QLabel#totalLabel { background-color: white; border-radius: 8px; padding: 12px 20px; font-size: 16px; font-weight: bold; color: #6c5ce7; }"
        );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 16, 24, 16);

    QLabel* title = new QLabel("Your Purchase History");
    title->setStyleSheet("font-size: 24px; font-weight: bold; padding: 10px 0px;");

    m_table = new QTableWidget();
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"ID", "Product Name", "Unit Price", "Qty", "Total"});
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_emptyLabel = new QLabel("No purchase records found.");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet("color: #b2bec3; font-style: italic; font-size: 16px;");
    m_emptyLabel->hide();

    m_totalLabel = new QLabel("Lifetime Spend: PKR 0.00");
    m_totalLabel->setObjectName("totalLabel");

    m_refreshBtn = new QPushButton("Update Records");
    m_refreshBtn->setObjectName("refreshBtn");
    m_backBtn = new QPushButton("Back to Shop");
    m_backBtn->setObjectName("backBtn");

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(m_backBtn);
    btnRow->addStretch();
    btnRow->addWidget(m_refreshBtn);

    mainLayout->addWidget(title);
    mainLayout->addWidget(m_table);
    mainLayout->addWidget(m_emptyLabel);
    mainLayout->addWidget(m_totalLabel, 0, Qt::AlignRight);
    mainLayout->addLayout(btnRow);

    connect(m_refreshBtn, &QPushButton::clicked, this, &OrderHistoryPage::onRefresh);
    connect(m_backBtn,    &QPushButton::clicked, this, &OrderHistoryPage::goBack);
}

void OrderHistoryPage::onRefresh()
{
    refreshOrders(); // Now pulls real data
}

void OrderHistoryPage::refreshOrders()
{
    m_table->setRowCount(0);

    // FETCH REAL DATA FROM FILE
    QVector<OrderRecord> orders = m_fm->loadOrders();

    if (orders.isEmpty()) {
        m_emptyLabel->show();
        m_table->hide();
        m_totalLabel->setText("Lifetime Spend: PKR 0.00");
        return;
    }

    m_emptyLabel->hide();
    m_table->show();

    double grandTotal = 0.0;
    for (int i = 0; i < orders.size(); i++) {
        int row = m_table->rowCount();
        m_table->insertRow(row);

        double lineTotal = orders[i].price * orders[i].quantity;
        grandTotal += lineTotal;

        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(orders[i].id)));
        m_table->setItem(row, 1, new QTableWidgetItem(orders[i].name));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::number(orders[i].price)));
        m_table->setItem(row, 3, new QTableWidgetItem(QString::number(orders[i].quantity)));
        m_table->setItem(row, 4, new QTableWidgetItem(QString::number(lineTotal)));
        m_table->setRowHeight(row, 40);
    }

    m_totalLabel->setText("Lifetime Spend: PKR " + QString::number(grandTotal, 'f', 2));
}