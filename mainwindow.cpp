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
    radioProcesser->moveToThread(radioThread);
    connect(radioThread, &QThread::finished, radioProcesser, &QObject::deleteLater);
    connect(radioProcesser, &DataProcessing::updateGraphData, this, &MainWindow::updateData);//, Qt::BlockingQueuedConnection);
    radioThread->start();
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
    //Set the x axis (key) to a dataset
    key = &t;
    //Give the x axis a label
    ui->graph->xAxis->setLabel("t");
    //Set axes ranges and min/max available range
    ui->graph->xAxis->setRange(-1, 1);
    ui->graph->yAxis->setRange(-1, 1);
    ui->xMin->setRange(-DBL_MAX, DBL_MAX);
    ui->yMin->setRange(-DBL_MAX, DBL_MAX);
    ui->xMax->setRange(-DBL_MAX, DBL_MAX);
    ui->yMax->setRange(-DBL_MAX, DBL_MAX);
    ui->parametricMin->setRange(0, DBL_MAX);
    ui->parametricMax->setRange(0, DBL_MAX);
    ui->recentTime->setRange(0, DBL_MAX);
    ui->xMin->setValue(0);
    ui->yMin->setValue(0);
    ui->xMax->setValue(10);
    ui->yMax->setValue(10);
    ui->parametricMin->setValue(0);
    ui->parametricMax->setValue(1);
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

//Update when the parametric minimum/maximum changes
void MainWindow::on_parametricMin_valueChanged() {
    parametricRange();
}

void MainWindow::on_parametricMax_valueChanged() {
    parametricRange();
}

//Update when the recent time changes
void MainWindow::on_recentTime_valueChanged() {
    recentSize();
}

//Set each graph as either visible or hidden based on checkbox
void MainWindow::on_t_clicked(bool checked) {
    tGraph->setVisible(checked);
    autoSize();
    parametricRange();
    recentSize();
    manualSize();
}

void MainWindow::on_x1_clicked(bool checked) {
    x1Graph->setVisible(checked);
    autoSize();
    parametricRange();
    recentSize();
    manualSize();
}

void MainWindow::on_x2_clicked(bool checked) {
    x2Graph->setVisible(checked);
    autoSize();
    parametricRange();
    recentSize();
    manualSize();
}

void MainWindow::on_x3_clicked(bool checked) {
    x3Graph->setVisible(checked);
    autoSize();
    parametricRange();
    recentSize();
    manualSize();
}

void MainWindow::on_manualSize_toggled(bool checked) {
    //Set graph to manualsize
    if(checked) {
        axisMode = MANUAL;
        //reset parametric indicies so graph is updated on switching
        //to parametric mode
        parametricMin = -1;
        parametricMax = -1;
        manualSize();
    }
}

void MainWindow::on_autorange_toggled(bool checked) {
    //Set the graph to autosize
    if(checked) {
        axisMode = AUTO;
        //reset parametric indicies so graph is updated on switching
        //to parametric mode
        parametricMin = -1;
        parametricMax = -1;
        autoSize();
    }
}

void MainWindow::on_recent_toggled(bool checked) {
    if(checked) {
        axisMode = RECENT;
        parametricMin = -1;
        parametricMax = -1;
        //reset parametric indicies so graph is updated on switching
        //to parametric mode
        recentSize();
    }
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
        ui->graph->replot();
    }
}

void MainWindow::manualSize() {
    //If the manualsize button is checked set the x and y axis to the selected min/max
    if(axisMode == MANUAL) {
        ui->graph->xAxis->setRange(ui->xMin->value(), ui->xMax->value());
        ui->graph->yAxis->setRange(ui->yMin->value(), ui->yMax->value());
        ui->graph->replot();
    }
}

void MainWindow::parametricRange() {
    if(axisMode == PARAMETRIC) {
        //Get target times from ui
        double minTime = ui->parametricMin->value();
        double maxTime = ui->parametricMax->value();
        //ensure times are in proper order, swap if they aren't
        if(minTime > maxTime) {
            double temp = parametricMax;
            maxTime = minTime;
            minTime = temp;
        }
        //return if given times make no sense at all
        if(maxTime < 0) {
            return;
        }
        if(minTime > t[graphEntries - 1]) {
            return;
        }
        //Check if current indicies match times to some degree of accuracy
        //determined by epsilon. If they match then the window is already
        //sized appropriately and no change is needed. If they do not, continue.
        if(parametricMin >= 0 && parametricMax >= 0) {
            if(abs(t[parametricMin] - minTime) < epsilon && abs(t[parametricMax] - maxTime) < epsilon) {
                return;
            }
        }
        //Give min and max indicies out of bounds values to guarantee any
        //valid index is the correct one
        parametricMin = -1;
        parametricMax = -1;
        //Clamp inputs in case they are out of bounds
        if(maxTime > t[graphEntries - 1]) {
            parametricMax = graphEntries - 1;
        }
        if(minTime < 0) {
            parametricMin = 0;
        }
        //If the inputs were clamped the indicies are already known, so only
        //search for the indicies if they are unknown
        if(parametricMin != 0) { parametricMin = findTimeIndex(minTime); }
        if(parametricMax != (graphEntries - 1)) { parametricMax = findTimeIndex(maxTime); }
        //If either index was not found, return
        if(parametricMin == -1 || parametricMax == -1) {
            return;
        }
        //Change domain to match the found limits, and then autosize the range appropriately
        ui->graph->xAxis->setRange((*key)[parametricMin], (*key)[parametricMax]);
        scaleValueAxisInKey((*key)[parametricMin], (*key)[parametricMax]);
    }
}

