#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H
#include "managerwindow.h"
#include <QDialog>
#include <QFile>          // Add this
#include <QDataStream>    // Add this
#include <QIODevice>
namespace Ui {
class AdminWindow;
}

class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_pushButtonLogout_clicked();

    void on_pushButtonAddUser_clicked();

    void on_pushButtonViewUsers_clicked();

    void on_pushButtonRemoveUser_clicked();

    void on_pushButton_clicked();


private:
    Ui::AdminWindow *ui;
    ManagerWindow *m_managerWindow;
    const QString USER_FILE = "users.dat";
    void loadUsersFromFile();
    void saveUsersToFile();
};

#endif // ADMINWINDOW_H
