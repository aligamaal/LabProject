#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H
#include "managerwindow.h"
#include "loginwindow.h"
#include <QDialog>
#include <QFile>
#include <QDataStream>
#include <QIODevice>
#include <QMainWindow>
#include <QRegularExpression>
namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(LoginWindow* loginWindow , QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_pushButtonLogout_clicked();

    void on_pushButtonAddUser_clicked();

    void on_pushButtonViewUsers_clicked();

    void on_pushButtonRemoveUser_clicked();

    void on_pushButton_clicked();



private:
    QString currentUsername;
    Ui::AdminWindow *ui;
    ManagerWindow *m_managerWindow;
    const QString USER_FILE = "users.dat";
    LoginWindow* loginWindowInstance;
    QString getCurrentLoggedInUser();
};

#endif // ADMINWINDOW_H