void MainWindow::recentSize() {
    if(axisMode == RECENT) {
        //If the target time is larger than the maximum time of the data, clamp
        //the input to showing all of the data instead
        if(ui->recentTime->value() > t[graphEntries - 1]) {
            axisMode = AUTO;
            autoSize();
            axisMode = RECENT;
        }
        else {
            //Find the index of the given target time. If it is not found return,
            //if it is found scale the graph appropriately
            int index = findTimeIndex(t[graphEntries - 1] - ui->recentTime->value());
            if(index == -1) { return; }
            ui->graph->xAxis->setRange((*key)[index], (*key)[graphEntries - 1]);
            scaleValueAxisInKey((*key)[index], (*key)[graphEntries - 1]);
        }
    }
}

int MainWindow::findTimeIndex(double targetTime) {
    //Calculate a new mean and standard deviation if the previous one was found
    //to be inaccurate
    if(newMean) { setNewMeanDeviation(); }
    //Check that the target time is within the dataset
    if(targetTime < t[0] || targetTime > t[graphEntries -1]) { return -1; }
    int minIndex, maxIndex;
    //Get initial guess at the minimum and maximum index of the target time
    //by using statistics
    minIndex = graphEntries - (t[graphEntries - 1] - targetTime)/(mean - stdDeviation) - 2;
    maxIndex = graphEntries - (t[graphEntries - 1] - targetTime)/(mean + stdDeviation);
    //Ensure that the indices are valid. Only one that should ever be called is
    //the first one
    while(maxIndex >= graphEntries) { maxIndex--; }
    while(minIndex >= graphEntries) { minIndex--; }
    while(minIndex < 0) { minIndex++; }
    while(maxIndex < 0) { maxIndex++; }
    //Check if the target time is within the initial guesses for the
    //minimum and maximum indices. If not, then shift the minimum and maximum
    //up or down by one standard deviation until it is found or
    //until it is determined the time does not lie in the dataset or is extremely
    //far outside the guess made by the algorithm. If it is outside the guess of
    //the algorithm then set it to recalculate the mean and standard deviation
    for(int i = 2;; i++) {
        if(targetTime <= t[maxIndex]) { break; }
        if(i > 4) { newMean = true; return -1; }
        if(maxIndex > (graphEntries - 1)) { maxIndex = graphEntries - 1; }
        minIndex = graphEntries - (t[graphEntries - 1] - targetTime)/(mean + (i - 1)*stdDeviation) - 2;
        maxIndex = graphEntries - (t[graphEntries - 1] - targetTime)/(mean + i*stdDeviation);
    }
    for(int i = 2;; i++) {
        if(targetTime >= t[minIndex]) { break; }
        if(i > 4) { newMean = true; return -1; }
        if(minIndex < 0) { minIndex = 0; }
        minIndex = graphEntries - (t[graphEntries - 1] - targetTime)/(mean - i*stdDeviation) - 2;
        maxIndex = graphEntries - (t[graphEntries - 1] - targetTime)/(mean - (i - 1)*stdDeviation);
    }
    //Perform a binary search within the minimum and maximum
    //bounds that have been found
    while((maxIndex - minIndex) > 1) {
        if(t[(minIndex + maxIndex)/2] < targetTime) {
            minIndex = (minIndex + maxIndex)/2;
        }
        else {
            maxIndex = (minIndex + maxIndex)/2;
        }
    }
    return (minIndex + maxIndex)/2;
}

