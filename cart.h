#ifndef CART_H
#define CART_H

#include <iostream>
#include <string>
#include "product.h"

class Cartitem {
public:
    Product item;
    unsigned long int quantity;

    Cartitem();
    Product getitem();
    Cartitem &operator--();
};

class Cart {
private:
    unsigned long long int Cid;
    Cartitem* p;
    int count;

public:
    Cartitem* getItems() const;     // Add this
    bool isEmpty() const;           // Add this

    Cart();
    Cart(unsigned long long int Ci);
    ~Cart();

    void addproduct(Product pro, int q);
    void removeproduct(Product pro);
    void viewcart();
    long double calculatetotal();
    void clearcart();
    void updateQuantity(int id, int newQty);

    // ADD THIS LINE:
    int getCount() const;
};

#endif // CART_H