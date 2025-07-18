#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "adminwindow.h"
#include "staffwindow.h"
#include "managerwindow.h"
#include "globals.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QDir>
#include <fstream>
#include <sstream>

using namespace std;

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
    , adminWindow(nullptr)
    , staffWindow(nullptr)
    , managerWindow(nullptr)
{
    ui->setupUi(this);
    loadUsersFromFile();
}

LoginWindow::~LoginWindow()
{
    for (auto& pair : usersMap) {
        delete pair.second;
    }
    usersMap.clear();

    // Clean up windows
    if (adminWindow) delete adminWindow;
    if (staffWindow) delete staffWindow;
    if (managerWindow) delete managerWindow;

    delete ui;
}

void LoginWindow::loadUsersFromFile()
{
    for (auto& pair : usersMap) {
        delete pair.second;
    }
    usersMap.clear();

    ifstream file("user.txt");

    if (!file.is_open()) {
        qDebug() << "Users file not found at: user.txt. Creating default admin user.";
        usersMap["admin"] = new Admin("admin", "admin123");
        saveUsersToFile();
        qDebug() << "Default admin user created - Username: admin, Password: admin123";
        return;
    }

    string line;
    int rowsCount = 0;

    // Read rows count
    if (getline(file, line)) {
        try {
            rowsCount = stoi(line);
            qDebug() << "File indicates" << rowsCount << "users";
        } catch (...) {
            qDebug() << "Error reading rows count, creating default admin";
            file.close();
            usersMap["admin"] = new Admin("admin", "admin123");
            saveUsersToFile();
            return;
        }
    }

    if (!getline(file, line)) {
        qDebug() << "No header line found";
        file.close();
        return;
    }

    int successfullyLoaded = 0;
    for (int i = 0; i < rowsCount && getline(file, line); i++) {
        istringstream iss(line);
        string username, password, role;

        if (!getline(iss, username, '|') ||
            !getline(iss, password, '|') ||
            !getline(iss, role)) {
            qDebug() << "Error parsing user data at line" << i + 3 << ": " << QString::fromStdString(line);
            continue;
        }

        QString qUsername = QString::fromStdString(username);
        QString qPassword = QString::fromStdString(password);
        QString qRole = QString::fromStdString(role);

        qDebug() << "Read data - Username:" << qUsername << "Password:" << qPassword << "Role:" << qRole;

        if (qRole.toLower() == "admin") {
            usersMap[qUsername] = new Admin(qUsername, qPassword);
            successfullyLoaded++;
        } else if (qRole.toLower() == "manager") {
            usersMap[qUsername] = new Manager(qUsername, qPassword);
            successfullyLoaded++;
        } else if (qRole.toLower() == "staff") {
            usersMap[qUsername] = new Staff(qUsername, qPassword);
            successfullyLoaded++;
        } else {
            qDebug() << "Unknown role:" << qRole;
        }
    }

    file.close();
    qDebug() << "Successfully loaded" << successfullyLoaded << "users out of" << rowsCount;

    ensureDefaultAdminExists();

    if (usersMap.size() > successfullyLoaded) {
        qDebug() << "Default admin was added after loading. Saving to file.";
        saveUsersToFile();
    }

    qDebug() << "Current users in system:";
    for (const auto& pair : usersMap) {
        qDebug() << "  Username:" << pair.first << "Role:" << pair.second->getRole();
    }
}


void LoginWindow::ensureDefaultAdminExists()
{
    if (usersMap.find("admin") == usersMap.end()) {
        qDebug() << "Default admin user not found. Adding default admin user.";
        usersMap["admin"] = new Admin("admin", "admin123");
        qDebug() << "Default admin user added - Username: admin, Password: admin123";
    } else {
        qDebug() << "Default admin user already exists";
    }
}
bool LoginWindow::saveUsersToFile()
{
     ensureDefaultAdminExists();
    ofstream file("user.txt");

    if (!file.is_open()){
        qDebug() << "Failed to open file for saving:" << "user.txt";
        return false;
    }

    int rowsCount = usersMap.size();
    file << rowsCount << endl;

    // Header
    file << "Username|Password|Role" << endl;

    for (const auto& pair : usersMap) {
        User* user = pair.second;
        file << user->getUsername().toStdString() << "|"
             << user->getPassword().toStdString() << "|"
             << user->getRole().toStdString() << endl;
    }

    file.close();
    qDebug() << "Users saved to file:" << "user.txt";
    qDebug() << "Saved" << rowsCount << "users";
    return true;
}

void LoginWindow::on_Push_ButtonClear_clicked()
{
    ui->lineEditUser->setText("");
    ui->lineEditPassword->setText("");
}