void MainWindow::setNewMeanDeviation() {
    //Determine number of samples to be taken from the data
    int sampleNum = static_cast<int>(sampleSize*graphEntries + 1);
    //Create array to hold the samples
    double *sampleData = reinterpret_cast<double*>(malloc(sampleNum*sizeof(double)));
    mean = 0;
    //Find random pieces of data and save to the sample array, then calculate mean
    for(int i = 0; i < sampleNum; i++) {
        int entry = rand() % (graphEntries - 1);
        sampleData[i] = t[entry + 1] - t[entry];
        mean += sampleData[i];
    }
    mean /= sampleNum;
    stdDeviation = 0;
    //Calculate standard deviation
    for(int i = 0; i < sampleNum; i++) {
        stdDeviation += pow(sampleData[i] - mean, 2);
    }
    //Avoid a memory leak
    free(sampleData);
    stdDeviation /= (sampleNum - 1);
    stdDeviation = sqrt(stdDeviation);
    //Show that a new mean/standard deviation has been calculated so another will
    //not be calculated unless this one is found to be inaccurate
    newMean = false;
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

void MainWindow::updateData(QList<double>* data, short minutes, short hours) {
    //Check if recieved data is corrupted by comparing timestamps. If timestap is
    //close to previous timestamp accept it. If not, check if it's close to the
    //next timestamp. Ignore the first piece of data since there's nothing to
    //compare to.
    if(graphEntries != 0) {
        //Check of previous entry was tagged as possibly corrupt
        if(dubiousData == NULL) {
            //If previous data was fine, check if current data is possibly corrupt.
            //If it is, then assign pointer to data and move to next piece of data
            //to compare it to by returning
            if(abs((*data)[0] - t[graphEntries - 1]) > 1) {
                dubiousData = data;
                return;
            }
        }
        else {
            //If the previous piece of data is possibly corrupted perform the same
            //test with the newest piece of data
            if(abs((*data)[0] - (*dubiousData)[0]) > 1) {
                //Free previous piece of data and perform corruption test on piece of new data
                free(dubiousData);
                dubiousData = NULL;
                if(abs((*data)[0] - t[graphEntries - 1]) > 1) {
                    dubiousData = data;
                    return;
                }
            }
            else if((*data)[0] == (*dubiousData)[0]) {
                //If the exact same double is sent twice in a row assume
                //it is corrupted because that should never happen and is likely
                //an error and both pieces of data are corrupted
                free(dubiousData);
                free(data);
                dubiousData = NULL;
                return;
            }
            else {
                //If the previous piece of data is found to not be corrupted
                //add it to the main data vector and free it
                t[graphEntries]  = (*dubiousData)[0];
                x1[graphEntries] = (*dubiousData)[1];
                x2[graphEntries] = (*dubiousData)[2];
                x3[graphEntries] = (*dubiousData)[3];
                free(dubiousData);
                //Add the previous piece of data to the graphs
                tGraph->addData((*key)[graphEntries], t[graphEntries]);
                x1Graph->addData((*key)[graphEntries], x1[graphEntries]);
                x2Graph->addData((*key)[graphEntries], x2[graphEntries]);
                x3Graph->addData((*key)[graphEntries], x3[graphEntries]);
                ++graphEntries;
            }
        }
    }
    //Add current piece of data to main data vector because it
    //passed the corruption tests
    t[graphEntries]  = (*data)[0];
    x1[graphEntries] = (*data)[1];
    x2[graphEntries] = (*data)[2];
    x3[graphEntries] = (*data)[3];
    //Free the data after it's added to the main vector to avoid memory leaks
    free(data);
    //Add the new data to the graphs
    tGraph->addData((*key)[graphEntries], t[graphEntries]);
    x1Graph->addData((*key)[graphEntries], x1[graphEntries]);
    x2Graph->addData((*key)[graphEntries], x2[graphEntries]);
    x3Graph->addData((*key)[graphEntries], x3[graphEntries]);
    ++graphEntries;
    //Call graphing functions to update the graph
    parametricRange();
    autoSize();
    recentSize();
    manualSize();
    //Check how close the used data is to the maximum data the vector can hold
    //and increase the vector size as needed
    if(t.length()*0.75 < graphEntries) {
        updateGraphVectorSize();
    }
}

void MainWindow::updateGraph() {
    //Update each graph to the new key, and call the graph type functions to update it
    x1Graph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), x1.mid(0,graphEntries), true);
    x2Graph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), x2.mid(0,graphEntries), true);
    x3Graph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), x3.mid(0,graphEntries), true);
    tGraph->setData(t.mid(0,graphEntries), (*key).mid(0,graphEntries), t.mid(0,graphEntries), true);
    parametricRange();
    autoSize();
    recentSize();
    manualSize();
}

//Doubles the size of all vectors
void MainWindow::updateGraphVectorSize() {
    t.resize(2*t.length());
    x1.resize(2*t.length());
    x2.resize(2*t.length());
    x3.resize(2*t.length());
}
