#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QCoreApplication::setApplicationName("Font creator");
//    QCoreApplication::setOrganizationName("FontCreator");
    MainWindow w;
    w.show();
    return a.exec();
}
