#include "task.h"

Task::Task(std::string name, int offset, QLinearGradient gradient):
    d_name(name),
    d_offset(static_cast<double>(offset)),
    d_gradient(gradient)
{
   // Initialize vectors
   d_xs = QVector<double>{};
   d_ys = QVector<double>{};

   // Initialize active state
   d_active = false;

   // Push initial points into vectors
   d_xs.append(0x0);
   d_ys.append(d_offset);
}


void Task::step (double dx) {
    d_xs.append(d_xs.last() + dx);
    d_ys.append(d_ys.last());
}

void Task::setOffset (double offset) {
    d_offset = offset;

    // Update the offset of all x-axis points
    for (int i = 0; i < d_ys.size(); ++i) {
        d_ys[i] += offset;
    }
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
        d_ys.append(d_offset + STEP_DY);
    } else {
        d_ys.append(d_offset);
    }

    // Update active state
    d_active = active;
}


