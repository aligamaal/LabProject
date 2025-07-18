#include "managerwindow.h"
#include "ui_managerwindow.h"
#include "loginwindow.h"
#include "globals.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <fstream>
#include <string>
#include <sstream>
#include <QInputDialog>

ManagerWindow::ManagerWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManagerWindow)
    , loginWindowInstance(nullptr)  // Initialize to nullptr
{
    ui->setupUi(this);
    loadInventoryFromFile();
}
void ManagerWindow::setLoginWindow(LoginWindow* loginWindow)
{
    loginWindowInstance = loginWindow;

    // Update the welcome message when login window is set
    QString currentUser = getCurrentLoggedInUser();
    if (!currentUser.isEmpty()) {
        ui->label->setText(QString("Welcome %1!").arg(currentUser));
        qDebug() << "Welcome message set for manager:" << currentUser;
    } else {
        ui->label->setText("Welcome Manager!");
    }
}
ManagerWindow::~ManagerWindow()
{
    // Clean up inventory items
    for (Item* item : inventoryList) {
        delete item;
    }
    inventoryList.clear();
    delete ui;
}

void ManagerWindow::loadInventoryFromFile()
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

void ManagerWindow::saveInventoryToFile()
{
    std::ofstream file("inventory.txt");
    if (!file.is_open()) {
        qDebug() << "Failed to open inventory file for saving";
        return;
    }

    // Write item count
    file << inventoryList.size() << std::endl;

    // Write header
    file << "Name|Category|Supplier|Quantity|Price|Threshold" << std::endl;

    // Write inventory items
    for (Item* item : inventoryList) {
        file << item->getName().toStdString() << "|"
             << item->getCategory().toStdString() << "|"
             << item->getSupplier().toStdString() << "|"
             << item->getQuantity() << "|"
             << item->getPrice() << "|"
             << item->getLowStockThreshold() << std::endl;
    }

    file.close();
    qDebug() << "Inventory saved to file. Items:" << inventoryList.size();
}

void ManagerWindow::on_pushButtonlogout_clicked()
{
    saveInventoryToFile();  // Save before logout
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}

void ManagerWindow::on_pushButtonViewinv_clicked()
{
    Item::populateTable(ui->tableWidget, inventoryList);
}

