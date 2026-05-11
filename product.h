#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product
{
protected:
    int     id;
    QString name;
    double  price;
    QString category; // <--- NEW: Stores the section/category

public:
    Product();
    Product(int i, QString n, double p);           // without category for Admin
    Product(int i, QString n, double p, QString c); // Updated constructor
    virtual ~Product();

    virtual void Display() const;

    // Getters
    QString getName()     const;
    int     getId()       const;
    double  getPrice()    const;
    QString getCategory() const; // <--- NEW Getter

    // Setters
    void modifyName(QString n);
    void modifyPrice(double p);
    void modifyCategory(QString c); // <--- NEW Setter

    bool isValid() const;
    bool operator==(const Product& obj) const;
};

// ... (Electronics class stays the same, just calls the updated base constructor)
class Electronics : public Product
{
private:
    int warranty;

public:
    Electronics(int i, QString n, double p, QString c, int w);
    void Display() const override;
    int getWarranty() const;
};

#endif // PRODUCT_H