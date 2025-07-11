#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "globals.h"
#include "loginwindow.h"
#include <QMessageBox>
#include "managerwindow.h"

AdminWindow::AdminWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminWindow)
    ,m_managerWindow(nullptr)
{
    ui->setupUi(this);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_pushButtonLogout_clicked()
{
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}


void AdminWindow::on_pushButtonAddUser_clicked()
{
    QString uname = ui->lineEdituser->text();
    QString pass = ui->lineEditpassword->text();
    QString role = ui->comboBoxRole->currentText();

    if (usersMap.find(uname) != usersMap.end()) {
        QMessageBox::warning(this, "Error", "Username already exists.");
        return;
    }

    if (role == "Admin") usersMap[uname] = new Admin(uname, pass);
    else if (role == "Manager") usersMap[uname] = new Manager(uname, pass);
    else if (role == "Staff") usersMap[uname] = new Staff(uname, pass);

    QMessageBox::information(this, "Success", "User added successfully.");
}


void AdminWindow::on_pushButtonViewUsers_clicked()
{
    ui->textEditUsers->clear();

    if (usersMap.empty()) {
        ui->textEditUsers->setPlainText("No users found.");
        return;
    }

    QString display;
    for (const auto& pair : usersMap) {
        User* user1 = pair.second;
        display += "Username: " + user1->getUsername()
                   + " | Role: " + user1->getRole() + "\n";
    }

    ui->textEditUsers->setPlainText(display);
}


void AdminWindow::on_pushButtonRemoveUser_clicked()
{
    QString uname = ui->lineEdituser->text().trimmed();

    if (uname.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a username.");
        return;
    }

    auto it = usersMap.find(uname);
    if (it == usersMap.end()) {
        QMessageBox::warning(this, "Error", "User not found.");
        return;
    }

    delete it->second;
    usersMap.erase(it);

    QMessageBox::information(this, "Success", "User removed successfully.");
    ui->lineEdituser->clear();
}


void AdminWindow::on_pushButton_clicked()
{
    if (!m_managerWindow) {
        m_managerWindow = new ManagerWindow(this); // 'this' for parent-child relationship
    }

    // Show the manager window and hide this one
    m_managerWindow->show();
    this->hide();
}

