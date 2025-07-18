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
    // Clean up dynamically allocated User objects
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
    // Clear existing users first
    for (auto& pair : usersMap) {
        delete pair.second;
    }
    usersMap.clear();

    // Try to open the file
    ifstream file("user.txt");  // Make sure this matches your save function

    if (!file.is_open()) {
        qDebug() << "Users file not found at: user.txt. Creating default admin user.";
        // Create a default admin user if no file exists
        usersMap["admin"] = new Admin("admin", "admin123");
        saveUsersToFile();  // This will save the default admin
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
            // Create default admin on error
            usersMap["admin"] = new Admin("admin", "admin123");
            saveUsersToFile();
            return;
        }
    }

    // Skip header line
    if (!getline(file, line)) {
        qDebug() << "No header line found";
        file.close();
        return;
    }

    // Read user data
    int successfullyLoaded = 0;
    for (int i = 0; i < rowsCount && getline(file, line); i++) {
        istringstream iss(line);
        string username, password, role;

        // Parse the line - note: no trailing '|' expected
        if (!getline(iss, username, '|') ||
            !getline(iss, password, '|') ||
            !getline(iss, role)) {  // Changed: removed '|' from last field
            qDebug() << "Error parsing user data at line" << i + 3 << ": " << QString::fromStdString(line);
            continue;
        }

        QString qUsername = QString::fromStdString(username);
        QString qPassword = QString::fromStdString(password);
        QString qRole = QString::fromStdString(role);

        qDebug() << "Read data - Username:" << qUsername << "Password:" << qPassword << "Role:" << qRole;

        // Create user based on role (case-insensitive comparison)
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

    // NEW: Ensure default admin exists after loading
    ensureDefaultAdminExists();

    // If default admin was added, save it to file
    if (usersMap.size() > successfullyLoaded) {
        qDebug() << "Default admin was added after loading. Saving to file.";
        saveUsersToFile();
    }

    // Debug: Print all loaded users
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

    // Write Rows Count At the Top of the file
    int rowsCount = usersMap.size();
    file << rowsCount << endl;

    // Header
    file << "Username|Password|Role" << endl;

    for (const auto& pair : usersMap) {
        User* user = pair.second;
        // Note: No trailing '|' after role
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
    // Validate username
    if (username.trimmed().isEmpty()) {
        qDebug() << "Username cannot be empty";
        return false;
    }

    // Check username length
    if (username.length() < 3) {
        qDebug() << "Username must be at least 3 characters long";
        return false;
    }

    if (username.length() > 20) {
        qDebug() << "Username cannot exceed 20 characters";
        return false;
    }

    // Validate username characters (only alphanumeric and underscore)
    QRegularExpression usernameRegex("^[a-zA-Z0-9_]+$");
    if (!usernameRegex.match(username).hasMatch()) {
        qDebug() << "Username can only contain letters, numbers, and underscores";
        return false;
    }

    // Check if user already exists (case-insensitive)
    for (const auto& pair : usersMap) {
        if (pair.first.toLower() == username.toLower()) {
            qDebug() << "Username already exists (case-insensitive check)";
            return false;
        }
    }

    // Validate password length (already checked in AdminWindow, but good to double-check)
    if (password.length() != 8) {
        qDebug() << "Password must be exactly 8 characters long";
        return false;
    }

    // Validate password characters (only alphanumeric)
    for (int i = 0; i < password.length(); i++) {
        QChar c = password.at(i);
        if (!c.isLetterOrNumber()) {
            qDebug() << "Password cannot contain special characters";
            return false;
        }
    }

    // Validate role
    QString normalizedRole = role.toLower();
    if (normalizedRole != "admin" && normalizedRole != "manager" && normalizedRole != "staff") {
        qDebug() << "Invalid role:" << role;
        return false;
    }

    try {
        // Add user to map
        if (normalizedRole == "admin") {
            usersMap[username] = new Admin(username, password);
        } else if (normalizedRole == "manager") {
            usersMap[username] = new Manager(username, password);
        } else if (normalizedRole == "staff") {
            usersMap[username] = new Staff(username, password);
        }

        // Save immediately after adding
        if (!saveUsersToFile()) {
            // If save failed, remove the user from memory
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
        return false; // User not found
    }

    if (username.toLower() == "admin") {
        qDebug() << "Cannot remove the default admin user";
        return false;
    }

    // Don't allow removing the last admin
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

    // Save immediately after removing
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
        currentLoggedInUser = username;  // Set the current user FIRST

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
                staffWindow->setLoginWindow(this);  // ADD THIS LINE!
            }
            staffWindow->show();
            this->hide();
        }
        else if (role == "manager") {
            if (!managerWindow) {
                managerWindow = new ManagerWindow(this);
                managerWindow->setLoginWindow(this);  // ADD THIS LINE!
            }
            managerWindow->show();
            this->hide();
        }

        // Clear the input fields after successful login
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
