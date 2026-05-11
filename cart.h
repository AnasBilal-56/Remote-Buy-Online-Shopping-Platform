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
    unsigned long long int Cid;
    Cartitem *p;
    int count;

public:
    Cart();
    Cart(unsigned long long int Ci);
    ~Cart();

    void addproduct(Product pro, int q);
    void removeproduct(Product pro);
    void viewcart();
    long double calculatetotal();
    void clearcart();

    // --- UI HELPER FUNCTIONS ---
    int itemCount() const { return count; }
    Cartitem* items() const { return p; }
    bool isEmpty() const { return count == 0; }
    void updateQuantity(int id, int newQty);
};

#endif // CART_H