#include "ShoppingGallery.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

ShoppingGallery::ShoppingGallery(Catalogue* cat, Cart* cart, QWidget* parent)
    : QWidget(parent), m_catalogue(cat), m_cart(cart)
{
    setupUi();
    refreshGallery();
}

void ShoppingGallery::setupUi()
{
    // Professional Dark/Light Styling
    setStyleSheet(
        "QWidget { background-color: #f5f6fa; font-family: 'Segoe UI'; font-size: 13px; }"
        "QTableWidget { background-color: white; border-radius: 8px; gridline-color: #f0f0f0; }"
        "QLineEdit { padding: 10px; border: 2px solid #dfe6e9; border-radius: 6px; background: white; }"
        "QPushButton#viewCartBtn { background-color: #0984e3; color: white; font-weight: bold; padding: 10px; border-radius: 6px; }"
        "QPushButton#addBtn { background-color: #00b894; color: white; border-radius: 4px; padding: 5px; }"
        );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Header Section
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* title = new QLabel("Product Gallery");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #2d3436;");

    m_viewCartBtn = new QPushButton("View My Cart");
    m_viewCartBtn->setObjectName("viewCartBtn");

    header->addWidget(title);
    header->addStretch();
    header->addWidget(m_viewCartBtn);

    // Search Bar
    m_searchBar = new QLineEdit();
    m_searchBar->setPlaceholderText("Search products by name or category...");

    // Table Setup
    m_table = new QTableWidget();
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"ID", "Product Name", "Section", "Price", "Action"});
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addLayout(header);
    mainLayout->addWidget(m_searchBar);
    mainLayout->addWidget(m_table);

    // Connections
    connect(m_searchBar, &QLineEdit::textChanged, this, &ShoppingGallery::onSearchChanged);
    connect(m_viewCartBtn, &QPushButton::clicked, this, &ShoppingGallery::goToCart);
}

void ShoppingGallery::refreshGallery(QString filter)
{
    m_table->setRowCount(0);
    filter = filter.toLower();

    for (int i = 0; i < m_catalogue->getSize(); i++) {
        Product* p = m_catalogue->getProductAtIndex(i);

        // Search Filter Logic (Checks Name and Category)
        if (!filter.isEmpty() &&
            !p->getName().toLower().contains(filter) &&
            !p->getCategory().toLower().contains(filter)) {
            continue;
        }

        int row = m_table->rowCount();
        m_table->insertRow(row);

        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(p->getId())));
        m_table->setItem(row, 1, new QTableWidgetItem(p->getName()));
        m_table->setItem(row, 2, new QTableWidgetItem(p->getCategory()));
        m_table->setItem(row, 3, new QTableWidgetItem("PKR " + QString::number(p->getPrice())));

        // Add to Cart Button inside the table
        QPushButton* addBtn = new QPushButton("Add to Cart");
        addBtn->setObjectName("addBtn");

        // Use a property to remember which product this button belongs to
        addBtn->setProperty("prodID", p->getId());
        connect(addBtn, &QPushButton::clicked, this, &ShoppingGallery::onAddToCart);

        m_table->setCellWidget(row, 4, addBtn);
        m_table->setRowHeight(row, 45);
    }
}

void ShoppingGallery::onSearchChanged(const QString& text)
{
    refreshGallery(text);
}

void ShoppingGallery::onAddToCart()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int id = btn->property("prodID").toInt();

    // Find the product in the catalogue
    for (int i = 0; i < m_catalogue->getSize(); i++) {
        Product* p = m_catalogue->getProductAtIndex(i);
        if (p->getId() == id) {
            m_cart->addproduct(*p, 1); // Logic: Add 1 quantity to Cart
            QMessageBox::information(this, "Added", p->getName() + " added to cart!");
            return;
        }
    }
}