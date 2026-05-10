#include "Shoppingheader.h"
class Product{
private:
    int id, stock; double price;
    string name, category;

public:
    Product(){id = 0,stock =0, price =0,name = "",category="";}
    Product(int i,int s, string n, double p, string c) {
        id = i,name = n, price = p, category =c, stock = s;
    }

    // void Display() const { cout << id << "|" << name << "|" << price << "|" << endl; } //useless?
    string getname() const { return name; }
    int getId() const { return id; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }
    int getStock() const { return stock; }

    void modifyName(string name) { this->name = name; }
    void modifyPrice(double price) { this->price = price; }
    void modifyStock(int stock) { this->stock = stock; }
};

class Catalogue{
private:
    Product *products; int size;

public:
    Catalogue(){ size = 0; products = new Product[100];}
    void addProduct(Product p){
        if (size < 100) {products[size++] = p;}
        else {cout << "Catalogue is Full!" << endl;}   ///TERMINAL OUTPUT CHANGE TO QT UI PLS
    }

    int getsize() const { return size; }
    bool isempty() const { return size == 0; }

    // void showProducts(){
    //     for (int i = 0; i < size; i++) {products[i].Display();}
    // }

    Product getProduct(int id){
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id)
                return products[i];
        }
    }

    Product getProductByName(string name){
        for (int i = 0; i < size; i++) {
            if (products[i].getname() == name)
                return products[i];
        }
    }

    bool ProductExists(int id){
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id)
                return true;
        }
        return false;
    }

    void removeProduct(int id){
        int index = -1;
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Product Not Found" << endl;
            return;
        }

        for (int i = index; i < size; i++) {products[i] = products[i + 1];}

        size--;
        cout << "Product Removed Successfully" << endl;
    }

    void UpdateProduct(int id, string name, double price){
        bool found = false;
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id) {
                products[i].modifyName(name);
                products[i].modifyPrice(price);
                cout << "Product Updated Successfully" << endl;
                found = true;
                break;
            }
        }

        if (!found) {cout << "Product Not Found" << endl;}
    }

    Product getProductAtIndex(int index){if (index >= 0 && index < size){return products[index];}}
    ~Catalogue() { delete[] products; }
};

Catalogue ProductCatalogue;

