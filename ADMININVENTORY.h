#ifndef REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_ADMININVENTORY_H
#define REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_ADMININVENTORY_H

#include <QWidget>
#include <QString>
#include <QListWidgetItem>
#include "product.h"      // ← Include your clean Product header
#include "catalogue.h"    // ← Include your clean Catalogue header

namespace Ui {
class AdminInventory;
}

class AdminInventory : public QWidget
{
    Q_OBJECT

public:
    explicit AdminInventory(QWidget* parent = nullptr);
    ~AdminInventory();

    // Pointer to return to login window
    QWidget* loginWindowPtr = nullptr;

private slots:
    void on_btnAddProduct_clicked();
    void on_btnDelete_clicked();

private:
    Ui::AdminInventory* ui;
    Catalogue* myCatalogue;

    void updateVisualList();
};

#endif // REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_ADMININVENTORY_H