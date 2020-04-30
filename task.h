#ifndef TASK_H
#define TASK_H

#include <qbrush.h>



class Task
{
public:

    // Constant (minimum step size)
    double const STEP_DX = 0.001;
    double const STEP_DY = 1.000;

    Task(std::string name, int offset, QLinearGradient gradient);

    // Stepper
    void step (double dx);

    // Setter: offset
    void setOffset (double offset);

    // Setter: active
    void setActive (bool active);

    // Getter: offset
    double offset () {
        return d_offset;
    }

    // Getter: gradient
    QLinearGradient gradient () {
        return d_gradient;
    }

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
    std::string d_name;

    // Offset of the task from the x-axis (vertical)
    double d_offset;

    // The gradient used to color the task
    QLinearGradient d_gradient;

    // The X and Y task vector (could be abstracted)
    QVector<double> d_xs, d_ys;

    // Boolean indicating whether or not the task is active
    bool d_active;

};

#endif // TASK_H
