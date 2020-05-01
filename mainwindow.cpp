#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
using namespace std;


/*
 *******************************************************************************
 *                               Scroll Support                                *
 *******************************************************************************
*/


void MainWindow::horzScrollBarChanged (int value)
{
    // Do not replot if the user is dragging the scrollbar
    if (qAbs(ui->plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
      ui->plot->xAxis->setRange(value/100.0, ui->plot->xAxis->range().size(), Qt::AlignCenter);
      ui->plot->replot();
    }
}


void MainWindow::vertScrollBarChanged (int value)
{
    if (qAbs(ui->plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
      ui->plot->yAxis->setRange(-value/100.0, ui->plot->yAxis->range().size(), Qt::AlignCenter);
      ui->plot->replot();
    }
}


void MainWindow::xAxisChanged (QCPRange range)
{
    ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
    ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0));
}


void MainWindow::yAxisChanged (QCPRange range)
{
    ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
    ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0));
}


/*
 *******************************************************************************
 *                                Mouse Support                                *
 *******************************************************************************
*/


void MainWindow::onClick(QMouseEvent *event) {
    QPoint p = event->pos();
    d_last_click_point = p;
    d_dragging = true;

    // Make the selection rectangle true
    d_selection_rect->setVisible(true);
}


void MainWindow::onMove(QMouseEvent *event) {
    QPoint p = event->pos();

    // Get the top of the y-range
    int origin_x, origin_y, height;
    origin_x = ui->plot->geometry().x();
    origin_y = ui->plot->geometry().y();
    height = ui->plot->geometry().height();

    // If dragging, actively update the rect
    if (d_dragging) {
        qDebug() << "Y = " << p.y() << "| (x,y) = (" << origin_x << ", " << origin_y << ")\n";
        d_selection_rect->topLeft->setPixelPoint(QPoint{d_last_click_point.x(), origin_y});
        d_selection_rect->bottomRight->setPixelPoint(QPoint{p.x(), origin_y + height});
        ui->plot->replot();
    }
}


void MainWindow::onRelease (QMouseEvent *event) {
    QPoint p = event->pos();
    if (!(p.x() == d_last_click_point.x() && p.y() == d_last_click_point.y())) {
        qDebug() << "Dragged from " << p.x() << " to " << d_last_click_point.x() << "\n";
        QCPRange new_x_range {
            ui->plot->xAxis->pixelToCoord(p.x()),
            ui->plot->xAxis->pixelToCoord(d_last_click_point.x())
        };
        QCPRange new_y_range {
            0.0,
            static_cast<double>(d_task_graphs->size() * 2)
        };

        // If no zoom is currently applied, then save the scaling
        if (d_last_range_x_p == nullptr && d_last_range_y_p == nullptr) {

            // Save the old range
            d_last_range_x_p = new QCPRange(ui->plot->xAxis->range());
            d_last_range_y_p = new QCPRange(ui->plot->yAxis->range());
        }

        // Apply the new range
        ui->plot->xAxis->setRange(new_x_range);
        ui->plot->yAxis->setRange(new_y_range);
    }

    // Hide the selection rectangle
    d_selection_rect->setVisible(false);

    // Unset dragging
    d_dragging = false;

    // Refresh
    ui->plot->replot();
}

void MainWindow::onWheel (QWheelEvent *event) {
    (void)(event);

    // If an old ranger exists, restore the value, free, and reset
    if (d_last_range_x_p != nullptr) {
        ui->plot->xAxis->setRange(*d_last_range_x_p);
        delete d_last_range_x_p;
        d_last_range_x_p = nullptr;
    }
    if (d_last_range_y_p != nullptr) {
        ui->plot->yAxis->setRange(*d_last_range_y_p);
        delete d_last_range_y_p;
        d_last_range_y_p = nullptr;
    }
}


/*
 *******************************************************************************
 *                           Menu Method Definitions                           *
 *******************************************************************************
*/


void MainWindow::createActions()
{
    d_open_action = new QAction(tr("&Open"), this);
    d_open_action->setStatusTip(tr("Create a new file"));
    connect(d_open_action, &QAction::triggered, this, &MainWindow::openFile);

    QActionGroup *alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(d_open_action);
}


void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(d_open_action);
}


void MainWindow::openFile()
{
    qDebug() << "openFile()";
}


