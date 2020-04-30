#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task.h"
#include "taskgraph.h"

#include <QMainWindow>
#include <string>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*\
     *  Activates the task with the given name on the graph
    \*/
    void startTask (int id);

    /*\
     * Deactivates the task with the given name on the graph
    \*/
    void stopTask (int id);

    /*\
     * Iterates the graph by updating w.r.t time
    \*/
    void iterate ();



private slots:

private:
    Ui::MainWindow *ui;

    // Vector of tasks to show
    QVector<TaskGraph *> *d_task_graphs;
};

#endif // MAINWINDOW_H
