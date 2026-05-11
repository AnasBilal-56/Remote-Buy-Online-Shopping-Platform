#include "filemanager.h"
#include <QDebug>
#include <QDateTime>

FileManager::FileManager(const QString& filename)
    : m_filename(filename)
{
}

void FileManager::saveOrder(const Cart& cart, const QString& username, double total)
{
    QFile file(m_filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Could not open file for saving!";
        return;
    }

    QTextStream out(&file);
    int count = cart.getCount();
    Cartitem* items = cart.getItems();

    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    for (int i = 0; i < count; i++) {
        out << username << ","
            << dateTime << ","
            << items[i].item.getId() << ","
            << items[i].item.getName() << ","
            << items[i].item.getPrice() << ","
            << items[i].quantity << ","
            << total << "\n";
    }

    file.close();
}

QStringList FileManager::loadOrdersForUser(const QString& username)
{
    QStringList userOrders;
    QFile file(m_filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return userOrders;  // File doesn't exist yet - return empty
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith(username + ",")) {
            userOrders.append(line);
        }
    }

    file.close();
    return userOrders;
}