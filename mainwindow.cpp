#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCPGraph *graph_p;
    QLinearGradient gradient{QPointF(0, 0), QPointF(0, 1)};
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    // Create the TaskGraph vector
    d_task_graphs = new QVector<TaskGraph *>{};

    // Apply general plot settings
    ui->plot->yAxis->setLabel("Tasks");
    ui->plot->xAxis->setLabel("Time (ms)");
    ui->plot->yAxis->setRange(0, 4.0);
    ui->plot->yAxis->setTicks(false);



    // Create the first gradient
    gradient.setColorAt(0.0, 0x3cc63c);
    gradient.setColorAt(1.0, 0x26f626);

    // Create and configure the first graph
    graph_p = ui->plot->addGraph();
    graph_p->setScatterStyle(QCPScatterStyle::ssDot);
    graph_p->setLineStyle(QCPGraph::LineStyle::lsLine);

    //graph_p->setBrush(gradient);
    graph_p->setName("A");

    // Add a new taskgraph instance
    d_task_graphs->append(new TaskGraph{graph_p, new Task{"A", 0, gradient}});





    // Create a second gradient
    gradient.setColorAt(0.0, 0xABCF3c);
    gradient.setColorAt(1.0, 0xCDD626);

    // Create and configure a second graph
    graph_p = ui->plot->addGraph();
    graph_p->setScatterStyle(QCPScatterStyle::ssDot);
    graph_p->setLineStyle(QCPGraph::LineStyle::lsLine);
    //graph_p->setBrush(gradient);
    graph_p->setName("B");

    // Add a new taskgraph instance
    d_task_graphs->append(new TaskGraph{graph_p, new Task{"B", 2, gradient}});
}

MainWindow::~MainWindow()
{
    delete ui;

    // Delete all tasks in the task graph
    for (TaskGraph *t : *d_task_graphs) {
        delete t;
    }

    // Delete the vector itself
    delete d_task_graphs;
}

/*\
 *  Activates the task with the given name on the graph
\*/
void MainWindow::startTask (int id) {
    d_task_graphs->at(id)->task()->setActive(true);
}

/*\
 * Deactivates the task with the given name on the graph
\*/
void MainWindow::stopTask (int id) {
    d_task_graphs->at(id)->task()->setActive(false);
}

/*\
 * Iterates the graph by updating w.r.t time
\*/
void MainWindow::iterate () {
    double absolute_max_x = 0.0;
    // Step all tasks
    for (TaskGraph *g : *d_task_graphs) {
        Task *task_p = g->task();

        task_p->step(1.0);

        g->graph()->setData(task_p->xs(), task_p->ys());

        // Update x axis
        double max_x = task_p->xs().last();

        if (max_x > absolute_max_x) {
            absolute_max_x = max_x;
        }
    }
    ui->plot->xAxis->setRange(0, absolute_max_x);
    ui->plot->replot();
    ui->plot->update();
}
