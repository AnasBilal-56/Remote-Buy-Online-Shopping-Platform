// ADMININVENTORY.h
#ifndef ADMININVENTORY_H
#define ADMININVENTORY_H

#include <QMainWindow>
#include "Catalogue.h"     // Make sure these are included
#include "product.h"

class MainWindow;   // Forward declaration

namespace Ui {
class AdminInventory;
}

class AdminInventory : public QMainWindow
{
    Q_OBJECT
signals:
    void inventoryChanged();
public:
    explicit AdminInventory(QWidget *parent = nullptr);
    ~AdminInventory();

    MainWindow* loginWindowPtr = nullptr;

private slots:
    void on_btnAddProduct_clicked();
    void on_btnDelete_clicked();
    void on_btnLogout_clicked();

private:
    Ui::AdminInventory *ui;
    Catalogue* myCatalogue = nullptr;

    void updateVisualList();
};

#endif // ADMININVENTORY_H