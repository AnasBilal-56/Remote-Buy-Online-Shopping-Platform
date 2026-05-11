#ifndef CARTPAGE_H
#define CARTPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

// Important logic includes
#include "cart.h"
#include "catalogue.h"
#include "filemanager.h" // Needed for saving orders

class CartPage : public QWidget
{
    Q_OBJECT

public:
    // Updated constructor to match MainWindow's call
    explicit CartPage(Cart* cart, Catalogue* catalogue, FileManager* fm, QWidget* parent = nullptr);
    ~CartPage() = default;

    // Call this whenever the page is shown to update the UI
    void refreshTable();

signals:
    void goToCatalogue();     // Signal to switch back to Gallery
    void goToOrderHistory();  // Signal to switch to History

private slots:
    void onRemoveItem();
    void onUpdateQty();
    void onCheckout();

private:
    void setupUi();

    // Data Pointers (Points to the objects owned by MainWindow)
    Cart* m_cart;
    Catalogue* m_catalogue;
    FileManager* m_fm;        // Added this to fix the 'm_fm' errors

    // UI Elements
    QTableWidget* m_table;
    QLabel* m_totalLabel;

    QPushButton* m_checkoutBtn;
    QPushButton* m_removeBtn;
    QPushButton* m_updateBtn;
    QPushButton* m_backBtn;
    QPushButton* m_historyBtn;
};

#endif // CARTPAGE_H