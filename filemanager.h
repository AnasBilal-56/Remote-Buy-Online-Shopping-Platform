#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "cart.h"

class FileManager
{
public:
    FileManager(const QString& filename = "history.txt");

    // Save order to history file
    void saveOrder(const Cart& cart, const QString& username, double total);

    // Load orders for a specific user
    QStringList loadOrdersForUser(const QString& username);

private:
    QString m_filename;
};

#endif // FILEMANAGER_H