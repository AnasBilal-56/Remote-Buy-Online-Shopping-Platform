//
// Created by ali on 10/05/2026.
//

#ifndef REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_CATALOGUEMENU_H
#define REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_CATALOGUEMENU_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class CatalogueMenu;
}

QT_END_NAMESPACE

class CatalogueMenu : public QWidget {
    Q_OBJECT

public:
    explicit CatalogueMenu(QWidget *parent = nullptr);

    ~CatalogueMenu() override;

private:
    Ui::CatalogueMenu *ui;
};


#endif //REMOTE_BUY_ONLINE_SHOPPING_PLATFORM_CATALOGUEMENU_H