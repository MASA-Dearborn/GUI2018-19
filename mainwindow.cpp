#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <float.h>
#include <algorithm>
#include <QTextStream>
#include "dataprocessing.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    t.resize(length);
    x1.resize(length);
    x2.resize(length);
    x3.resize(length);
    //Attach plot to ui and add graphs to it
    plot    = ui->graph;
    tGraph  = new QCPCurve(plot->xAxis, plot->yAxis);
    x1Graph = new QCPCurve(plot->xAxis, plot->yAxis);
    x2Graph = new QCPCurve(plot->xAxis, plot->yAxis);
    x3Graph = new QCPCurve(plot->xAxis, plot->yAxis);
    //Generate some data
    for (int i=0; i<length; ++i)
    {
      t[i]  = (i - length/2)/100.0;
      x1[i] = t[i]*t[i];
      x2[i] = -2*t[i]*t[i] + 5*t[i] + 3;
      x3[i] = exp(t[i]);
    }
    //Set the x axis (key) to a dataset
    key = &t;
    //Give the axes some labels
    ui->graph->xAxis->setLabel("t");
    //ui->graph->yAxis->setLabel("x1");
    //Set axes ranges and min/max available range
    ui->graph->xAxis->setRange(-1, 1);
    ui->graph->yAxis->setRange(-1, 1);
    ui->xMin->setRange(-DBL_MAX, DBL_MAX);
    ui->yMin->setRange(-DBL_MAX, DBL_MAX);
    ui->xMax->setRange(-DBL_MAX, DBL_MAX);
    ui->yMax->setRange(-DBL_MAX, DBL_MAX);
    ui->parametricMin->setRange(-DBL_MAX, DBL_MAX);
    ui->parametricMax->setRange(-DBL_MAX, DBL_MAX);
    ui->xMin->setValue(-1.0);
    ui->yMin->setValue(-1.0);
    ui->xMax->setValue(1.0);
    ui->yMax->setValue(1.0);
    ui->parametricMin->setValue(-1.0);
    ui->parametricMax->setValue(1.0);
    //Finish setting up plots and graph them
    tGraph->setPen(QPen(Qt::blue));
    x1Graph->setPen(QPen(Qt::red));
    x2Graph->setPen(QPen(Qt::green));
    x3Graph->setPen(QPen(Qt::black));
    //plot->plotLayout()->addElement(0, 1, plot->legend);
    tGraph->setName("t");
    x1Graph->setName("x1");
    x2Graph->setName("x2");
    x3Graph->setName("x3");
    //Place legend along bottom of the plot instead of in the corner
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    plot->plotLayout()->addElement(1, 0, subLayout);
    subLayout->setMargins(QMargins(5, 0, 5, 5));
    subLayout->addElement(0, 0, plot->legend);
    //Change the fill order of the legend, so it's filled left to right in columns
    plot->legend->setFillOrder(QCPLegend::foColumnsFirst);
    //Set legend's row stretch factor very small so it ends up with minimum height
    plot->plotLayout()->setRowStretchFactor(1, 0.001);
    plot->legend->setVisible(true);
    updateGraph();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_horizontalAxis_activated(int index) {
    //Check which option was selected and set the x axis accordingly
    switch (index) {
    case 0 :
        key = &t;
        ui->graph->xAxis->setLabel("t");
    break;

    case 1 :
        key = &x1;
        ui->graph->xAxis->setLabel("x1");
    break;

    case 2 :
        key = &x2;
        ui->graph->xAxis->setLabel("x2");
    break;

    case 3 :
        key = &x3;
        ui->graph->xAxis->setLabel("x3");
    break;
    }
    updateGraph();
}

void MainWindow::on_autorange_clicked(bool checked) {
    //Set the graph to autosize
    if(checked) {
        axisMode = 1;
        autoSize();
    }
}

void MainWindow::on_manualSize_clicked(bool checked) {
    //Set graph to manualsize
    if(checked) {
        axisMode = 0;
        manualSize();
    }
}

//Call manualsize whenever the manual domain/range is changed
void MainWindow::on_xMin_valueChanged() {
    manualSize();
}

void MainWindow::on_yMin_valueChanged() {
    manualSize();
}

void MainWindow::on_xMax_valueChanged() {
    manualSize();
}

void MainWindow::on_yMax_valueChanged() {
    manualSize();
}