/*
 *******************************************************************************
 *                          Class Method Definitions                           *
 *******************************************************************************
*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    d_dragging(false),
    d_last_click_point(0,0),
    d_last_range_x_p(nullptr),
    d_last_range_y_p(nullptr),
    d_selection_rect(nullptr),
    d_open_action(nullptr)
{
    ui->setupUi(this);

    // Set the name
    this->setWindowTitle("RTPlot");

    // Create the TaskGraph vector
    d_task_graphs = new QVector<TaskGraph *>{};

    // Apply general plot settings
    ui->plot->yAxis->setLabel("Tasks");
    ui->plot->xAxis->setLabel("Time (ms)");
    //ui->plot->yAxis->setTicks(false);

    // Rotate Y-axis labels
    ui->plot->yAxis->setAutoTickStep(false);
    ui->plot->yAxis->setAutoSubTicks(false);

    // Apply scroll support settings to the plot
    ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot->setInteractions(QCP::iRangeZoom);

    // Connect the axes and scroll bars
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    // Connect navigation related events
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(onClick(QMouseEvent *)));
    connect(ui->plot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(onMove(QMouseEvent *)));
    connect(ui->plot, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(onRelease(QMouseEvent *)));
    connect(ui->plot, SIGNAL(mouseWheel(QWheelEvent *)), this, SLOT(onWheel(QWheelEvent *)));

    // Apply the legend
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->plot->legend->setFont(legendFont);
    ui->plot->legend->setSelectedFont(legendFont);
    ui->plot->legend->setVisible(true);

    // Create the selection rect
    d_selection_rect = new QCPItemRect(ui->plot);
    d_selection_rect->topLeft->setPixelPoint(QPoint{0,0});
    d_selection_rect->bottomRight->setPixelPoint(QPoint{0,0});
    d_selection_rect->setVisible(false);
    d_selection_rect->setPen(QPen(QColor{255, 0, 0}));
    d_selection_rect->setBrush(QBrush(QColor{0, 0, 255, 30}));
}


MainWindow::~MainWindow()
{
    // Delete the selection rect
    delete d_selection_rect;

    // Delete the UI
    delete ui;

    // Delete all tasks in the task graph
    for (TaskGraph *t : *d_task_graphs) {
        delete t;
    }

    // Delete the vector itself
    delete d_task_graphs;
}


int MainWindow::addTask (QString name, QColor color, double x_offset) {
    QPen pen;
    QCPGraph *graph_p;

    // Obtain the new graph index
    int index = d_task_graphs->size();

    // Create the pen border color for the graph
    pen.setColor(color);
    pen.setWidth(3);

    // Create a new graph
    graph_p = ui->plot->addGraph();
    graph_p->setScatterStyle(QCPScatterStyle::ssDot);
    graph_p->setLineStyle(QCPGraph::LineStyle::lsLine);
    graph_p->setPen(pen);
    graph_p->setName(name);

    // Get the offset for the graph
    int y_offset = d_task_graphs->size() * 2;

    // Add the new taskgraph instance
    d_task_graphs->append(new TaskGraph{graph_p, new Task{name, x_offset, y_offset}});

    // Calculate the new required height for the plot
    double y_range = static_cast<double>(y_offset + 2);

    // Update the plot range with the new configuration
    ui->plot->yAxis->setRange(0, y_range);

    ui->plot->yAxis->setRange(0, y_range);
    ui->plot->yAxis->setTickStep (1.0);
    ui->plot->yAxis->setSubTickCount(0);

    return index;
}


void MainWindow::startTask (int id) {
    d_task_graphs->at(id)->task()->setActive(true);

    // add the text label at the top:
//    QCPItemText *textLabel = new QCPItemText(customPlot);
//    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
//    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
//    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
//    textLabel->setText("Text Item Demo");
//    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
//    textLabel->setPen(QPen(Qt::black)); // show black border around text

//    // add the arrow:
//    QCPItemLine *arrow = new QCPItemLine(customPlot);
//    arrow->start->setParentAnchor(textLabel->bottom);
//    arrow->end->setCoords(4, 1.6); // point to (4, 1.6) in x-y-plot coordinates
//    arrow->setHead(QCPLineEnding::esSpikeArrow);
}


void MainWindow::stopTask (int id) {
    d_task_graphs->at(id)->task()->setActive(false);
}


void MainWindow::step (double step_size) {
    double x_bound = 0.0;

    // Step all tasks
    for (TaskGraph *g : *d_task_graphs) {
        Task *task_p = g->task();

        // Step the task forward in time
        task_p->step(abs(step_size));

        // Update the graph data
        g->graph()->setData(task_p->xs(), task_p->ys());

        // Update x-axis bound
        if (task_p->xs().last() > x_bound) {
            x_bound = task_p->xs().last();
        }
    }

    // Rescale on the x-axis and then redraw
    ui->plot->xAxis->setRange(0, x_bound);
    ui->plot->replot();
    ui->plot->update();
}
