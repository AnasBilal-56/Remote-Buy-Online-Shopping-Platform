#include "catalogue.h"
#include "catalogue.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Constructor
Catalogue::Catalogue(int cap)
{
    capacity = cap;
    size = 0;
    products = new Product*[cap];
}

// Destructor
Catalogue::~Catalogue()
{
    for (int i = 0; i < size; i++)
    {
        delete products[i];
    }
    delete[] products;
}

// Add a product pointer
void Catalogue::addProduct(Product* p)
{
    if (size < capacity)
    {
        products[size] = p;
        size++;
    }
}

// Remove a product by ID and shift the array
void Catalogue::removeProduct(int id)
{
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        if (products[i]->getId() == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1) return;

    delete products[index];

    // Shift remaining elements left to fill the gap
    for (int i = index; i < size - 1; i++)
    {
        products[i] = products[i + 1];
    }
    size--;
}

// Get current size
int Catalogue::getSize() const
{
    return size;
}

// Get product at a specific array index
Product* Catalogue::getProductAtIndex(int index)
{
    if (index >= 0 && index < size)
    {
        return products[index];
    }
    return nullptr;
}

// Check if a product ID exists in the catalogue
bool Catalogue::exists(int id)
{
    for (int i = 0; i < size; i++)
    {
        if (products[i]->getId() == id)
        {
            return true;
        }
    }
    return false;
}

// Check if catalogue is empty
bool Catalogue::isEmpty() const
{
    return size == 0;
}
Product* Catalogue::findProductById(int id) const
{
    for (int i = 0; i < size; i++) {
        if (products[i]->getId() == id)
            return products[i];
    }
    return nullptr;
}
// ====================== FILE OPERATIONS ======================
void Catalogue::saveToFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << filename;
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < size; i++) {
        Product* p = products[i];
        if (p) {
            out << p->getId() << ","
                << p->getName() << ","
                << p->getPrice() << "\n";
        }
    }
    file.close();
}

void Catalogue::loadFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // Clear existing products before loading
    for (int i = 0; i < size; i++) {
        delete products[i];
    }
    size = 0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList fields = line.split(",");
        if (fields.size() >= 3) {
            int id = fields[0].trimmed().toInt();
            QString name = fields[1].trimmed();
            double price = fields[2].trimmed().toDouble();

            if (id != 0 || fields[0].trimmed() == "0") {
                addProduct(new Product(id, name, price));
            }
        }
    }
    file.close();
}