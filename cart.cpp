#include "cart.h"

using namespace std;

// ==========================================
// Cartitem Implementation
// ==========================================

Cartitem::Cartitem() {
    quantity = 0;
}

Product Cartitem::getitem() {
    return item;
}

Cartitem& Cartitem::operator--() {
    if (quantity > 0) {
        quantity--;
    }
    return *this;
}

// ==========================================
// Cart Implementation
// ==========================================

Cart::Cart() {
    Cid = 0;
    p = nullptr;
    count = 0;
}

Cart::Cart(unsigned long long int Ci) {
    Cid = Ci;
    p = nullptr;
    count = 0;
}

Cart::~Cart() {
    delete[] p;
    p = nullptr;
}

void Cart::addproduct(Product pro, int q) {
    if (count == 0) {
        p = new Cartitem[1];
        p[0].item = pro;
        p[0].quantity = q;
        count = 1;
        return;
    }

    for (int i = 0; i < count; i++) {
        if (p[i].item == pro) {
            p[i].quantity += q;
            return;
        }
    }

    Cartitem *temp = new Cartitem[count + 1];
    for (int i = 0; i < count; i++) {
        temp[i] = p[i];
    }

    delete[] p;
    temp[count].item = pro;
    temp[count].quantity = q;
    count++;
    p = temp;
}

void Cart::removeproduct(Product pro) {
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (p[i].item == pro) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << " Product not found " << endl;
        return;
    }

    Cartitem *temp = new Cartitem[count - 1];
    int j = 0;
    for (int i = 0; i < count; i++) {
        if (i != index) {
            temp[j] = p[i];
            j++;
        }
    }

    delete[] p;
    p = temp;
    count--;
}

void Cart::viewcart() {
    for (int i = 0; i < count; i++) {
        p[i].item.Display();
        cout << " Quantity :  " << p[i].quantity << endl;
    }
}

long double Cart::calculatetotal() {
    long double total = 0;
    for (int i = 0; i < count; i++) {
        total += (p[i].item.getPrice() * p[i].quantity);
    }
    return total;
}

void Cart::clearcart() {
    delete[] p;
    p = nullptr;
    count = 0;
}

// --- NEW HELPER FUNCTION ADDED ---
void Cart::updateQuantity(int id, int newQty) {
    for (int i = 0; i < count; i++) {
        if (p[i].item.getId() == id) {
            if (newQty == 0) {
                removeproduct(p[i].item);
            } else {
                p[i].quantity = newQty;
            }
            return;
        }
    }
}