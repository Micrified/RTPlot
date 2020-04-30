#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Global x offset
    double x_offset = 0.0;

    // Task storage
    QVector<int> task_ids{};

    // Create a few tasks
    task_ids.append(w.addTask("A", QColor{255, 0, 0}, x_offset));
    task_ids.append(w.addTask("B", QColor{0, 255, 0}, x_offset));
    task_ids.append(w.addTask("C", QColor{0, 0, 255}, x_offset));
    task_ids.append(w.addTask("D", QColor{120, 50, 155}, x_offset));
    task_ids.append(w.addTask("E", QColor{200, 60, 35}, x_offset));

    // Start and stop the tasks arbitrarily
    for (int i = 0; i < 20; ++i) {
        int task_number = rand() % task_ids.length();

        if ((rand() % 2) == 0) {
            w.startTask(task_number);
        } else {
            w.stopTask(task_number);
        }

        w.step(1.0);
    }

    return a.exec();
}
