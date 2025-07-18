#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "classes.h"
#include <QMainWindow>
#include <QFile>
#include <QDataStream>
#include <QIODevice>
#include <QString>
#include <QMap>
#include <QRegularExpression>
class AdminWindow;
class StaffWindow;
class ManagerWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    bool addUser(const QString& username, const QString& password, const QString& role);
    bool removeUser(const QString& username);
    QString getUserList();
    void ensureDefaultAdminExists();
    QString getCurrentUser() const;


private slots:
    void on_Push_ButtonClear_clicked();

    void on_pushButtonLogin_clicked();

private:
    Ui::LoginWindow *ui;
    void loadUsersFromFile();
    bool saveUsersToFile();
    AdminWindow* adminWindow;
    StaffWindow* staffWindow;
    ManagerWindow* managerWindow;

    const QString USER_FILE = "users.dat";
    std::map<QString, User*> usersMap;
    QString currentLoggedInUser;
};
#endif // LOGINWINDOW_H
