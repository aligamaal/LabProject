#ifndef STAFFWINDOW_H
#define STAFFWINDOW_H

#include <QDialog>

namespace Ui {
class StaffWindow;
}

class StaffWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StaffWindow(QWidget *parent = nullptr);
    ~StaffWindow();

private slots:
    void on_pushButtonlogout_clicked();

    void on_pushButtonViewInv_clicked();

private:
    Ui::StaffWindow *ui;
};

#endif // STAFFWINDOW_H
