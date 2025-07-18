#ifndef STAFFWINDOW_H
#define STAFFWINDOW_H

#include <QDialog>
#include "classes.h"
#include <vector>

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
    void setLoginWindow(LoginWindow* loginWindow);
    ~StaffWindow();

private slots:
    void on_pushButtonlogout_clicked();
    void on_pushButtonViewInv_clicked();

private:
    Ui::StaffWindow *ui;

    LoginWindow* loginWindowInstance = nullptr;
    std::vector<Item*> inventoryList;

    void loadInventoryFromFile();
    QString getCurrentLoggedInUser();
};

#endif // STAFFWINDOW_H
