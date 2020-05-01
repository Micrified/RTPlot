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
     *  @brief Creates a new task in the graph to be tracked
     *  @param
     *  - name: The name of the task used in the legend
     *  - color: The color of the line
     *  - x_offset: The x-axis offset
     * @return Identifier (index) of the added task graph
    \*/
    int addTask (QString name, QColor color, double x_offset);

    /*\
     *  @brief Sets the task with the given ID to be active
     *  @param
     *  - id: The identifier of the task returned from addTask
     *  @return None
    \*/
    void startTask (int id);

    /*\
     * @brief Sets the task with the given ID to be inactive
     * @param
     * - id: The identifier of the task returned from addTask
     * @return None
    \*/
    void stopTask (int id);

    /*\
     * @brief Steps all tasks forward in time by the given x-axis
     *        unit
     * @note In order to not go back in time, the absolute value
     *       is used
     * @param
     *  - step_size: The step-size to step all tasks forward with
     * @return None
    \*/
    void step (double step_size);



private slots:
    void horzScrollBarChanged (int value);
    void vertScrollBarChanged (int value);
    void xAxisChanged (QCPRange range);
    void yAxisChanged (QCPRange range);
    void onClick (QMouseEvent *event);
    void onMove (QMouseEvent *event);
    void onRelease (QMouseEvent *event);
    void onWheel (QWheelEvent *event);
private:
    Ui::MainWindow *ui;

    // Boolean set when a dragging event is occurring
    bool d_dragging;

    // The point at which the last click happened
    QPoint d_last_click_point;

    // The x-axis range last used (reset)
    QCPRange *d_last_range_x_p;
    QCPRange *d_last_range_y_p;

    // The current rectangle to draw
    QCPItemRect *d_selection_rect;

    // The open-file action
    QAction *d_open_action;

    // Vector of tasks to show
    QVector<TaskGraph *> *d_task_graphs;


    // Create actions for the menu
    void createActions();

    // Create the menus
    void createMenus();

    // File opening handler
    void openFile();


};

#endif // MAINWINDOW_H
