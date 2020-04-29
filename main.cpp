#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    for (int i = 0; i < 10; ++i) {
        w.iterate();
    }

    return a.exec();
}
