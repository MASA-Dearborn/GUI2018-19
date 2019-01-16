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
    ui->graph->yAxis->setLabel("x1");
    //Set axes ranges and min/max available range
    ui->graph->xAxis->setRange(-1, 1);
    ui->graph->yAxis->setRange(-1, 1);
    ui->xMin->setRange(-DBL_MAX, DBL_MAX);
    ui->yMin->setRange(-DBL_MAX, DBL_MAX);
    ui->xMax->setRange(-DBL_MAX, DBL_MAX);
    ui->yMax->setRange(-DBL_MAX, DBL_MAX);
    ui->xMin->setValue(-1.0);
    ui->yMin->setValue(-1.0);
    ui->xMax->setValue(1.0);
    ui->yMax->setValue(1.0);
    //Finish setting up plots and graph them
    tGraph->setPen(QPen(Qt::blue));
    x1Graph->setPen(QPen(Qt::red));
    x2Graph->setPen(QPen(Qt::green));
    x3Graph->setPen(QPen(Qt::black));
    tGraph->setName("t");
    x1Graph->setName("x1");
    x2Graph->setName("x2");
    x3Graph->setName("x3");
    plot->legend->setIconSize(10,10);
    plot->legend->setVisible(true);
    updateGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalAxis_activated(int index)
{
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

void MainWindow::on_autorange_clicked(bool checked)
{
    //Set the graph to autosize
    if(checked) {
        resizeAxis = true;
        autoSize();
    }
}

void MainWindow::on_manualSize_clicked(bool checked)
{
    //Set graph to manualsize
    if(checked) {
        resizeAxis = false;
        manualSize();
    }
}

//Call manualsize whenever the manual domain/range is changed
void MainWindow::on_xMin_valueChanged()
{
    manualSize();
}

void MainWindow::on_yMin_valueChanged()
{
    manualSize();
}

void MainWindow::on_xMax_valueChanged()
{
    manualSize();
}

void MainWindow::on_yMax_valueChanged()
{
    manualSize();
}

void MainWindow::autoSize() {
    //If autosize button is checked change the x and y axis to include all plotted data
    if(resizeAxis) {
        plot->xAxis->rescale(true);
        plot->yAxis->rescale(true);
    }
    ui->graph->replot();
}

void MainWindow::manualSize() {
    //If the manualsize button is checked set the x and y axis to the selected min/max
    if(!resizeAxis) {
        ui->graph->xAxis->setRange(ui->xMin->value(), ui->xMax->value());
        ui->graph->yAxis->setRange(ui->yMin->value(), ui->yMax->value());
        ui->graph->replot();
    }
}

void MainWindow::updateGraph() {
    //Update each graph to the new key, and call autosize in case the domain or range changed
    x1Graph->setData(t, *key, x1, true);
    x2Graph->setData(t, *key, x2, true);
    x3Graph->setData(t, *key, x3, true);
    tGraph->setData(t, *key, t, true);
    autoSize();
}

//Set each graph as either visible or hidden based on checkbox
void MainWindow::on_t_clicked(bool checked)
{
    tGraph->setVisible(checked);
    ui->graph->replot();
}

void MainWindow::on_x1_clicked(bool checked)
{
    x1Graph->setVisible(checked);
    ui->graph->replot();
}

void MainWindow::on_x2_clicked(bool checked)
{
    x2Graph->setVisible(checked);
    ui->graph->replot();
}

void MainWindow::on_x3_clicked(bool checked)
{
    x3Graph->setVisible(checked);
    ui->graph->replot();
}
