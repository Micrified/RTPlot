#include "taskgraph.h"


TaskGraph::TaskGraph(QCPGraph *graph_p, Task *task_p):
    d_graph(graph_p),
    d_task(task_p)
{

}

TaskGraph::~TaskGraph() {
    delete d_task;

    // I asume the graph will be deleted by the UI object
}
