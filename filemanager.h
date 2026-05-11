#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include "cart.h"

// Define the struct HERE instead of OrderHistory.h
// This ensures FileManager knows exactly what it is saving/loading
struct OrderRecord {
    int id;
    QString name;
    double price;
    int quantity;
};

class FileManager
{
public:
    FileManager() = default;

    // Saves the current cart items to orders.txt
    void saveOrder(Cart& cart);

    // Reads all orders from orders.txt
    QVector<OrderRecord> loadOrders();

    void clearHistory();

private:
    const QString m_filename = "orders.txt";
};

#endif // FILEMANAGER_H