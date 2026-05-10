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

    // void showProducts(){
    //     for (int i = 0; i < size; i++) {products[i].Display();}
    // }

    Product getProduct(int id){
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id)
                return products[i];
        }
        return ;
    }

    Product getProductByName(string name)
    {
        for (int i = 0; i < size; i++) {
            if (products[i].getname() == name)
                return products[i];
        }
        return Product(-1, "Not Found", 0);
    }

    bool ProductExists(int id)
    {
        for (int i = 0; i < size; i++) {
            if (products[i].getId() == id)
                return true;
        }
        return false;
    }

    void removeProduct(int id)
    {
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

        for (int i = index; i < size - 1; i++) {
            products[i] = products[i + 1];
        }

        size--;
        cout << "Product Removed Successfully" << endl;
    }

    void UpdateProduct(int id, string name, double price)
    {
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

        if (!found) {
            cout << "Product Not Found" << endl;
        }
    }

    int getsize() const { return size; }
    bool isempty() const { return size == 0; }

    Product getProductAtIndex(int index)
    {
        if (index >= 0 && index < size)
            return products[index];

        return Product(-1, "Not Found", 0);
    }

    ~Catalogue() { delete[] products; }
};

class FileManager
{
public:
    void saveProducts(Catalogue &obj)
    {
        ofstream file("product.txt");

        if (!file) {
            cout << "Error: Unable to open file for writing." << endl;
            return;
        }

        for (int i = 0; i < obj.getsize(); i++) {
            Product p = obj.getProductAtIndex(i);
            file << p.getId() << "," << p.getname() << "," << p.getPrice() << endl;
        }

        file.close();
        cout << "Products saved successfully!" << endl;
    }

    void loadProduct(Catalogue &obj)
    {
        ifstream file("product.txt");

        if (!file) {
            cout << "Error: Unable to open file for reading." << endl;
            return;
        }

        int id;
        string name;
        double price;

        while (file >> id) {
            file.ignore();
            getline(file, name, ',');
            file >> price;

            obj.addProduct(Product(id, name, price));
        }

        file.close();
    }

    void appendProduct(Product p)
    {
        ofstream file("product.txt", ios::app);

        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        file << p.getId() << "," << p.getname() << "," << p.getPrice() << endl;

        file.close();
        cout << "Product appended successfully!" << endl;
    }

    void saveOrder(Product p, int quantity)
    {
        ofstream file("orders.txt", ios::app);

        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        file << p.getId() << "," << p.getname() << "," << p.getPrice() << "," << quantity << endl;

        file.close();
        cout << "Order saved successfully!" << endl;
    }

    void loadOrders()
    {
        ifstream file("orders.txt");

        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        int id, quantity;
        string name;
        double price;

        cout << "Order History:" << endl;

        while (file >> id) {
            file.ignore();
            getline(file, name, ',');
            file >> price;
            file.ignore();
            file >> quantity;

            cout << id << "|" << name << "|" << price << "|" << quantity << endl;
        }

        file.close();
    }
};