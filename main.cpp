#include "loginwindow.h"
#include <QApplication>

// main.cpp
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;  // Remove manual usersMap initialization
    w.show();
    return a.exec();
}
