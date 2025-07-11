#include "loginwindow.h"
#include "globals.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    usersMap["admin"]=new Admin("admin","adminpass");
    LoginWindow w;
    w.show();
    return a.exec();
}
