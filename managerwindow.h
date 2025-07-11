#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QDialog>
#include <fstream>

namespace Ui {
class ManagerWindow;
}

class ManagerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerWindow(QWidget *parent = nullptr);
    ~ManagerWindow();

private slots:
    void on_pushButtonlogout_clicked();

    void on_pushButtonViewinv_clicked();

    void on_pushButtonAdditem_clicked();


    void on_pushButtonReport_clicked();

private:
    Ui::ManagerWindow *ui;
    int currentEditIndex = -1;

    void exportReport(const QString &reportText);
    void exportAsCsv(const std::string &filePath);
};


#endif // MANAGERWINDOW_H
