#include "filemanager.h"
#include <QDebug>

void FileManager::saveOrder(Cart& cart)
{
    // Open in Append mode so we don't delete old orders
    QFile file(m_filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Could not open file for saving!";
        return;
    }

    QTextStream out(&file);
    Cartitem* items = cart.items();

    for (int i = 0; i < cart.itemCount(); i++) {
        // Save format: ID | Name | Price | Quantity
        out << items[i].item.getId() << ","
            << items[i].item.getName() << ","
            << items[i].item.getPrice() << ","
            << (int)items[i].quantity << "\n";
    }

    file.close();
}

QVector<OrderRecord> FileManager::loadOrders()
{
    QVector<OrderRecord> records;
    QFile file(m_filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return records; // Return empty if no file exists yet
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() == 4) {
            OrderRecord rec;
            rec.id = fields[0].toInt();
            rec.name = fields[1];
            rec.price = fields[2].toDouble();
            rec.quantity = fields[3].toInt();
            records.append(rec);
        }
    }

    file.close();
    return records;
}

void FileManager::clearHistory()
{
    QFile file(m_filename);
    file.remove();
}