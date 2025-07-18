#include "staffwindow.h"
#include "ui_staffwindow.h"
#include "loginwindow.h"
#include "globals.h"
#include <QDebug>
#include <fstream>
#include <string>
#include <sstream>

StaffWindow::StaffWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StaffWindow)
    , loginWindowInstance(nullptr)  // Initialize to nullptr
{
    ui->setupUi(this);
    loadInventoryFromFile();  // Load inventory when window opens
}

void StaffWindow::setLoginWindow(LoginWindow* loginWindow)
{
    loginWindowInstance = loginWindow;

    // Update the welcome message when login window is set
    QString currentUser = getCurrentLoggedInUser();
    if (!currentUser.isEmpty()) {
        // Assuming you have a label named "label" in your UI
        ui->label->setText(QString("Welcome %1!").arg(currentUser));
        qDebug() << "Welcome message set for staff:" << currentUser;
    } else {
        ui->label->setText("Welcome Staff!");
    }
}

StaffWindow::~StaffWindow()
{
    // Clean up inventory items
    for (Item* item : inventoryList) {
        delete item;
    }
    inventoryList.clear();
    delete ui;
}

QString StaffWindow::getCurrentLoggedInUser()
{
    // Get current user from LoginWindow instance
    if (loginWindowInstance) {
        return loginWindowInstance->getCurrentUser();
    }

    // Fallback if loginWindowInstance is null
    return "Staff User";
}

void StaffWindow::loadInventoryFromFile()
{
    // Clear existing inventory
    for (Item* item : inventoryList) {
        delete item;
    }
    inventoryList.clear();

    std::ifstream file("inventory.txt");
    if (!file.is_open()) {
        qDebug() << "Inventory file not found. Starting with empty inventory.";
        return;
    }

    std::string line;
    int itemCount = 0;

    // Read item count
    if (std::getline(file, line)) {
        try {
            itemCount = std::stoi(line);
            qDebug() << "Loading" << itemCount << "items from inventory file";
        } catch (...) {
            qDebug() << "Error reading item count";
            file.close();
            return;
        }
    }

    // Skip header line
    if (!std::getline(file, line)) {
        qDebug() << "No header line found";
        file.close();
        return;
    }

    // Read inventory items
    for (int i = 0; i < itemCount && std::getline(file, line); i++) {
        std::istringstream iss(line);
        std::string name, category, supplier, qtyStr, priceStr, thresholdStr;

        // Parse: Name|Category|Supplier|Quantity|Price|Threshold
        if (std::getline(iss, name, '|') &&
            std::getline(iss, category, '|') &&
            std::getline(iss, supplier, '|') &&
            std::getline(iss, qtyStr, '|') &&
            std::getline(iss, priceStr, '|') &&
            std::getline(iss, thresholdStr)) {

            try {
                int qty = std::stoi(qtyStr);
                double price = std::stod(priceStr);
                int threshold = std::stoi(thresholdStr);

                Item* item = new Item(QString::fromStdString(name), qty, price,
                                      QString::fromStdString(category),
                                      QString::fromStdString(supplier));
                item->setLowStockThreshold(threshold);
                inventoryList.push_back(item);

                qDebug() << "Loaded item:" << QString::fromStdString(name);
            } catch (...) {
                qDebug() << "Error parsing item data:" << QString::fromStdString(line);
            }
        }
    }

    file.close();
    qDebug() << "Successfully loaded" << inventoryList.size() << "items";
}

void StaffWindow::on_pushButtonlogout_clicked()
{
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}

void StaffWindow::on_pushButtonViewInv_clicked()
{
    // Display the inventory in the table widget
    Item::populateTable(ui->tableWidget, inventoryList);
}
