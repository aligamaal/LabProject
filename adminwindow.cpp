#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "globals.h"
#include "loginwindow.h"
#include <QMessageBox>
#include "managerwindow.h"
#include <QFile>        // For file operations
#include <QDataStream>  // For binary data handling
#include <QDir>

AdminWindow::AdminWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminWindow)
    ,m_managerWindow(nullptr)
{
    ui->setupUi(this);
    QFile userFile(USER_FILE);
    if (userFile.exists()) {
        loadUsersFromFile();
    } else {
        // Create default admin
        usersMap["admin"] = new Admin("admin", "adminpass");
        saveUsersToFile(); // Save immediately
    }
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

    saveUsersToFile();
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
    saveUsersToFile();
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

void AdminWindow::loadUsersFromFile()
{
    QFile file(USER_FILE);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        usersMap.clear(); // Clear existing users

        int userCount;
        in >> userCount;

        for (int i = 0; i < userCount; i++) {
            QString username, password, role;
            in >> username >> password >> role;

            // Reconstruct users from file data
            if (role == "Admin") {
                usersMap[username] = new Admin(username, password);
            } else if (role == "Manager") {
                usersMap[username] = new Manager(username, password);
            } else if (role == "Staff") {
                usersMap[username] = new Staff(username, password);
            }
        }
        file.close();
    }
}

void AdminWindow::saveUsersToFile()
{
    QFile file(USER_FILE);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << usersMap.size(); // Save total count first

        // Save all users from the map
        for (const auto& [username, user] : usersMap) {
            out << username
                << user->getPassword()
                << user->getRole();
        }
        file.close();
    }
}


