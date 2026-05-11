#include "product.h"

Product::Product() {
    id = 0;
    price = 0.0;
    name = "";
    category = "";
}

Product::Product(int i, QString n, double p, QString c) {
    id = i;
    name = n;
    price = p;
    category = c;
}
Product::Product(int i, QString n, double p)
    : id(i), name(n), price(p), category("General") {}   // or whatever default
Product::~Product() {}

void Product::Display() const {}

QString Product::getName() const { return name; }
int Product::getId() const { return id; }
double Product::getPrice() const { return price; }
QString Product::getCategory() const { return category; }

void Product::modifyName(QString n) { name = n; }
void Product::modifyPrice(double p) { price = p; }
void Product::modifyCategory(QString c) { category = c; }

bool Product::isValid() const { return id != 0; }

bool Product::operator==(const Product& obj) const {
    return (this->id == obj.id);
}

// Electronics Implementation
Electronics::Electronics(int i, QString n, double p, QString c, int w)
    : Product(i, n, p, c)
{
    warranty = w;
}

void Electronics::Display() const {}
int Electronics::getWarranty() const { return warranty; }