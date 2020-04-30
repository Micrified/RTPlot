#ifndef TASKGRAPH_H
#define TASKGRAPH_H

#include "qcustomplot.h"
#include "task.h"


class TaskGraph
{
public:
    TaskGraph(QCPGraph *graph_p, Task *task_p);
    ~TaskGraph();

    // Get: graph
    QCPGraph * graph () {
        return d_graph;
    }

    // Get: task
    Task * task() {
        return d_task;
    }

private:
    QCPGraph *d_graph;
    Task *d_task;
};

#endif // TASKGRAPH_H
