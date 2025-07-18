
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
    // Set up table columns
    table->setColumnCount(6);

    // Set column headers
    QStringList headers;
    headers << "Name" << "Category" << "Supplier" << "Quantity" << "Price" << "Stock Status";
    table->setHorizontalHeaderLabels(headers);

    // Set number of rows
    table->setRowCount(itemList.size());

    // Populate the table
    for (int row = 0; row < itemList.size(); ++row) {
        Item* item = itemList[row];

        // Column 0: Name
        table->setItem(row, 0, new QTableWidgetItem(item->getName()));

        // Column 1: Category
        table->setItem(row, 1, new QTableWidgetItem(item->getCategory()));

        // Column 2: Supplier
        table->setItem(row, 2, new QTableWidgetItem(item->getSupplier()));

        // Column 3: Quantity
        table->setItem(row, 3, new QTableWidgetItem(QString::number(item->getQuantity())));

        // Column 4: Price
        table->setItem(row, 4, new QTableWidgetItem("$" + QString::number(item->getPrice(), 'f', 2)));

        // Column 5: Stock Status
        QString stockStatus = item->isLowStock() ? "LOW STOCK" : "Normal";
        QTableWidgetItem* statusItem = new QTableWidgetItem(stockStatus);

        // Color code the stock status
        if (item->isLowStock()) {
            statusItem->setBackground(QBrush(QColor(255, 200, 200))); // Light red background
            statusItem->setForeground(QBrush(QColor(150, 0, 0)));     // Dark red text
        } else {
            statusItem->setBackground(QBrush(QColor(200, 255, 200))); // Light green background
            statusItem->setForeground(QBrush(QColor(0, 100, 0)));     // Dark green text
        }

        table->setItem(row, 5, statusItem);
    }

    // Make the table look better
    table->resizeColumnsToContents();  // Auto-size columns
    table->setAlternatingRowColors(true);  // Alternate row colors
    table->setSelectionBehavior(QAbstractItemView::SelectRows);  // Select entire rows
    table->setSortingEnabled(true);  // Allow column sorting
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


