#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

typedef struct {
    std::string task_name;

} task_t;

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
    void startTask (char const * const task_name);

    /*\
     * Deactivates the task with the given name on the graph
    \*/
    void stopTask (char const * const task_name);

    /*\
     * Iterates the graph by updating w.r.t time
    \*/
    void iterate ();



private slots:

private:
    Ui::MainWindow *ui;

    // Points for the graph
    QVector<double> d_task_x, d_task_y;

    // The current task name
    std::string d_task_name;

    // Whether the task is active or not
    bool d_task_active;
};

#endif // MAINWINDOW_H
