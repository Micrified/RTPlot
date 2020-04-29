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

    // Set task as initially inactive
    d_task_active = false;

    // Create and configure the graph
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
    ui->plot->graph(0)->setLineStyle(QCPGraph::LineStyle::lsLine);
    ui->plot->graph(0)->setName("Task");

    // Initialize the X and Y task vectors
    d_task_x = QVector<double>{};
    d_task_y = QVector<double>{};

    // Iterate a bit
    for (int i = 0; i < 20; ++i) {
        iterate();
        if ((i % 2) == 0) {
            startTask("Foo");
        } else {
            stopTask("Foo");
        }
        cout << "Iterate() ...\n";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*\
 *  Activates the task with the given name on the graph
\*/
void MainWindow::startTask (char const * const task_name) {
   d_task_active = true;
}

/*\
 * Deactivates the task with the given name on the graph
\*/
void MainWindow::stopTask (char const * const task_name) {
    d_task_active = false;
}

/*\
 * Iterates the graph by updating w.r.t time
\*/
void MainWindow::iterate () {
    static double step = 0;
    d_task_x.append((step += 1));
    d_task_y.append((d_task_active ? 1.0 : 0.0));
    ui->plot->graph(0)->setData(d_task_x, d_task_y);
    ui->plot->replot();
    ui->plot->update();
}
