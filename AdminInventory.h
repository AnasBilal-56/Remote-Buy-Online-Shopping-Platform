

#ifndef REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_ADMININVENTORY_H
#define REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_ADMININVENTORY_H
#include <QWidget>
#include <QString>
#include <QListWidgetItem>
#include <QDebug>
namespace Ui {
    class AdminInventory;
}
class Product {
protected:
    int id;
    QString name;
    double price;

public:
    Product();
    Product(int i, QString n, double p);
    virtual ~Product() {} // Virtual destructor is critical for inheritance

    virtual void Display() const;

    QString getName()  const;
    int     getId()    const;
    double  getPrice() const;

    void modifyName(QString n);
    void modifyPrice(double p);
};

class Electronics : public Product {
private:
    int warranty;

public:
    Electronics(int i, QString n, double p, int w);
    void Display() const override;
    int getWarranty() const { return warranty; }
};
class Catalogue {
private:
    Product** products;
    int size;
    int capacity;

public:
    Catalogue(int cap);
    ~Catalogue();

    void     addProduct(Product* p);
    void     removeProduct(int id);
    int      getSize() const;
    Product* getProductAtIndex(int index);

    void saveToFile(QString filename);
    void loadFromFile(QString filename);
};

class AdminInventory : public QWidget {
    Q_OBJECT

public:
    explicit AdminInventory(QWidget* parent = nullptr);
    ~AdminInventory();

private slots:
    void on_btnAddProduct_clicked();
    void on_btnDelete_clicked();

private:
    Ui::AdminInventory* ui;
    Catalogue* myCatalogue;
    void updateVisualList();
};

#endif // REMOTE_BUY_ONLINE_SHOPPING_PLATFORMADM_ININVENTORY_H