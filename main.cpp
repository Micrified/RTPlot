#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    for (int i = 0; i < 20; ++i) {
        if ((i % 3) == 0) {
            w.startTask(0);
        } else {
            w.stopTask(0);
        }

        if ((i % 4) == 0) {
            w.startTask(1);
        } else {
            w.stopTask(1);
        }
        w.iterate();
    }

    return a.exec();
}
