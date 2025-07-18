#ifndef STAFFWINDOW_H
#define STAFFWINDOW_H

#include <QDialog>
#include "classes.h"  // Add this include
#include <vector>     // Add this include

// ADD FORWARD DECLARATION
class LoginWindow;

namespace Ui {
class StaffWindow;
}

class StaffWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StaffWindow(QWidget *parent = nullptr);
    void setLoginWindow(LoginWindow* loginWindow);  // Add this method
    ~StaffWindow();

private slots:
    void on_pushButtonlogout_clicked();
    void on_pushButtonViewInv_clicked();

private:
    Ui::StaffWindow *ui;

    // ADD THESE MEMBER VARIABLES
    LoginWindow* loginWindowInstance = nullptr;
    std::vector<Item*> inventoryList;

    // ADD THESE HELPER FUNCTIONS
    void loadInventoryFromFile();
    QString getCurrentLoggedInUser();
};

#endif // STAFFWINDOW_H