bool LoginWindow::addUser(const QString& username, const QString& password, const QString& role) {
    if (username.trimmed().isEmpty()) {
        qDebug() << "Username cannot be empty";
        return false;
    }

    if (username.length() < 3) {
        qDebug() << "Username must be at least 3 characters long";
        return false;
    }

    if (username.length() > 20) {
        qDebug() << "Username cannot exceed 20 characters";
        return false;
    }

    QRegularExpression usernameRegex("^[a-zA-Z0-9_]+$");
    if (!usernameRegex.match(username).hasMatch()) {
        qDebug() << "Username can only contain letters, numbers, and underscores";
        return false;
    }

    for (const auto& pair : usersMap) {
        if (pair.first.toLower() == username.toLower()) {
            qDebug() << "Username already exists (case-insensitive check)";
            return false;
        }
    }

    if (password.length() != 8) {
        qDebug() << "Password must be exactly 8 characters long";
        return false;
    }

    for (int i = 0; i < password.length(); i++) {
        QChar c = password.at(i);
        if (!c.isLetterOrNumber()) {
            qDebug() << "Password cannot contain special characters";
            return false;
        }
    }

    QString normalizedRole = role.toLower();
    if (normalizedRole != "admin" && normalizedRole != "manager" && normalizedRole != "staff") {
        qDebug() << "Invalid role:" << role;
        return false;
    }

    try {
        if (normalizedRole == "admin") {
            usersMap[username] = new Admin(username, password);
        } else if (normalizedRole == "manager") {
            usersMap[username] = new Manager(username, password);
        } else if (normalizedRole == "staff") {
            usersMap[username] = new Staff(username, password);
        }

        if (!saveUsersToFile()) {
            delete usersMap[username];
            usersMap.erase(username);
            qDebug() << "Failed to save user to file, user not added";
            return false;
        }

        qDebug() << "User added and saved successfully:" << username;
        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "Exception while adding user:" << e.what();
        return false;
    }
    catch (...) {
        qDebug() << "Unknown exception while adding user";
        return false;
    }
}

bool LoginWindow::removeUser(const QString& username) {
    auto it = usersMap.find(username);
    if (it == usersMap.end()) {
        return false;
    }

    if (username.toLower() == "admin") {
        qDebug() << "Cannot remove the default admin user";
        return false;
    }

    if (it->second->getRole().toLower() == "admin") {
        int adminCount = 0;
        for (const auto& pair : usersMap) {
            if (pair.second->getRole().toLower() == "admin") {
                adminCount++;
            }
        }
        if (adminCount <= 1) {
            qDebug() << "Cannot remove the last admin user";
            return false;
        }
    }

    delete it->second;
    usersMap.erase(it);

    saveUsersToFile();
    qDebug() << "User removed and saved:" << username;
    return true;
}

QString LoginWindow::getUserList() {
    if (usersMap.empty()) {
        return "No users found.";
    }

    QString display;
    for (const auto& pair : usersMap) {
        User* user = pair.second;
        display += "Username: " + user->getUsername()
                   + " | Role: " + user->getRole() + "\n";
    }
    return display;
}

void LoginWindow::on_pushButtonLogin_clicked()
{
    QString username = ui->lineEditUser->text().trimmed();
    QString password = ui->lineEditPassword->text();

    qDebug() << "Attempting login with username:" << username;
    qDebug() << "Available users:" << usersMap.size();

    auto it = usersMap.find(username);
    if (it != usersMap.end() && it->second->getPassword() == password) {
        QString role = it->second->getRole().toLower();
        qDebug() << "Login successful! User role:" << role;
        currentLoggedInUser = username;

        if (role == "admin") {
            if (!adminWindow) {
                adminWindow = new AdminWindow(this);
            }
            adminWindow->show();
            this->hide();
        }
        else if (role == "staff") {
            if (!staffWindow) {
                staffWindow = new StaffWindow(this);
                staffWindow->setLoginWindow(this);
            }
            staffWindow->show();
            this->hide();
        }
        else if (role == "manager") {
            if (!managerWindow) {
                managerWindow = new ManagerWindow(this);
                managerWindow->setLoginWindow(this);
            }
            managerWindow->show();
            this->hide();
        }

        ui->lineEditUser->clear();
        ui->lineEditPassword->clear();
    } else {
        qDebug() << "Login failed for username:" << username;
        QMessageBox::warning(this, "Login Failed", "Invalid username or password");
    }
}

QString LoginWindow::getCurrentUser() const
{
    return currentLoggedInUser;
}

