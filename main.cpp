//
//  !!! Dit bestand moet je in principe NIET wijzigen !!!
//

#include <QApplication>
#include <iostream>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