void MainWindow::on_parametric_clicked(bool checked) {
    //If parametric button is checked change the x and y axis to include all plotted data in the given t range
    if(checked) {
        axisMode = 2;
        parametricRange();
    }
}

void MainWindow::autoSize() {
    //If autosize button is checked change the x and y axis to include all plotted data
    if(axisMode == 1) {
        plot->xAxis->rescale(true);
        plot->yAxis->rescale(true);
    }
    ui->graph->replot();
}

void MainWindow::manualSize() {
    //If the manualsize button is checked set the x and y axis to the selected min/max
    if(axisMode == 0) {
        ui->graph->xAxis->setRange(ui->xMin->value(), ui->xMax->value());
        ui->graph->yAxis->setRange(ui->yMin->value(), ui->yMax->value());
        ui->graph->replot();
    }
}

void MainWindow::updateGraph() {
    //Update each graph to the new key, and call autosize/parametricRange in case the domain or range changed
    x1Graph->setData(t, *key, x1, true);
    x2Graph->setData(t, *key, x2, true);
    x3Graph->setData(t, *key, x3, true);
    tGraph->setData(t, *key, t, true);
    parametricRange();
    autoSize();
}

//Set each graph as either visible or hidden based on checkbox
void MainWindow::on_t_clicked(bool checked) {
    tGraph->setVisible(checked);
    autoSize();
    parametricRange();
}

void MainWindow::on_x1_clicked(bool checked) {
    x1Graph->setVisible(checked);
    autoSize();
    parametricRange();
}

void MainWindow::on_x2_clicked(bool checked) {
    x2Graph->setVisible(checked);
    autoSize();
    parametricRange();
}

void MainWindow::on_x3_clicked(bool checked) {
    x3Graph->setVisible(checked);
    autoSize();
    parametricRange();
}

void MainWindow::parametricRange() {
    if(axisMode == 2) {
        int minIndex = -1, maxIndex = -1; //Initialize the min/max indicies out of bounds to simplify code in for loop
        //Determine the index in which our parameter (t) is as close to the t value entered by the user
        for(int i=0; i<length; ++i) {
            if(ui->parametricMin->value() <= t[i] && minIndex < 0) {
                minIndex = i;
            }
            if(ui->parametricMax->value() <= t[i] && maxIndex < 0) {
                maxIndex = i;
            }
            //If the min and max indicies are both found break loop
            if(minIndex >= 0 && maxIndex >= 0) {
                break;
            }
        }
        //If either the min/max indicies provided cannot be found initialize the indicies to the first or last possible value respectively
        if(minIndex <= -1) {
            minIndex = 0;
        }
        if(maxIndex <= -1) {
            maxIndex = length - 1;
        }
        //Change domain to match the found limits, and then autosize the range appropriately
        ui->graph->xAxis->setRange((*key)[minIndex], (*key)[maxIndex]);
        scaleValueAxisInKey((*key)[minIndex], (*key)[maxIndex]);
    }
}

void MainWindow::scaleValueAxisInKey(double minKey, double maxKey, double underScale, double overScale) {
    bool inRange; //Provided to the getValueRange function to report back if there was a result in the provided range; should be unnecessary to use but required as an input
    QCPRange *domain = new QCPRange(minKey, maxKey); //Create new range corresponding to the selected domain
    double minValue = DBL_MAX, maxValue = -DBL_MAX; //Initialize the upper/lower bounds to their most extreme counterparts to ensure that the range is in the dataset
    QCPRange tempRange;
    //Iterate over all plots and get their min/max values to determine the absolute min/max of the domain
    for(int i = 0; i < plot->plottableCount(); ++i) {
        if(plot->plottable(i)->visible()) {
            tempRange = plot->plottable(i)->getValueRange(inRange, QCP::sdBoth, *domain); //Create new range corresponding to the range of the selected graph
            if(tempRange.lower < minValue) {
                minValue = tempRange.lower;
            }
            if(tempRange.upper > maxValue) {
                maxValue = tempRange.upper;
            }
        }
    }
    ui->graph->yAxis->setRange(minValue*underScale, maxValue*overScale); //Set the newly found range, adjusting for any extra space above or below
    ui->graph->replot();
}

void MainWindow::on_parametricMin_valueChanged() {
    parametricRange();
}

void MainWindow::on_parametricMax_valueChanged() {
    parametricRange();
}