void ManagerWindow::on_pushButtonAdditem_clicked()
{
    QString name = ui->lineEditName->text().trimmed();
    int qty = ui->spinBoxQuantity->value();
    double price = ui->doubleSpinBoxPrice->value();
    QString category = ui->lineEditCategory->text().trimmed();
    QString supply = ui->lineEditsup->text().trimmed();

    // Enhanced validation with specific error messages
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Item name cannot be empty.");
        ui->lineEditName->setFocus();
        return;
    }

    if (category.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Category cannot be empty.");
        ui->lineEditCategory->setFocus();
        return;
    }

    if (supply.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Supplier cannot be empty.");
        ui->lineEditsup->setFocus();
        return;
    }

    // Check for negative or zero quantity
    if (qty <= 0) {
        QMessageBox::warning(this, "Input Error", "Quantity must be greater than 0.");
        ui->spinBoxQuantity->setFocus();
        ui->spinBoxQuantity->selectAll();
        return;
    }

    // Check for negative or zero price
    if (price <= 0.0) {
        QMessageBox::warning(this, "Input Error", "Price must be greater than 0.00.");
        ui->doubleSpinBoxPrice->setFocus();
        ui->doubleSpinBoxPrice->selectAll();
        return;
    }

    // Check for reasonable maximum values to prevent overflow
    if (qty > 999999) {
        QMessageBox::warning(this, "Input Error", "Quantity cannot exceed 999,999.");
        ui->spinBoxQuantity->setFocus();
        return;
    }

    if (price > 999999.99) {
        QMessageBox::warning(this, "Input Error", "Price cannot exceed $999,999.99.");
        ui->doubleSpinBoxPrice->setFocus();
        return;
    }

    // Enhanced duplicate check - check both name and category combination
    for (Item* existingItem : inventoryList) {
        if (existingItem->getName().toLower() == name.toLower() &&
            existingItem->getCategory().toLower() == category.toLower()) {
            QMessageBox::warning(this, "Duplicate Item",
                                 QString("An item with name '%1' in category '%2' already exists.\n"
                                         "Please use a different name or category combination.")
                                     .arg(name).arg(category));
            ui->lineEditName->setFocus();
            ui->lineEditName->selectAll();
            return;
        }
    }

    // Validate name contains only appropriate characters (letters, numbers, spaces, common symbols)
    QRegularExpression nameRegex("^[a-zA-Z0-9\\s\\-_().&]+$");
    if (!nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, "Input Error",
                             "Item name contains invalid characters.\n"
                             "Only letters, numbers, spaces, hyphens, underscores, parentheses, periods, and ampersands are allowed.");
        ui->lineEditName->setFocus();
        return;
    }

    // Validate category contains only appropriate characters
    if (!nameRegex.match(category).hasMatch()) {
        QMessageBox::warning(this, "Input Error",
                             "Category contains invalid characters.\n"
                             "Only letters, numbers, spaces, hyphens, underscores, parentheses, periods, and ampersands are allowed.");
        ui->lineEditCategory->setFocus();
        return;
    }

    // Validate supplier contains only appropriate characters
    if (!nameRegex.match(supply).hasMatch()) {
        QMessageBox::warning(this, "Input Error",
                             "Supplier name contains invalid characters.\n"
                             "Only letters, numbers, spaces, hyphens, underscores, parentheses, periods, and ampersands are allowed.");
        ui->lineEditsup->setFocus();
        return;
    }

    try {
        // Create and add new item
        Item* item = new Item(name, qty, price, category, supply);
        inventoryList.push_back(item);

        // Save to file immediately
        saveInventoryToFile();

        QMessageBox::information(this, "Success",
                                 QString("Item '%1' has been successfully added to inventory.").arg(name));

        // Clear fields after successful addition
        ui->lineEditName->clear();
        ui->spinBoxQuantity->setValue(0);
        ui->doubleSpinBoxPrice->setValue(0.0);
        ui->lineEditCategory->clear();
        ui->lineEditsup->clear();

        // Set focus back to name field for next entry
        ui->lineEditName->setFocus();

        // Refresh the table if it's currently displayed
        if (ui->tableWidget->rowCount() > 0) {
            Item::populateTable(ui->tableWidget, inventoryList);
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error",
                              QString("Failed to add item due to an unexpected error:\n%1").arg(e.what()));
    }
    catch (...) {
        QMessageBox::critical(this, "Error",
                              "Failed to add item due to an unknown error.");
    }
}

void ManagerWindow::on_pushButtonReport_clicked()
{
    // 1. Generate inventory reports
    int totalItemTypes = inventoryList.size();
    int lowStockCount = 0;
    QVector<Item*> lowStockItems;

    for (Item* item : inventoryList) {
        if (item->isLowStock()) {
            lowStockItems.append(item);
            lowStockCount++;
        }
    }

    // 2. Prepare report text
    QString reportText = "INVENTORY REPORT\n";
    reportText += QString("Total Item Types: %1\n").arg(totalItemTypes);
    reportText += QString("Low Stock Items: %1\n\n").arg(lowStockCount);

    if (!lowStockItems.isEmpty()) {
        reportText += "LOW STOCK ITEMS DETAILS\n";
        reportText += "Name | Category | Supplier | Current Quantity | Threshold\n";
        reportText += "----------------------------------------------------------------\n";
        for (Item* item : lowStockItems) {
            reportText += QString("%1 | %2 | %3 | %4 | %5\n")
            .arg(item->getName())
                .arg(item->getCategory())
                .arg(item->getSupplier())
                .arg(item->getQuantity())
                .arg(item->getLowStockThreshold());
        }
    } else {
        reportText += "LOW STOCK ITEMS DETAILS\n";
        reportText += "No items are currently low on stock.\n";
    }

    // 3. Show the report
    QMessageBox::information(this, "Inventory Report", reportText);

    // 4. Offer to export
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Export Report",
        "Would you like to export this report?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        exportReport(reportText);
    }
}

