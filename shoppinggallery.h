#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "catalogue.h"
#include "cart.h"

class ShoppingGallery : public QWidget
{
    Q_OBJECT

public:
    explicit ShoppingGallery(Catalogue* cat, Cart* cart, QWidget* parent = nullptr);
    void refreshGallery(QString filter = "");

signals:
    void goToCart(); // Signal to switch to the Cart page

private slots:
    void onSearchChanged(const QString& text);
    void onAddToCart();

private:
    void setupUi();

    Catalogue* m_catalogue;
    Cart* m_cart;

    QLineEdit* m_searchBar;
    QTableWidget* m_table;
    QPushButton* m_viewCartBtn;
};