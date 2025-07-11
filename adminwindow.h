#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H
#include "managerwindow.h"
#include <QDialog>
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
};

#endif // ADMINWINDOW_H
