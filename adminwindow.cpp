#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h"
#include <QMessageBox>
#include "managerwindow.h"
#include <QFile>
#include <QDataStream>
#include <QDir>

AdminWindow::AdminWindow(LoginWindow* loginWindow, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
    , m_managerWindow(nullptr)
    , loginWindowInstance(loginWindow)
{
    ui->setupUi(this);
    qDebug() << "AdminWindow constructor called";


    QString currentUser = getCurrentLoggedInUser();
    if (!currentUser.isEmpty()) {
        ui->labelAdmin->setText(QString("Welcome %1!").arg(currentUser));
        qDebug() << "Welcome message set for user:" << currentUser;
    } else {
        ui->labelAdmin->setText("Welcome Admin!");
        qDebug() << "No current user found, using default welcome message";
    }
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_pushButtonLogout_clicked()
{
    loginWindowInstance->show();
    this->close();
}

void AdminWindow::on_pushButtonAddUser_clicked()
{
    QString uname = ui->lineEdituser->text().trimmed();
    QString pass = ui->lineEditpassword->text();
    QString role = ui->comboBoxRole->currentText();

    if (uname.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username cannot be empty.");
        ui->lineEdituser->setFocus();
        return;
    }

    if (uname.length() < 3) {
        QMessageBox::warning(this, "Input Error", "Username must be at least 3 characters long.");
        ui->lineEdituser->setFocus();
        ui->lineEdituser->selectAll();
        return;
    }

    if (uname.length() > 20) {
        QMessageBox::warning(this, "Input Error", "Username cannot exceed 20 characters.");
        ui->lineEdituser->setFocus();
        ui->lineEdituser->selectAll();
        return;
    }

    // Check for valid username characters
    QRegularExpression usernameRegex("^[a-zA-Z0-9_]+$");
    if (!usernameRegex.match(uname).hasMatch()) {
        QMessageBox::warning(this, "Input Error",
                             "Username can only contain letters, numbers, and underscores.\n"
                             "No spaces or special characters allowed.");
        ui->lineEdituser->setFocus();
        ui->lineEdituser->selectAll();
        return;
    }

    if (pass.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Password cannot be empty.");
        ui->lineEditpassword->setFocus();
        return;
    }

    if (pass.length() != 8) {
        QMessageBox::warning(this, "Input Error",
                             QString("Password must be exactly 8 characters long.\nCurrent length: %1 characters.")
                                 .arg(pass.length()));
        ui->lineEditpassword->setFocus();
        ui->lineEditpassword->selectAll();
        return;
    }

    for (int i = 0; i < pass.length(); i++) {
        QChar c = pass.at(i);
        if (!c.isLetterOrNumber()) {
            QMessageBox::warning(this, "Input Error",
                                 QString("Password cannot contain special characters.\n"
                                         "Invalid character '%1' found at position %2.\n"
                                         "Only letters and numbers are allowed.")
                                     .arg(c).arg(i + 1));
            ui->lineEditpassword->setFocus();
            ui->lineEditpassword->selectAll();
            return;
        }
    }

    if (role.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a role for the user.");
        ui->comboBoxRole->setFocus();
        return;
    }

    try {
        if (loginWindowInstance->addUser(uname, pass, role)) {
            QMessageBox::information(this, "Success",
                                     QString("User '%1' with role '%2' has been successfully added.")
                                         .arg(uname).arg(role));

            ui->lineEdituser->clear();
            ui->lineEditpassword->clear();
            ui->comboBoxRole->setCurrentIndex(0);

            ui->lineEdituser->setFocus();
        } else {
            QMessageBox::warning(this, "Error",
                                 QString("Failed to add user '%1'.\nUsername may already exist or there was a system error.")
                                     .arg(uname));
            ui->lineEdituser->setFocus();
            ui->lineEdituser->selectAll();
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error",
                              QString("An unexpected error occurred while adding user:\n%1").arg(e.what()));
    }
    catch (...) {
        QMessageBox::critical(this, "Error",
                              "An unknown error occurred while adding user.");
    }
}

void AdminWindow::on_pushButtonViewUsers_clicked()
{
    ui->textEditUsers->clear();


    QString userList = loginWindowInstance->getUserList();
    ui->textEditUsers->setPlainText(userList);
}

void AdminWindow::on_pushButtonRemoveUser_clicked()
{
    QString uname = ui->lineEdituser->text().trimmed();
    if (uname.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a username.");
        return;
    }

    if (loginWindowInstance->removeUser(uname)) {
        QMessageBox::information(this, "Success", "User removed successfully.");
        ui->lineEdituser->clear();
    } else {
        QMessageBox::warning(this, "Error", "User not found.");
    }
}

void AdminWindow::on_pushButton_clicked()
{
    if (!m_managerWindow) {
        m_managerWindow = new ManagerWindow();
    }
    m_managerWindow->show();
    this->hide();
}

QString AdminWindow::getCurrentLoggedInUser()
{
    if (loginWindowInstance) {
        return loginWindowInstance->getCurrentUser();
    }

    return "Admin User";
}

