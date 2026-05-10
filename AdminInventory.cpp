//Edited by Anas 
#include "AdminInventory.h"
#include "ui_AdminInventory.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
Product::Product() : id(0), name(""), price(0.0) {}
Product::Product(int i, QString n, double p) : id(i), name(n), price(p) {}

void Product::Display() const {
    qDebug() << "ID:" << id << "| Name:" << name << "| Price: $" << price;
}

QString Product::getName() const { return name; }
int Product::getId() const { return id; }
double Product::getPrice() const { return price; }
void Product::modifyName(QString n) { this->name = n; }
void Product::modifyPrice(double p) { this->price = p; }
Electronics::Electronics(int i, QString n, double p, int w) : Product(i, n, p) {
    this->warranty = w;
}

void Electronics::Display() const {
    qDebug() << "ELECTRONICS ITEM | ID:" << getId() << "| Warranty:" << warranty << "months";
}
Catalogue::Catalogue(int cap) : capacity(cap), size(0) {
    products = new Product * [capacity];
}

Catalogue::~Catalogue() {
    for (int i = 0; i < size; i++) { delete products[i]; }
    delete[] products;
}

void Catalogue::addProduct(Product* p) {
    if (size < capacity) { products[size++] = p; }
}

int Catalogue::getSize() const { return size; }

Product* Catalogue::getProductAtIndex(int index) {
    if (index >= 0 && index < size) return products[index];
    return nullptr;
}

void Catalogue::removeProduct(int id) {
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (products[i]->getId() == id) {
            index = i; break;
        }
    }
    if (index != -1) {
        delete products[index];
        for (int i = index; i < size - 1; i++) { products[i] = products[i + 1]; }
        size--;
    }
}

void Catalogue::saveToFile(QString filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < size; i++) {
            out << products[i]->getId() << "," << products[i]->getName() << "," << products[i]->getPrice() << "\n";
        }
        file.close();
    }
}

void Catalogue::loadFromFile(QString filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() == 3) {
            // FIX: Correct array indices [0], [1], [2] restored
            int id = fields[0].toInt();
            QString name = fields[1];
            double price = fields[2].toDouble();

            addProduct(new Product(id, name, price));
        }
    }
    file.close();
}

// ==========================================================
// 4. ADMIN INVENTORY WINDOW FUNCTIONS
// ==========================================================

AdminInventory::AdminInventory(QWidget* parent) : QWidget(parent), ui(new Ui::AdminInventory) {
    ui->setupUi(this);

    // 1. Set Window Title
    this->setWindowTitle("Admin Inventory System");

    // 2. Load the Logo
    // Ensure you have added logo.png to your .qrc file!
    QPixmap pix(":/logo.png");
    if (!pix.isNull()) {
        // This scales the logo to fit your QLabel named 'label'
        ui->label->setPixmap(pix.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        qDebug() << "Logo failed to load! Check the resource path.";
    }

    // 3. Initialize Catalogue and load data
    myCatalogue = new Catalogue(100);
    myCatalogue->loadFromFile("inventory.txt");
    updateVisualList();
}

AdminInventory::~AdminInventory() {
    if (myCatalogue != nullptr) {
        myCatalogue->saveToFile("inventory.txt");
    }
    delete myCatalogue;
    delete ui;
}

void AdminInventory::on_btnAddProduct_clicked() {
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
    ui->txtID->clear();
    ui->txtName->clear();
    ui->txtPrice->clear();
    updateVisualList();
}

void AdminInventory::on_btnDelete_clicked() {
    QListWidgetItem* item = ui->listProducts->currentItem();
    if (!item) {
        QMessageBox::warning(this, "No Selection", "Please select a product!");
        return;
    }

    if (QMessageBox::question(this, "Confirm", "Delete this product?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString text = item->text();

        // FIX: Added to select the ID part of the string
        int id = text.split(" | ").at(0).toInt();

        myCatalogue->removeProduct(id);

        // Also fix the small typo: ensured updateVisualList() is one word
        updateVisualList();

        myCatalogue->saveToFile("inventory.txt");
    }
}

void AdminInventory::updateVisualList() {
    ui->listProducts->clear();
    for (int i = 0; i < myCatalogue->getSize(); i++) {
        Product* p = myCatalogue->getProductAtIndex(i);
        if (p) {
            QString info = QString::number(p->getId()) + " | " + p->getName() + " | $" + QString::number(p->getPrice(), 'f', 2);
            ui->listProducts->addItem(info);
        }
    }
}
