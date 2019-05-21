#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <float.h>
#include <algorithm>
#include <QTextStream>
#include "dataprocessing.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //radio = new DataProcessing();
    radioProcesser->moveToThread(radioThread);
    connect(radioThread, &QThread::finished, radioProcesser, &QObject::deleteLater);
    connect(radioProcesser, &DataProcessing::updateGraphData, this, &MainWindow::updateData);//, Qt::BlockingQueuedConnection);
    radioThread->start();
    //radioProcesser.updateGraphData();
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
    /*
    for (int i=0; i<length; ++i)
    {
      t[i]  = (i - length/2)/100.0;
      x1[i] = t[i]*t[i];
      x2[i] = -2*t[i]*t[i] + 5*t[i] + 3;
      x3[i] = exp(t[i]);
      graphEntries++;
    }
    */
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
    radioThread->exit();
    radioThread->wait();
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

void MainWindow::on_autorange_toggled(bool checked) {
    //Set the graph to autosize
    if(checked) {
        axisMode = AUTO;
        parametricMin = -1;
        parametricMax = -1;
        autoSize();
    }
}

void MainWindow::on_manualSize_toggled(bool checked) {
    //Set graph to manualsize
    if(checked) {
        axisMode = MANUAL;
        parametricMin = -1;
        parametricMax = -1;
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

void MainWindow::on_parametric_toggled(bool checked) {
    //If parametric button is checked change the x and y axis to include all plotted data in the given t range
    if(checked) {
        axisMode = PARAMETRIC;
        parametricRange();
    }
}

void MainWindow::autoSize() {
    //If autosize button is checked change the x and y axis to include all plotted data
    if(axisMode == AUTO) {
        plot->xAxis->rescale(true);
        plot->yAxis->rescale(true);
    }
    ui->graph->replot();
}

void MainWindow::manualSize() {
    //If the manualsize button is checked set the x and y axis to the selected min/max
    if(axisMode == MANUAL) {
        ui->graph->xAxis->setRange(ui->xMin->value(), ui->xMax->value());
        ui->graph->yAxis->setRange(ui->yMin->value(), ui->yMax->value());
        ui->graph->replot();
    }
}

void MainWindow::updateGraph() {
    //Update each graph to the new key, and call autosize/parametricRange in case the domain or range changed
    x1Graph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), x1.mid(0,graphEntries), true);
    x2Graph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), x2.mid(0,graphEntries), true);
    x3Graph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), x3.mid(0,graphEntries), true);
    tGraph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), t.mid(0,graphEntries), true);
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
    if(axisMode == PARAMETRIC) {
        if(parametricMin >= 0 && parametricMax >= 0) {
            if(abs(t[parametricMin] - ui->parametricMin->value()) < epsilon && abs(t[parametricMax] - ui->parametricMax->value()) < epsilon) {
                return;
            }
        }
        //qDebug() << "Parametric sizing found a new domain\n";
        parametricMin = findTimeIndex(ui->parametricMin->value());
        parametricMax = findTimeIndex(ui->parametricMax->value());
        //If either the min/max indicies provided cannot be found initialize the indicies to the first or last possible value respectively
        if(parametricMin <= -1) {
            parametricMin = 0;
        }
        if(parametricMax <= -1) {
            parametricMax = graphEntries - 1;
        }
        //Change domain to match the found limits, and then autosize the range appropriately
        ui->graph->xAxis->setRange((*key)[parametricMin], (*key)[parametricMax]);
        scaleValueAxisInKey((*key)[parametricMin], (*key)[parametricMax]);
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

void MainWindow::updateGraphVectorSize() {
    t.resize(2*t.length());
    x1.resize(2*t.length());
    x2.resize(2*t.length());
    x3.resize(2*t.length());
}

void MainWindow::expandGraph() {
    if(t.length()*0.75 < graphEntries) {
        updateGraphVectorSize();
    }
    graphEntries++;
    t[graphEntries]  = (graphEntries - 1000/2)/100.0;
    x1[graphEntries] = t[graphEntries]*t[graphEntries];
    x2[graphEntries] = -2*t[graphEntries]*t[graphEntries] + 5*t[graphEntries] + 3;
    x3[graphEntries] = exp(t[graphEntries]);
}

void MainWindow::updateData(QList<double>* data, short minutes, short hours) {
    //qDebug() << data[0];
    //qDebug() << data[1];
    //qDebug() << data[2];
    //qDebug() << data[3];
    //qDebug() << "definitely working";

    t[graphEntries]  = (*data)[0];
    x1[graphEntries] = (*data)[1];
    x2[graphEntries] = (*data)[2];
    x3[graphEntries] = (*data)[3];

    free(data);
    //qDebug() << t[graphEntries];
    //qDebug() << x1[graphEntries];
    //qDebug() << x2[graphEntries];
    //qDebug() << x3[graphEntries];
    //qDebug() << t[graphEntries];
    //updateGraph();

    tGraph->addData((*key)[graphEntries], t[graphEntries]);
    x1Graph->addData((*key)[graphEntries], x1[graphEntries]);
    x2Graph->addData((*key)[graphEntries], x2[graphEntries]);
    x3Graph->addData((*key)[graphEntries], x3[graphEntries]);

    ++graphEntries;
    parametricRange();
    autoSize();
    recentSize();
    if(t.length()*0.75 < graphEntries) {
        updateGraphVectorSize();
    }
}

void MainWindow::on_recent_toggled(bool checked)
{
    if(checked) {
        axisMode = RECENT;
        parametricMin = -1;
        parametricMax = -1;
        recentSize();
    }
}

void MainWindow::recentSize() {
    //qDebug() << "Called\n";
    if(axisMode == RECENT) {
        if(ui->recentTime->value() > t[graphEntries - 1]) {
            axisMode = AUTO;
            autoSize();
            axisMode = RECENT;
        }
        else {
            int index = findTimeIndex(t[graphEntries - 1] - ui->recentTime->value());
            if(index == -1) { return; }
            ui->graph->xAxis->setRange((*key)[index], (*key)[graphEntries - 1]);
            scaleValueAxisInKey((*key)[index], (*key)[graphEntries - 1]);
        }
    }
}

void MainWindow::setNewMeanDeviation() {
    int sampleNum = static_cast<int>(0.05*graphEntries + 1);
    double *sampleData = reinterpret_cast<double*>(malloc(sampleNum*sizeof(double)));
    mean = 0;
    for(int i = 0; i < sampleNum; i++) {
        int entry = rand() % (graphEntries - 1);
        sampleData[i] = t[entry + 1] - t[entry];
        mean += sampleData[i];
    }
    mean /= sampleNum;
    stdDeviation = 0;
    for(int i = 0; i < sampleNum; i++) {
        stdDeviation += pow(sampleData[i] - mean, 2);
    }
    stdDeviation /= (sampleNum - 1);
    stdDeviation = sqrt(stdDeviation);
    newMean = false;
}

int MainWindow::findTimeIndex(double targetTime) {
    if(newMean) { setNewMeanDeviation(); }
    if(targetTime < t[0] || targetTime > t[graphEntries -1]) { return -1; }
    int minIndex, maxIndex;
    minIndex = graphEntries - (t[graphEntries - 1] - targetTime - stdDeviation)/mean - 2;
    maxIndex = graphEntries - (t[graphEntries - 1] - targetTime + stdDeviation)/mean;
    if(maxIndex == graphEntries) { maxIndex--; }
    int stdsAway = 0;
    //qDebug() << t[minIndex] << " " << t[maxIndex] << " " << targetTime << "\n";
    while(targetTime > t[maxIndex]) {
        minIndex += stdDeviation/mean + 1;
        maxIndex += stdDeviation/mean + 1;
        newMean = true;
        stdsAway++;
        if(maxIndex > (graphEntries - 1) || stdsAway > 2) { return -1; }
    }
    while(targetTime < t[minIndex]) {
        //qDebug() << targetTime << " " << t[minIndex] << "\n";
        minIndex -= stdDeviation/mean + 1;
        maxIndex -= stdDeviation/mean + 1;
        newMean = true;
        stdsAway++;
        if(minIndex < 0 || stdsAway > 2) { return -1; }
    }
    while(abs(t[(minIndex + maxIndex)/2] - targetTime) > epsilon) {
        if(t[(minIndex + maxIndex)/2] < targetTime) {
            minIndex = (minIndex + maxIndex)/2;
        }
        else {
            maxIndex = (minIndex + maxIndex)/2;
        }
    }
    //if(newMean) {
    //    qDebug() << "New mean found\n";
    //}
    return (minIndex + maxIndex)/2;
}
