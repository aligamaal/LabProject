#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "globals.h"
#include "adminwindow.h"
#include "staffwindow.h"
#include "managerwindow.h"
#include <QMessageBox>
#include <map>
LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}





void LoginWindow::on_Push_ButtonClear_clicked()
{
        ui->lineEditUser->setText("");
        ui->lineEditPassword->setText("");

}


void LoginWindow::on_pushButtonLogin_clicked()
{
    QString username = ui->lineEditUser->text();
    QString password = ui->lineEditPassword->text();

    if (usersMap.find(username) != usersMap.end() && usersMap[username]->getPassword() == password) {
        QString role = usersMap[username]->getRole();

        if (role == "admin") {
            AdminWindow *admin = new AdminWindow();
            admin->show();
            this->close(); // close login window
        } else if (role == "staff") {
            StaffWindow *staff = new StaffWindow();
            staff->show();
            this->close();
        } else if (role == "manager") {
            ManagerWindow *manager = new ManagerWindow();
            manager->show();
            this->close();
        }
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password");
           }
}

