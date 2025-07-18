#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h"
#include <QMessageBox>
#include "managerwindow.h"
#include <QFile>
#include <QDataStream>
#include <QDir>

AdminWindow::AdminWindow(LoginWindow* loginWindow, QWidget *parent)
    : QMainWindow(parent)  // Match the base class type
    , ui(new Ui::AdminWindow)
    , m_managerWindow(nullptr)
    , loginWindowInstance(loginWindow)  // Initialize member variable
{
    ui->setupUi(this);
    qDebug() << "AdminWindow constructor called";

    // Get the current logged-in username and display it
    QString currentUser = getCurrentLoggedInUser();
    if (!currentUser.isEmpty()) {
        ui->labelAdmin->setText(QString("Welcome %1!").arg(currentUser));
        qDebug() << "Welcome message set for user:" << currentUser;
    } else {
        ui->labelAdmin->setText("Welcome Admin!");
        qDebug() << "No current user found, using default welcome message";
    }

    // Rest of your constructor code
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_pushButtonLogout_clicked()
{
    // Show the existing LoginWindow instead of creating a new one
    loginWindowInstance->show();
    this->close();
}
void AdminWindow::on_pushButtonAddUser_clicked()
{
    QString uname = ui->lineEdituser->text().trimmed();
    QString pass = ui->lineEditpassword->text();
    QString role = ui->comboBoxRole->currentText();

    // Enhanced username validation
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

    // Enhanced password validation
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

    // Check each character individually for better error reporting
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

    // Validate role selection
    if (role.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a role for the user.");
        ui->comboBoxRole->setFocus();
        return;
    }

    try {
        // Use LoginWindow's method to add user
        if (loginWindowInstance->addUser(uname, pass, role)) {
            QMessageBox::information(this, "Success",
                                     QString("User '%1' with role '%2' has been successfully added.")
                                         .arg(uname).arg(role));

            // Clear fields after successful addition
            ui->lineEdituser->clear();
            ui->lineEditpassword->clear();
            ui->comboBoxRole->setCurrentIndex(0); // Reset to first role option

            // Set focus back to username field for next entry
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

    // Get users from LoginWindow
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

    // Use LoginWindow's method to remove user
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
    // Get current user from LoginWindow instance
    if (loginWindowInstance) {
        return loginWindowInstance->getCurrentUser();
    }

    // Fallback if loginWindowInstance is null
    return "Admin User";
}

