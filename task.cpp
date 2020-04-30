#include "task.h"

Task::Task(std::string name, double x_offset, int y_offset):
    d_name(name),
    d_x_offset(x_offset),
    d_y_offset(static_cast<double>(y_offset))
{
   // Initialize vectors
   d_xs = QVector<double>{};
   d_ys = QVector<double>{};

   // Initialize active state
   d_active = false;

   // Push initial points into vectors
   d_xs.append(d_x_offset);
   d_ys.append(d_y_offset);
}


void Task::step (double dx) {
    d_xs.append(d_xs.last() + dx);
    d_ys.append(d_ys.last());
}


void Task::setActive (bool active) {

    // Do nothing if active state is the same as the current
    if (d_active == active) {
        return;
    }

    // Append a new x offset with a very small step size
    d_xs.append(d_xs.last() + STEP_DX);

    // Go high if previous state was inactive, else go low
    if (d_active == false) {
        d_ys.append(d_y_offset + STEP_DY);
    } else {
        d_ys.append(d_y_offset);
    }

    // Update active state
    d_active = active;
}


