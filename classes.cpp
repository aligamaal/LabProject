
#include "classes.h"
#include <iostream>
#include <iomanip>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include <QAbstractItemView>

// ---- User ----
User::User(const QString& uname, const QString& pass, const QString& r)
    : username(uname), password(pass), role(r) {}

bool User::login(const QString& uname, const QString& pass) {
    return (username == uname && password == pass);
}

QString User::getUsername() const {
    return username;
}

QString User::getPassword() const {
    return password;
}

QString User::getRole() const {
    return role;
}

void User::setPassword(const QString& newPass) {
    password = newPass;
}

User::~User() {}  // Virtual destructor definition

// ---- Admin ----
Admin::Admin(const QString& uname, const QString& pass)
    : User(uname, pass, "Admin") {}

void Admin::displayMenu() const {
    std::cout << "Admin Menu\n";
}

// ---- Manager ----
Manager::Manager(const QString& uname, const QString& pass)
    : User(uname, pass, "Manager") {}

void Manager::displayMenu() const {
    std::cout << "Manager Menu\n";
}

// ---- Staff ----
Staff::Staff(const QString& uname, const QString& pass)
    : User(uname, pass, "Staff") {}

void Staff::displayMenu() const {
    std::cout << "Staff Menu\n";
}


// ---- Item Base Class ----
Item::Item(const QString& n, int qty, double pr, const QString& category_ , const QString& supplier_)
    : name(n),quantity(qty), price(pr), lowStockThreshold(5) , category(category_) , supplier(supplier_) {}


void Item::display() const {
    std::cout << "=== Item Details ===" << std::endl;
    std::cout << "Name: " << name.toStdString() << std::endl;
    std::cout << "Category: " << category.toStdString() << std::endl;
    std::cout << "Supplier: " << supplier.toStdString() << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Price: $" << std::fixed << std::setprecision(2) << price << std::endl;
    std::cout << "Low Stock Threshold: " << lowStockThreshold << std::endl;

    // Show stock status
    if (isLowStock()) {
        std::cout << "*** LOW STOCK WARNING ***" << std::endl;
    } else {
        std::cout << "Stock Status: Normal" << std::endl;
    }
    std::cout << "===================" << std::endl;
}
void Item::populateTable(QTableWidget* table, const std::vector<Item*>& itemList)
{
    table->setColumnCount(6);

    QStringList headers;
    headers << "Name" << "Category" << "Supplier" << "Quantity" << "Price" << "Stock Status";
    table->setHorizontalHeaderLabels(headers);

    table->setRowCount(itemList.size());

    for (int row = 0; row < itemList.size(); ++row) {
        Item* item = itemList[row];

        table->setItem(row, 0, new QTableWidgetItem(item->getName()));

        table->setItem(row, 1, new QTableWidgetItem(item->getCategory()));

        table->setItem(row, 2, new QTableWidgetItem(item->getSupplier()));

        table->setItem(row, 3, new QTableWidgetItem(QString::number(item->getQuantity())));

        table->setItem(row, 4, new QTableWidgetItem("$" + QString::number(item->getPrice(), 'f', 2)));

        QString stockStatus = item->isLowStock() ? "LOW STOCK" : "Normal";
        QTableWidgetItem* statusItem = new QTableWidgetItem(stockStatus);

        if (item->isLowStock()) {
            statusItem->setBackground(QBrush(QColor(255, 200, 200)));
            statusItem->setForeground(QBrush(QColor(150, 0, 0)));
        } else {
            statusItem->setBackground(QBrush(QColor(200, 255, 200)));
            statusItem->setForeground(QBrush(QColor(0, 100, 0)));
        }

        table->setItem(row, 5, statusItem);
    }

    table->resizeColumnsToContents();
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSortingEnabled(true);
}
QString Item::getName() const {
    return name;
}
int Item::getQuantity() const {
    return quantity;
}
double Item::getPrice() const {
    return price;
}
int Item::getLowStockThreshold() const{
    return lowStockThreshold;
}
QString Item::getCategory() const{
    return category;
}
QString Item::getSupplier() const{
    return supplier;
}
void Item::setQuantity(int qty) {
    quantity = qty;
}
void Item::setPrice(double pr) {
    price = pr;
}
void Item::setName(QString n){
    name = n;
}
void Item::setCategory(QString category_){
    category = category_;
}
 void Item::setLowStockThreshold(int threshold) {
     lowStockThreshold = threshold;
 }
 void Item::setSupplier(QString supplier_){
     supplier = supplier_;
 }
 Item::~Item() {}

 bool Item::isLowStock() const {
     return quantity <= lowStockThreshold;
 }


