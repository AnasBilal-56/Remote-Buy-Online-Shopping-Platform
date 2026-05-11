#pragma once
#ifndef CATALOGUE_H
#define CATALOGUE_H

#include "product.h"
#include <QString>

class Catalogue
{
private:
    Product** products;
    int size;
    int capacity;

public:
    Catalogue(int cap = 100);
    ~Catalogue();

    void addProduct(Product* p);
    void removeProduct(int id);
    int getSize() const;
    Product* getProductAtIndex(int index);
    bool exists(int id);
    bool isEmpty() const;
    Product* findProductById(int id) const;

    // File Operations
    void saveToFile(const QString& filename);
    void loadFromFile(const QString& filename);
};

#endif // CATALOGUE_H