//
// Created by ali on 10/05/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CatalogueMenu.h" resolved

#include "cataloguemenu.h"
#include "ui_CatalogueMenu.h"


CatalogueMenu::CatalogueMenu(QWidget *parent) : QWidget(parent), ui(new Ui::CatalogueMenu) {
    ui->setupUi(this);
}

CatalogueMenu::~CatalogueMenu() {
    delete ui;
}