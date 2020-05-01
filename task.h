#ifndef TASK_H
#define TASK_H

#include <qbrush.h>
#include <QString>


class Task
{
public:

    // Constant (minimum step size)
    double const STEP_DX = 0.0001;
    double const STEP_DY = 1.000;

    Task(QString name, double x_offset, int y_offset);

    // Stepper
    void step (double dx);

    // Setter: active
    void setActive (bool active);

    // Getter: xs
    QVector<double> xs () {
        return d_xs;
    }

    // Getter: ys
    QVector<double> ys () {
        return d_ys;
    }

    // Getter: active
    bool active () {
        return d_active;
    }

private:

    // The task name
    QString d_name;

    // The x-axis offset
    double d_x_offset;

    // The y-axis offset
    double d_y_offset;

    // The X and Y task vector (could be abstracted)
    QVector<double> d_xs, d_ys;

    // Boolean indicating whether or not the task is active
    bool d_active;

};

#endif // TASK_H
