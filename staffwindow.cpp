#include "staffwindow.h"
#include "ui_staffwindow.h"
#include "loginwindow.h"
#include "globals.h"


StaffWindow::StaffWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StaffWindow)
{
    ui->setupUi(this);
}

StaffWindow::~StaffWindow()
{
    delete ui;
}

void StaffWindow::on_pushButtonlogout_clicked()
{
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}


void StaffWindow::on_pushButtonViewInv_clicked()
{
    ui->listWidgetInv->clear(); // Clear old entries

    for (Item* item : inventoryList) {
        ui->listWidgetInv->addItem(item->display());
    }
}

