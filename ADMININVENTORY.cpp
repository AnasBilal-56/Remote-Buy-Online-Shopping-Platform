#include "ADMININVENTORY.h"
#include "ui_ADMININVENTORY.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

AdminInventory::AdminInventory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminInventory)
{
    ui->setupUi(this);
    this->setWindowTitle("Admin Inventory System");

    // Optional: Load logo if you have it in resources
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

    // Add to catalogue
    myCatalogue->addProduct(new Product(id, name, price));

    // Save immediately
    myCatalogue->saveToFile("inventory.txt");

    // Clear inputs and refresh list
    ui->txtID->clear();
    ui->txtName->clear();
    ui->txtPrice->clear();
    updateVisualList();

    QMessageBox::information(this, "Success", "Product added successfully!");
}

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
}

void AdminInventory::updateVisualList()
{
    ui->listProducts->clear();

    for (int i = 0; i < myCatalogue->getSize(); i++) {
        Product* p = myCatalogue->getProductAtIndex(i);
        if (p) {
            QString info = QString::number(p->getId())
            + " | " + p->getName()
                + " | $" + QString::number(p->getPrice(), 'f', 2);
            ui->listProducts->addItem(info);
        }
    }
}