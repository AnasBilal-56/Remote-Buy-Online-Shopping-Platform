#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include "filemanager.h" // Include back the FileManager

class OrderHistoryPage : public QWidget
{
    Q_OBJECT

public:
    // Constructor now takes the FileManager pointer
    explicit OrderHistoryPage(FileManager* fm, QWidget* parent = nullptr);

    // This will now fetch from the file
    void refreshOrders();

signals:
    void goBack();

private slots:
    void onRefresh();

private:
    void setupUi();

    FileManager* m_fm; // Pointer to the shared file manager
    QTableWidget* m_table;
    QLabel* m_totalLabel;
    QLabel* m_emptyLabel;
    QPushButton* m_refreshBtn;
    QPushButton* m_backBtn;
};