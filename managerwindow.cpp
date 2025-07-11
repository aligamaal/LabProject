#include "managerwindow.h"
#include "ui_managerwindow.h"
#include "loginwindow.h"
#include "globals.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>

ManagerWindow::ManagerWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManagerWindow)
{
    ui->setupUi(this);
}

ManagerWindow::~ManagerWindow()
{
    delete ui;
}

void ManagerWindow::on_pushButtonlogout_clicked()
{
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}


void ManagerWindow::on_pushButtonViewinv_clicked()
{
    ui->listWidgetInv->clear(); // Clear old entries

    for (Item* item : inventoryList) {
        ui->listWidgetInv->addItem(item->display());
    }
}


void ManagerWindow::on_pushButtonAdditem_clicked()
{
    QString id = ui->lineEditItemID->text();
    QString name = ui->lineEditName->text();
    QString desc = ui->lineEditDesc->text();
    int qty = ui->spinBoxQuantity->value();
    double price = ui->doubleSpinBoxPrice->value();

    QString itemType = ui->comboBoxItemType->currentText();

    if (itemType == "Product") {
        QString brand = ui->lineEditBrand->text();
        QString category = ui->lineEditCategory->text();

        if (id.isEmpty() || name.isEmpty() || brand.isEmpty() || category.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all Product fields.");
            return;
        }

        Product* p = new Product(id, name, desc, qty, price, brand, category);
        inventoryList.push_back(p);
        QMessageBox::information(this, "Added", "Product added to inventory.");
    }
    else if (itemType == "Supply") {
        QString supplier = ui->lineEditsup->text();
        bool consumable = ui->checkBoxConsumable->isChecked();

        if (id.isEmpty() || name.isEmpty() || supplier.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all Supply fields.");
            return;
        }

        Supply* s = new Supply(id, name, desc, qty, price, supplier, consumable);
        inventoryList.push_back(s);
        QMessageBox::information(this, "Added", "Supply added to inventory.");
    }

    // Clear fields
    ui->lineEditItemID->clear();
    ui->lineEditName->clear();
    ui->lineEditDesc->clear();
    ui->spinBoxQuantity->setValue(0);
    ui->doubleSpinBoxPrice->setValue(0.0);
    ui->lineEditBrand->clear();
    ui->lineEditCategory->clear();
    ui->lineEditsup->clear();
    ui->checkBoxConsumable->setChecked(false);
}




#include <fstream>
#include <string>

void ManagerWindow::on_pushButtonReport_clicked()
{
    // 1. Generate inventory reports
    int totalItems = 0;
    QVector<Item*> lowStockItems;

    for (Item* item : inventoryList) {
        totalItems += item->getQuantity();
        if (item->isLowStock()) {
            lowStockItems.append(item);
        }
    }

    // 2. Prepare report text
    QString reportText = "INVENTORY REPORT\n\n";
    reportText += QString("Total items in stock: %1\n\n").arg(totalItems);

    if (!lowStockItems.isEmpty()) {
        reportText += "LOW STOCK ITEMS:\n";
        for (Item* item : lowStockItems) {
            reportText += QString("- %1 (ID: %2, Qty: %3)\n")
            .arg(item->getName())
                .arg(item->getItemID())
                .arg(item->getQuantity());
        }
    } else {
        reportText += "No low stock items.\n";
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
        // Using ofstream for text file export
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
        // Write CSV header
        csvFile << "ID,Name,Quantity,Type,Details\n";

        // Write data rows
        for (Item* item : inventoryList) {
            std::string details;
            if (auto product = dynamic_cast<Product*>(item)) {
                details = "Brand:" + product->getBrand().toStdString() +
                          ",Category:" + product->getCategory().toStdString();
            } else if (auto supply = dynamic_cast<Supply*>(item)) {
                details = "Supplier:" + supply->getSupplier().toStdString() +
                          ",Consumable:" + (supply->isConsumable() ? "Yes" : "No");
            }

            csvFile << item->getItemID().toStdString() << ","
                    << "\"" << item->getName().toStdString() << "\","
                    << item->getQuantity() << ","
                    << item->getType().toStdString() << ","
                    << "\"" << details << "\"\n";
        }

        csvFile.close();
        QMessageBox::information(this, "Success", "CSV report exported successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Could not save CSV file.");
    }
}