void ManagerWindow::on_pushButtonSearch_clicked()
{
    // Create a simple input dialog for search
    bool ok;
    QStringList searchOptions;
    searchOptions << "Name" << "Category" << "Supplier";

    QString searchType = QInputDialog::getItem(this, "Search Type",
                                               "Select search criteria:",
                                               searchOptions, 0, false, &ok);

    if (!ok || searchType.isEmpty()) {
        return; // User cancelled
    }

    QString searchText = QInputDialog::getText(this, "Search Items",
                                               QString("Enter %1 to search for:").arg(searchType.toLower()),
                                               QLineEdit::Normal, "", &ok);

    if (!ok || searchText.trimmed().isEmpty()) {
        return; // User cancelled or entered empty text
    }

    // Perform search
    std::vector<Item*> results = searchItems(searchText.trimmed(), searchType);

    // Display results
    if (results.empty()) {
        QMessageBox::information(this, "Search Results",
                                 QString("No items found matching '%1' in %2.")
                                     .arg(searchText).arg(searchType.toLower()));
    } else {
        displaySearchResults(results);
        QMessageBox::information(this, "Search Results",
                                 QString("Found %1 item(s) matching '%2' in %3.")
                                     .arg(results.size()).arg(searchText).arg(searchType.toLower()));
    }
}

std::vector<Item*> ManagerWindow::searchItems(const QString &searchText, const QString &searchType)
{
    std::vector<Item*> results;
    QString lowerSearchText = searchText.toLower();

    for (Item* item : inventoryList) {
        bool matches = false;

        if (searchType == "Name") {
            matches = item->getName().toLower().contains(lowerSearchText);
        } else if (searchType == "Category") {
            matches = item->getCategory().toLower().contains(lowerSearchText);
        } else if (searchType == "Supplier") {
            matches = item->getSupplier().toLower().contains(lowerSearchText);
        }

        if (matches) {
            results.push_back(item);
        }
    }

    return results;
}

void ManagerWindow::displaySearchResults(const std::vector<Item*> &results)
{
    // Use the existing table to display search results
    Item::populateTable(ui->tableWidget, results);
}

void ManagerWindow::exportReport(const QString &reportText)
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save Report",
        QDir::homePath() + "/inventory_report.txt",
        "Text Files (*.txt);;CSV Files (*.csv)"
        );

    if (filePath.isEmpty()) return;

    if (filePath.endsWith(".csv", Qt::CaseInsensitive)) {
        exportAsCsv(filePath.toStdString());
    } else {
        std::ofstream outFile(filePath.toStdString());
        if (outFile.is_open()) {
            outFile << reportText.toStdString();
            outFile.close();
            QMessageBox::information(this, "Success", "Report exported successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Could not save file.");
        }
    }
}

void ManagerWindow::exportAsCsv(const std::string &filePath)
{
    std::ofstream csvFile(filePath);
    if (csvFile.is_open()) {
        int totalItemTypes = inventoryList.size();
        int lowStockItems = 0;

        for (Item* item : inventoryList) {
            if (item->isLowStock()) {
                lowStockItems++;
            }
        }

        csvFile << "Total Item Types," << totalItemTypes << "\n";
        csvFile << "Low Stock Items," << lowStockItems << "\n";
        csvFile << "\n";

        csvFile << "LOW STOCK ITEMS DETAILS\n";
        csvFile << "Name,Category,Supplier,Current Quantity,Threshold\n";

        for (Item* item : inventoryList) {
            if (item->isLowStock()) {
                csvFile << "\"" << item->getName().toStdString() << "\","
                        << "\"" << item->getCategory().toStdString() << "\","
                        << "\"" << item->getSupplier().toStdString() << "\","
                        << item->getQuantity() << ","
                        << item->getLowStockThreshold() << "\n";
            }
        }

        csvFile.close();
        QMessageBox::information(this, "Success", "CSV report exported successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Could not save CSV file.");
    }
}
QString ManagerWindow::getCurrentLoggedInUser()
{
    // Get current user from LoginWindow instance
    if (loginWindowInstance) {
        return loginWindowInstance->getCurrentUser();
    }

    // Fallback if loginWindowInstance is null
    return "Manager User";
}
