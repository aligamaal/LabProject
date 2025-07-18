#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QDialog>
#include <fstream>
#include "classes.h"
#include <vector>
#include <QRegularExpression>

// ADD FORWARD DECLARATION
class LoginWindow;

namespace Ui {
class ManagerWindow;
}

class ManagerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerWindow(QWidget *parent = nullptr);
    void setLoginWindow(LoginWindow* loginWindow);
    ~ManagerWindow();

private slots:
    void on_pushButtonlogout_clicked();
    void on_pushButtonViewinv_clicked();
    void on_pushButtonAdditem_clicked();
    void on_pushButtonReport_clicked();
    void on_pushButtonSearch_clicked();

private:
    Ui::ManagerWindow *ui;
    int currentEditIndex = -1;

    LoginWindow* loginWindowInstance = nullptr;

    std::vector<Item*> inventoryList;

    void exportReport(const QString &reportText);
    void exportAsCsv(const std::string &filePath);
    void saveInventoryToFile();
    void loadInventoryFromFile();

    std::vector<Item*> searchItems(const QString &searchText, const QString &searchType);
    void displaySearchResults(const std::vector<Item*> &results);

    // ADD THIS FUNCTION
    QString getCurrentLoggedInUser();
};



#endif // MANAGERWINDOW_H
