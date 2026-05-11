#include "ADMININVENTORY.h"
#include "ui_ADMININVENTORY.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QPixmap>

AdminInventory::AdminInventory(QWidget *parent)
    : QMainWindow(parent)          // Changed to QMainWindow (recommended)
    , ui(new Ui::AdminInventory)
{
    ui->setupUi(this);
    this->setWindowTitle("Admin Inventory System");

    // Load logo if available
    QPixmap pix(":/logo.png");
    if (!pix.isNull() && ui->label) {
        ui->label->setPixmap(pix.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    myCatalogue = new Catalogue(100);
    myCatalogue->loadFromFile("inventory.txt");
    updateVisualList();
}

AdminInventory::~AdminInventory()
{
    if (myCatalogue) {
        myCatalogue->saveToFile("inventory.txt");
        delete myCatalogue;
    }
    delete ui;
}

// ====================== ADD PRODUCT ======================
void AdminInventory::on_btnAddProduct_clicked()
{
    if (ui->txtID->text().isEmpty() || ui->txtName->text().isEmpty() || ui->txtPrice->text().isEmpty()) {
        QMessageBox::critical(this, "Input Error", "All fields must be filled!");
        return;
    }

    int id = ui->txtID->text().toInt();
    QString name = ui->txtName->text();
    double price = ui->txtPrice->text().toDouble();

    if (id == 0 && ui->txtID->text() != "0") {
        QMessageBox::warning(this, "Format Error", "ID must be a valid number!");
        return;
    }

    myCatalogue->addProduct(new Product(id, name, price));
    myCatalogue->saveToFile("inventory.txt");

    ui->txtID->clear();
    ui->txtName->clear();
    ui->txtPrice->clear();

    updateVisualList();
    QMessageBox::information(this, "Success", "Product added successfully!");
    emit inventoryChanged();
}

// ====================== DELETE PRODUCT ======================
void AdminInventory::on_btnDelete_clicked()
{
    QListWidgetItem *item = ui->listProducts->currentItem();
    if (!item) {
        QMessageBox::warning(this, "No Selection", "Please select a product to delete!");
        return;
    }

    if (QMessageBox::question(this, "Confirm Delete",
                              "Are you sure you want to delete this product?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QString text = item->text();
        int id = text.split(" | ").at(0).toInt();

        myCatalogue->removeProduct(id);
        myCatalogue->saveToFile("inventory.txt");
        updateVisualList();

        QMessageBox::information(this, "Deleted", "Product deleted successfully.");
    }
    emit inventoryChanged();
}

// ====================== REFRESH LIST ======================
void AdminInventory::updateVisualList()
{
    ui->listProducts->clear();
    for (int i = 0; i < myCatalogue->getSize(); i++) {
        Product* p = myCatalogue->getProductAtIndex(i);
        if (p) {
            QString info = QString::number(p->getId()) +
                           " | " + p->getName() +
                           " | $" + QString::number(p->getPrice(), 'f', 2);
            ui->listProducts->addItem(info);
        }
    }
}

// ====================== LOGOUT ======================
// ====================== LOGOUT ======================
void AdminInventory::on_btnLogout_clicked()
{
    if (loginWindowPtr != nullptr) {
        loginWindowPtr->returnToLogin();
    }

    this->close();
}