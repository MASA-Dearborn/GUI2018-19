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
    connect(this, &MainWindow::sendMessage, radioProcesser, &DataProcessing::sendMessage);
    connect(radioProcesser, &DataProcessing::addPort, this, &MainWindow::addPort);
    connect(this, &MainWindow::changePorts, radioProcesser, &DataProcessing::changePort);
    connect(this, &MainWindow::changeStopState, radioProcesser, &DataProcessing::changeStopState);
    radioThread->start();
    //emit enumeratePorts();
    time.resize(length);
    latitude.resize(length);
    longitude.resize(length);
    gpsAltitude.resize(length);
    gpsSpeed.resize(length);
    xAcceleration.resize(length);
    yAcceleration.resize(length);
    zAcceleration.resize(length);
    xOrientation.resize(length);
    yOrientation.resize(length);
    zOrientation.resize(length);
    xAngularVelocity.resize(length);
    yAngularVelocity.resize(length);
    zAngularVelocity.resize(length);
    xMagneticField.resize(length);
    yMagneticField.resize(length);
    zMagneticField.resize(length);
    temperature.resize(length);
    pressure.resize(length);
    altimeterAltitude.resize(length);
    humidity.resize(length);
    //Attach plot to ui and add graphs to it
    plot                   = ui->graph;
    timeGraph              = new QCPCurve(plot->xAxis, plot->yAxis);
    latitudeGraph          = new QCPCurve(plot->xAxis, plot->yAxis);
    longitudeGraph         = new QCPCurve(plot->xAxis, plot->yAxis);
    gpsAltitudeGraph       = new QCPCurve(plot->xAxis, plot->yAxis);
    gpsSpeedGraph          = new QCPCurve(plot->xAxis, plot->yAxis);
    xAccelerationGraph     = new QCPCurve(plot->xAxis, plot->yAxis);
    yAccelerationGraph     = new QCPCurve(plot->xAxis, plot->yAxis);
    zAccelerationGraph     = new QCPCurve(plot->xAxis, plot->yAxis);
    xOrientationGraph      = new QCPCurve(plot->xAxis, plot->yAxis);
    yOrientationGraph      = new QCPCurve(plot->xAxis, plot->yAxis);
    zOrientationGraph      = new QCPCurve(plot->xAxis, plot->yAxis);
    xAngularVelocityGraph  = new QCPCurve(plot->xAxis, plot->yAxis);
    yAngularVelocityGraph  = new QCPCurve(plot->xAxis, plot->yAxis);
    zAngularVelocityGraph  = new QCPCurve(plot->xAxis, plot->yAxis);
    xMagneticFieldGraph    = new QCPCurve(plot->xAxis, plot->yAxis);
    yMagneticFieldGraph    = new QCPCurve(plot->xAxis, plot->yAxis);
    zMagneticFieldGraph    = new QCPCurve(plot->xAxis, plot->yAxis);
    temperatureGraph       = new QCPCurve(plot->xAxis, plot->yAxis);
    pressureGraph          = new QCPCurve(plot->xAxis, plot->yAxis);
    altimeterAltitudeGraph = new QCPCurve(plot->xAxis, plot->yAxis);
    humidityGraph          = new QCPCurve(plot->xAxis, plot->yAxis);

    timeGraph->setVisible(false);
    latitudeGraph->setVisible(false);
    longitudeGraph->setVisible(false);
    gpsAltitudeGraph->setVisible(false);
    gpsSpeedGraph->setVisible(false);
    xAccelerationGraph->setVisible(false);
    yAccelerationGraph->setVisible(false);
    zAccelerationGraph->setVisible(false);
    xOrientationGraph->setVisible(false);
    yOrientationGraph->setVisible(false);
    zOrientationGraph->setVisible(false);
    xAngularVelocityGraph->setVisible(false);
    yAngularVelocityGraph->setVisible(false);
    zAngularVelocityGraph->setVisible(false);
    xMagneticFieldGraph->setVisible(false);
    yMagneticFieldGraph->setVisible(false);
    zMagneticFieldGraph->setVisible(false);
    temperatureGraph->setVisible(false);
    pressureGraph->setVisible(false);
    altimeterAltitudeGraph->setVisible(false);
    humidityGraph->setVisible(false);

    //Set the x axis (key) to a dataset
    key = &time;
    //Give the x axis a label
    ui->graph->xAxis->setLabel("Time");
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
    timeGraph->setName("Time");
    latitudeGraph->setName("Latitude");
    longitudeGraph->setName("Longitude");
    gpsAltitudeGraph->setName("GPS Altitude");
    gpsSpeedGraph->setName("GPS Speed");
    xAccelerationGraph->setName("X Acceleration");
    yAccelerationGraph->setName("Y Acceleration");
    zAccelerationGraph->setName("Z Acceleration");
    xOrientationGraph->setName("X Orientation");
    yOrientationGraph->setName("Y Orientation");
    zOrientationGraph->setName("Z Orientation");
    xAngularVelocityGraph->setName("X Angular Velocity");
    yAngularVelocityGraph->setName("Y Angular Velocity");
    zAngularVelocityGraph->setName("Z Angular Velocity");
    xMagneticFieldGraph->setName("X Magnetic Field");
    yMagneticFieldGraph->setName("Y Magnetic Field");
    zMagneticFieldGraph->setName("Z Magnetic Field");
    temperatureGraph->setName("Temperature");
    pressureGraph->setName("Pressure");
    altimeterAltitudeGraph->setName("Altimeter Altitude");
    humidityGraph->setName("Humidity");

    timeGraph->setPen(QPen(Qt::blue));
    latitudeGraph->setPen(QPen(Qt::red));
    longitudeGraph->setPen(QPen(Qt::green));
    gpsAltitudeGraph->setPen(QPen(Qt::yellow));
    gpsSpeedGraph->setPen(QPen(Qt::magenta));
    xAccelerationGraph->setPen(QPen(Qt::black));
    yAccelerationGraph->setPen(QPen(Qt::gray));
    zAccelerationGraph->setPen(QPen(Qt::cyan));
    xOrientationGraph->setPen(QPen(QColor(255, 0, 255)));
    yOrientationGraph->setPen(QPen(QColor(173,251,37)));
    zOrientationGraph->setPen(QPen(QColor(74,37,180)));
    xAngularVelocityGraph->setPen(QPen(QColor(68,222,97)));
    yAngularVelocityGraph->setPen(QPen(QColor(169,25,236)));
    zAngularVelocityGraph->setPen(QPen(QColor(156,38,92)));
    xMagneticFieldGraph->setPen(QPen(QColor(15,64,100)));
    yMagneticFieldGraph->setPen(QPen(QColor(68,147,95)));
    zMagneticFieldGraph->setPen(QPen(QColor(234,122,222)));
    temperatureGraph->setPen(QPen(QColor(054,66,222)));
    pressureGraph->setPen(QPen(QColor(94,35,88)));
    altimeterAltitudeGraph->setPen(QPen(QColor(110,230,129)));
    humidityGraph->setPen(QPen(QColor(74,129,222)));


    /*
    tGraph->setPen(QPen(Qt::blue));
    x1Graph->setPen(QPen(Qt::red));
    x2Graph->setPen(QPen(Qt::green));
    x3Graph->setPen(QPen(Qt::black));
    //plot->plotLayout()->addElement(0, 1, plot->legend);
    tGraph->setName("t");
    x1Graph->setName("x1");
    x2Graph->setName("x2");
    x3Graph->setName("x3");
    */
    //Place legend along bottom of the plot instead of in the corner
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;

    plot->plotLayout()->addElement(0, 1, subLayout);
    subLayout->setMargins(QMargins(5, 0, 5, 5));
    subLayout->addElement(0, 0, plot->legend);
    plot->plotLayout()->setColumnStretchFactor(1, 0.1);
    //Change the fill order of the legend, so it's filled left to right in columns
    plot->legend->setFillOrder(QCPLegend::foRowsFirst);
    plot->legend->setWrap(11);
    //Set legend's row stretch factor very small so it ends up with minimum height
    plot->plotLayout()->setRowStretchFactor(1, 0.001);
    plot->legend->setVisible(true);
    timeGraph->removeFromLegend();
    latitudeGraph->removeFromLegend();
    longitudeGraph->removeFromLegend();
    gpsAltitudeGraph->removeFromLegend();
    gpsSpeedGraph->removeFromLegend();
    xAccelerationGraph->removeFromLegend();
    yAccelerationGraph->removeFromLegend();
    zAccelerationGraph->removeFromLegend();
    xOrientationGraph->removeFromLegend();
    yOrientationGraph->removeFromLegend();
    zOrientationGraph->removeFromLegend();
    xAngularVelocityGraph->removeFromLegend();
    yAngularVelocityGraph->removeFromLegend();
    zAngularVelocityGraph->removeFromLegend();
    xMagneticFieldGraph->removeFromLegend();
    yMagneticFieldGraph->removeFromLegend();
    zMagneticFieldGraph->removeFromLegend();
    temperatureGraph->removeFromLegend();
    pressureGraph->removeFromLegend();
    altimeterAltitudeGraph->removeFromLegend();
    humidityGraph->removeFromLegend();
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
        key = &time;
        ui->graph->xAxis->setLabel("Time");
        break;

    case 1 :
        key = &latitude;
        ui->graph->xAxis->setLabel("Latitude");
        break;

    case 2 :
        key = &longitude;
        ui->graph->xAxis->setLabel("Longitude");
        break;

    case 3 :
        key = &gpsAltitude;
        ui->graph->xAxis->setLabel("GPS Altitude");
        break;

    case 4 :
        key = &gpsSpeed;
        ui->graph->xAxis->setLabel("GPS Speed");
        break;

    case 5 :
        key = &xAcceleration;
        ui->graph->xAxis->setLabel("X Acceleration");
        break;

    case 6 :
        key = &yAcceleration;
        ui->graph->xAxis->setLabel("Y Acceleration");
        break;

    case 7 :
        key = &zAcceleration;
        ui->graph->xAxis->setLabel("Z Acceleration");
        break;

    case 8 :
        key = &xOrientation;
        ui->graph->xAxis->setLabel("X Orientation");
        break;

    case 9 :
        key = &yOrientation;
        ui->graph->xAxis->setLabel("Y Orientation");
        break;

    case 10 :
        key = &zOrientation;
        ui->graph->xAxis->setLabel("Z Orientation");
        break;

    case 11 :
        key = &xAngularVelocity;
        ui->graph->xAxis->setLabel("X Angular Velocity");
        break;

    case 12 :
        key = &yAngularVelocity;
        ui->graph->xAxis->setLabel("Y Angular Velocity");
        break;

    case 13 :
        key = &zAngularVelocity;
        ui->graph->xAxis->setLabel("Z Angular Velocity");
        break;

    case 14 :
        key = &xMagneticField;
        ui->graph->xAxis->setLabel("X Magnetic Field");
        break;

    case 15 :
        key = &yMagneticField;
        ui->graph->xAxis->setLabel("Y Magnetic Field");
        break;

    case 16 :
        key = &zMagneticField;
        ui->graph->xAxis->setLabel("Z Magnetic Field");
        break;

    case 17 :
        key = &temperature;
        ui->graph->xAxis->setLabel("Temperature");
        break;

    case 18 :
        key = &pressure;
        ui->graph->xAxis->setLabel("Pressure");
        break;

    case 19 :
        key = &altimeterAltitude;
        ui->graph->xAxis->setLabel("Altimeter Altitude");
        break;

    case 20 :
        key = &humidity;
        ui->graph->xAxis->setLabel("Humidity");
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
/*
void MainWindow::on_t_clicked(bool checked) {
    timeGraph->setVisible(checked);
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
*/

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
        if(minTime > time[graphEntries - 1]) {
            return;
        }
        //Check if current indicies match times to some degree of accuracy
        //determined by epsilon. If they match then the window is already
        //sized appropriately and no change is needed. If they do not, continue.
        if(parametricMin >= 0 && parametricMax >= 0) {
            if(abs(time[parametricMin] - minTime) < epsilon && abs(time[parametricMax] - maxTime) < epsilon) {
                return;
            }
        }
        //Give min and max indicies out of bounds values to guarantee any
        //valid index is the correct one
        parametricMin = -1;
        parametricMax = -1;
        //Clamp inputs in case they are out of bounds
        if(maxTime > time[graphEntries - 1]) {
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
        if(ui->recentTime->value() > time[graphEntries - 1]) {
            axisMode = AUTO;
            autoSize();
            axisMode = RECENT;
        }
        else {
            //Find the index of the given target time. If it is not found return,
            //if it is found scale the graph appropriately
            int index = findTimeIndex(time[graphEntries - 1] - ui->recentTime->value());
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
    if(targetTime < time[0] || targetTime > time[graphEntries -1]) { return -1; }
    int minIndex, maxIndex;
    //Get initial guess at the minimum and maximum index of the target time
    //by using statistics
    minIndex = graphEntries - (time[graphEntries - 1] - targetTime)/(mean - stdDeviation) - 2;
    maxIndex = graphEntries - (time[graphEntries - 1] - targetTime)/(mean + stdDeviation);
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
        if(targetTime <= time[maxIndex]) { break; }
        if(i > 4) { newMean = true; return -1; }
        if(maxIndex > (graphEntries - 1)) { maxIndex = graphEntries - 1; }
        minIndex = graphEntries - (time[graphEntries - 1] - targetTime)/(mean + (i - 1)*stdDeviation) - 2;
        maxIndex = graphEntries - (time[graphEntries - 1] - targetTime)/(mean + i*stdDeviation);
    }
    for(int i = 2;; i++) {
        if(targetTime >= time[minIndex]) { break; }
        if(i > 4) { newMean = true; return -1; }
        if(minIndex < 0) { minIndex = 0; }
        minIndex = graphEntries - (time[graphEntries - 1] - targetTime)/(mean - i*stdDeviation) - 2;
        maxIndex = graphEntries - (time[graphEntries - 1] - targetTime)/(mean - (i - 1)*stdDeviation);
    }
    //Perform a binary search within the minimum and maximum
    //bounds that have been found
    while((maxIndex - minIndex) > 1) {
        if(time[(minIndex + maxIndex)/2] < targetTime) {
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
        sampleData[i] = time[entry + 1] - time[entry];
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

void MainWindow::updateData(QList<double>* data) {
    //Check if recieved data is corrupted by comparing timestamps. If timestap is
    //close to previous timestamp accept it. If not, check if it's close to the
    //next timestamp. Ignore the first piece of data since there's nothing to
    //compare to.
    double timeStamp;
    //qDebug() << "Update\n";
    if(graphEntries != 0) {
        timeStamp = (data->at(0) - initSecond) + 60*(data->at(21) - initMinute) + 3600*(data->at(22) - initHour) + 86400*(data->at(23) - initDay);
        //qDebug() << timeStamp << "\n";
        //Check of previous entry was tagged as possibly corrupt
        if(dubiousData == NULL) {
            //If previous data was fine, check if current data is possibly corrupt.
            //If it is, then assign pointer to data and move to next piece of data
            //to compare it to by returning
            if(abs(timeStamp - time[graphEntries - 1]) > 1) {
                //qDebug() << "Possibly corrupt data\n";
                dubiousData = data;
                return;
            }
        }
        else {
            //If the previous piece of data is possibly corrupted perform the same
            //test with the newest piece of data
            double dubiousTimeStamp = ((dubiousData->at(0) - initSecond) + 60*(dubiousData->at(21) - initMinute) + 3600*(dubiousData->at(22) - initHour) + 86400*(dubiousData->at(23) - initDay));
            //qDebug() << "Possibly corrupt data\n";
            if(abs(timeStamp - dubiousTimeStamp) > 1) {
                //Free previous piece of data and perform corruption test on piece of new data
                qDebug() << dubiousTimeStamp << " " << timeStamp << "\n";
                free(dubiousData);
                dubiousData = NULL;
                if(abs(timeStamp  - time[graphEntries - 1]) > 1) {
                    dubiousData = data;
                    return;
                }
            }
            else if(timeStamp == dubiousTimeStamp) {
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
                /*
                for(int i = 0; i < 21; i++) {
                    qDebug() << dubiousData->at(i) << "\n";
                }
                */
                //qDebug() << (time.length() - graphEntries);
                //qDebug() << "\n";

                time[graphEntries]              = dubiousTimeStamp;
                latitude[graphEntries]          = dubiousData->at(1);
                longitude[graphEntries]         = dubiousData->at(2);
                gpsAltitude[graphEntries]       = dubiousData->at(3);
                gpsSpeed[graphEntries]          = dubiousData->at(4);
                xAcceleration[graphEntries]     = dubiousData->at(5);
                yAcceleration[graphEntries]     = dubiousData->at(6);
                zAcceleration[graphEntries]     = dubiousData->at(7);
                xOrientation[graphEntries]      = dubiousData->at(8);
                yOrientation[graphEntries]      = dubiousData->at(9);
                zOrientation[graphEntries]      = dubiousData->at(10);
                xAngularVelocity[graphEntries]  = dubiousData->at(11);
                yAngularVelocity[graphEntries]  = dubiousData->at(12);
                zAngularVelocity[graphEntries]  = dubiousData->at(13);
                xMagneticField[graphEntries]    = dubiousData->at(14);
                yMagneticField[graphEntries]    = dubiousData->at(15);
                zMagneticField[graphEntries]    = dubiousData->at(16);
                temperature[graphEntries]       = dubiousData->at(17);
                pressure[graphEntries]          = dubiousData->at(18);
                altimeterAltitude[graphEntries] = dubiousData->at(19);
                humidity[graphEntries]          = dubiousData->at(20);
                //qDebug() << "Free data\n";
                free(dubiousData);
                dubiousData = NULL;
                //qDebug() << "Data added\n";
                //Add the previous piece of data to the graphs
                timeGraph->addData((*key)[graphEntries], time[graphEntries]);
                latitudeGraph->addData((*key)[graphEntries], latitude[graphEntries]);
                longitudeGraph->addData((*key)[graphEntries], longitude[graphEntries]);
                gpsAltitudeGraph->addData((*key)[graphEntries], gpsAltitude[graphEntries]);
                gpsSpeedGraph->addData((*key)[graphEntries], gpsSpeed[graphEntries]);
                xAccelerationGraph->addData((*key)[graphEntries], xAcceleration[graphEntries]);
                yAccelerationGraph->addData((*key)[graphEntries], yAcceleration[graphEntries]);
                zAccelerationGraph->addData((*key)[graphEntries], zAcceleration[graphEntries]);
                xOrientationGraph->addData((*key)[graphEntries], xOrientation[graphEntries]);
                yOrientationGraph->addData((*key)[graphEntries], yOrientation[graphEntries]);
                zOrientationGraph->addData((*key)[graphEntries], zOrientation[graphEntries]);
                xAngularVelocityGraph->addData((*key)[graphEntries], xAngularVelocity[graphEntries]);
                yAngularVelocityGraph->addData((*key)[graphEntries], yAngularVelocity[graphEntries]);
                zAngularVelocityGraph->addData((*key)[graphEntries], zAngularVelocity[graphEntries]);
                xMagneticFieldGraph->addData((*key)[graphEntries], xMagneticField[graphEntries]);
                yMagneticFieldGraph->addData((*key)[graphEntries], yMagneticField[graphEntries]);
                zMagneticFieldGraph->addData((*key)[graphEntries], zMagneticField[graphEntries]);
                temperatureGraph->addData((*key)[graphEntries], temperature[graphEntries]);
                pressureGraph->addData((*key)[graphEntries], pressure[graphEntries]);
                altimeterAltitudeGraph->addData((*key)[graphEntries], altimeterAltitude[graphEntries]);
                humidityGraph->addData((*key)[graphEntries], humidity[graphEntries]);

                ++graphEntries;

            }
        }
    }
    else {
        initSecond = data->at(0);
        initMinute = data->at(21);
        initHour   = data->at(22);
        initDay    = data->at(23);
        initMonth  = data->at(24);
        timeStamp  = 0;
    }
    //qDebug() << timeStamp << "\n";
    //Add current piece of data to main data vector because it
    //passed the corruption tests
    time[graphEntries]              = timeStamp;
    latitude[graphEntries]          = (*data)[1];
    longitude[graphEntries]         = (*data)[2];
    gpsAltitude[graphEntries]       = (*data)[3];
    gpsSpeed[graphEntries]          = (*data)[4];
    xAcceleration[graphEntries]     = (*data)[5];
    yAcceleration[graphEntries]     = (*data)[6];
    zAcceleration[graphEntries]     = (*data)[7];
    xOrientation[graphEntries]      = (*data)[8];
    yOrientation[graphEntries]      = (*data)[9];
    zOrientation[graphEntries]      = (*data)[10];
    xAngularVelocity[graphEntries]  = (*data)[11];
    yAngularVelocity[graphEntries]  = (*data)[12];
    zAngularVelocity[graphEntries]  = (*data)[13];
    xMagneticField[graphEntries]    = (*data)[14];
    yMagneticField[graphEntries]    = (*data)[15];
    zMagneticField[graphEntries]    = (*data)[16];
    temperature[graphEntries]       = (*data)[17];
    pressure[graphEntries]          = (*data)[18];
    altimeterAltitude[graphEntries] = (*data)[19];
    humidity[graphEntries]          = (*data)[20];

    //qDebug() << xAcceleration[graphEntries];

    //Free the data after it's added to the main vector to avoid memory leaks
    free(data);
    //Add the new data to the graphs
    timeGraph->addData((*key)[graphEntries], time[graphEntries]);
    latitudeGraph->addData((*key)[graphEntries], latitude[graphEntries]);
    longitudeGraph->addData((*key)[graphEntries], longitude[graphEntries]);
    gpsAltitudeGraph->addData((*key)[graphEntries], gpsAltitude[graphEntries]);
    gpsSpeedGraph->addData((*key)[graphEntries], gpsSpeed[graphEntries]);
    xAccelerationGraph->addData((*key)[graphEntries], xAcceleration[graphEntries]);
    yAccelerationGraph->addData((*key)[graphEntries], yAcceleration[graphEntries]);
    zAccelerationGraph->addData((*key)[graphEntries], zAcceleration[graphEntries]);
    xOrientationGraph->addData((*key)[graphEntries], xOrientation[graphEntries]);
    yOrientationGraph->addData((*key)[graphEntries], yOrientation[graphEntries]);
    zOrientationGraph->addData((*key)[graphEntries], zOrientation[graphEntries]);
    xAngularVelocityGraph->addData((*key)[graphEntries], xAngularVelocity[graphEntries]);
    yAngularVelocityGraph->addData((*key)[graphEntries], yAngularVelocity[graphEntries]);
    zAngularVelocityGraph->addData((*key)[graphEntries], zAngularVelocity[graphEntries]);
    xMagneticFieldGraph->addData((*key)[graphEntries], xMagneticField[graphEntries]);
    yMagneticFieldGraph->addData((*key)[graphEntries], yMagneticField[graphEntries]);
    zMagneticFieldGraph->addData((*key)[graphEntries], zMagneticField[graphEntries]);
    temperatureGraph->addData((*key)[graphEntries], temperature[graphEntries]);
    pressureGraph->addData((*key)[graphEntries], pressure[graphEntries]);
    altimeterAltitudeGraph->addData((*key)[graphEntries], altimeterAltitude[graphEntries]);
    humidityGraph->addData((*key)[graphEntries], humidity[graphEntries]);
    ++graphEntries;
    //Call graphing functions to update the graph
    parametricRange();
    autoSize();
    recentSize();
    manualSize();
    //Check how close the used data is to the maximum data the vector can hold
    //and increase the vector size as needed
    if(time.length()*0.75 < graphEntries) {
        updateGraphVectorSize();
    }
}

void MainWindow::updateGraph() {
    //Update each graph to the new key, and call the graph type functions to update it
    timeGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), time.mid(0,graphEntries), true);
    latitudeGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), latitude.mid(0,graphEntries), true);
    longitudeGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), longitude.mid(0,graphEntries), true);
    gpsAltitudeGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), gpsAltitude.mid(0,graphEntries), true);
    gpsSpeedGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), gpsSpeed.mid(0,graphEntries), true);
    xAccelerationGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), xAcceleration.mid(0,graphEntries), true);
    yAccelerationGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), yAcceleration.mid(0,graphEntries), true);
    zAccelerationGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), zAcceleration.mid(0,graphEntries), true);
    xOrientationGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), xOrientation.mid(0,graphEntries), true);
    yOrientationGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), yOrientation.mid(0,graphEntries), true);
    zOrientationGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), zOrientation.mid(0,graphEntries), true);
    xAngularVelocityGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), xAngularVelocity.mid(0,graphEntries), true);
    yAngularVelocityGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), yAngularVelocity.mid(0,graphEntries), true);
    zAngularVelocityGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), zAngularVelocity.mid(0,graphEntries), true);
    xMagneticFieldGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), xMagneticField.mid(0,graphEntries), true);
    yMagneticFieldGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), yMagneticField.mid(0,graphEntries), true);
    zMagneticFieldGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), zMagneticField.mid(0,graphEntries), true);
    temperatureGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), temperature.mid(0,graphEntries), true);
    pressureGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), pressure.mid(0,graphEntries), true);
    altimeterAltitudeGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), altimeterAltitude.mid(0,graphEntries), true);
    humidityGraph->setData(time.mid(0,graphEntries), (*key).mid(0,graphEntries), humidity.mid(0,graphEntries), true);

    parametricRange();
    autoSize();
    recentSize();
    manualSize();
}

//Doubles the size of all vectors
void MainWindow::updateGraphVectorSize() {
    time.resize(2*time.length());
    latitude.resize(2*latitude.length());
    longitude.resize(2*longitude.length());
    gpsAltitude.resize(2*gpsAltitude.length());
    gpsSpeed.resize(2*gpsSpeed.length());
    xAcceleration.resize(2*xAcceleration.length());
    yAcceleration.resize(2*yAcceleration.length());
    zAcceleration.resize(2*zAcceleration.length());
    xOrientation.resize(2*xOrientation.length());
    yOrientation.resize(2*yOrientation.length());
    zOrientation.resize(2*zOrientation.length());
    xAngularVelocity.resize(2*xAngularVelocity.length());
    yAngularVelocity.resize(2*yAngularVelocity.length());
    zAngularVelocity.resize(2*zAngularVelocity.length());
    xMagneticField.resize(2*xMagneticField.length());
    yMagneticField.resize(2*yMagneticField.length());
    zMagneticField.resize(2*zMagneticField.length());
    temperature.resize(2*temperature.length());
    pressure.resize(2*pressure.length());
    altimeterAltitude.resize(2*altimeterAltitude.length());
    humidity.resize(2*humidity.length());

}

void MainWindow::on_enableGraphs_itemChanged(QListWidgetItem *item)
{
    if(item->text().compare("Time") == 0) {
        timeGraph->setVisible(item->checkState());
        if(item->checkState()) {
            timeGraph->addToLegend();
        }
        else {
            timeGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Latitude") == 0) {
        latitudeGraph->setVisible(item->checkState());
        if(item->checkState()) {
            latitudeGraph->addToLegend();
        }
        else {
            latitudeGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Longitude") == 0) {
        longitudeGraph->setVisible(item->checkState());
        if(item->checkState()) {
            longitudeGraph->addToLegend();
        }
        else {
            longitudeGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("GPS Altitude") == 0) {
        gpsAltitudeGraph->setVisible(item->checkState());
        if(item->checkState()) {
            gpsAltitudeGraph->addToLegend();
        }
        else {
            gpsAltitudeGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("GPS Speed") == 0) {
        gpsSpeedGraph->setVisible(item->checkState());
        if(item->checkState()) {
            gpsSpeedGraph->addToLegend();
        }
        else {
            gpsSpeedGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("X Acceleration") == 0) {
        xAccelerationGraph->setVisible(item->checkState());
        if(item->checkState()) {
            xAccelerationGraph->addToLegend();
        }
        else {
            xAccelerationGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Y Acceleration") == 0) {
        yAccelerationGraph->setVisible(item->checkState());
        if(item->checkState()) {
            yAccelerationGraph->addToLegend();
        }
        else {
            yAccelerationGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Z Acceleration") == 0) {
        zAccelerationGraph->setVisible(item->checkState());
        if(item->checkState()) {
            zAccelerationGraph->addToLegend();
        }
        else {
            zAccelerationGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("X Orientation") == 0) {
        xOrientationGraph->setVisible(item->checkState());
        if(item->checkState()) {
            xOrientationGraph->addToLegend();
        }
        else {
            xOrientationGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Y Orientation") == 0) {
        yOrientationGraph->setVisible(item->checkState());
        if(item->checkState()) {
            yOrientationGraph->addToLegend();
        }
        else {
            yOrientationGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Z Orientation") == 0) {
        zOrientationGraph->setVisible(item->checkState());
        if(item->checkState()) {
            zOrientationGraph->addToLegend();
        }
        else {
            zOrientationGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("X Angular Velocity") == 0) {
        xAngularVelocityGraph->setVisible(item->checkState());
        if(item->checkState()) {
            xAngularVelocityGraph->addToLegend();
        }
        else {
            xAngularVelocityGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Y Angular Velocity") == 0) {
        yAngularVelocityGraph->setVisible(item->checkState());
        if(item->checkState()) {
            yAngularVelocityGraph->addToLegend();
        }
        else {
            yAngularVelocityGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Z Angular Velocity") == 0) {
        zAngularVelocityGraph->setVisible(item->checkState());
        if(item->checkState()) {
            zAngularVelocityGraph->addToLegend();
        }
        else {
            zAngularVelocityGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("X Magnetic Field") == 0) {
        xMagneticFieldGraph->setVisible(item->checkState());
        if(item->checkState()) {
            xMagneticFieldGraph->addToLegend();
        }
        else {
            xMagneticFieldGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Y Magnetic Field") == 0) {
        yMagneticFieldGraph->setVisible(item->checkState());
        if(item->checkState()) {
            yMagneticFieldGraph->addToLegend();
        }
        else {
            yMagneticFieldGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Z Magnetic Field") == 0) {
        zMagneticFieldGraph->setVisible(item->checkState());
        if(item->checkState()) {
            zMagneticFieldGraph->addToLegend();
        }
        else {
            zMagneticFieldGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Temperature") == 0) {
        temperatureGraph->setVisible(item->checkState());
        if(item->checkState()) {
            temperatureGraph->addToLegend();
        }
        else {
            temperatureGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Pressure") == 0) {
        pressureGraph->setVisible(item->checkState());
        if(item->checkState()) {
            pressureGraph->addToLegend();
        }
        else {
            pressureGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Altimeter Altitude") == 0) {
        altimeterAltitudeGraph->setVisible(item->checkState());
        if(item->checkState()) {
            altimeterAltitudeGraph->addToLegend();
        }
        else {
            altimeterAltitudeGraph->removeFromLegend();
        }
    }
    else if(item->text().compare("Humidity") == 0) {
        humidityGraph->setVisible(item->checkState());
        if(item->checkState()) {
            humidityGraph->addToLegend();
        }
        else {
            humidityGraph->removeFromLegend();
        }
    }

    autoSize();
    parametricRange();
    recentSize();
    manualSize();
}

void MainWindow::on_sleepButton_clicked()
{
    if(isAsleep) {
        QByteArray *message = new QByteArray("Wake", 4);
        emit sendMessage(message);
        ui->sleepButton->setText("Sleep");
    }
    else {
       QByteArray *message = new QByteArray("Sleep", 5);
       emit sendMessage(message);
       ui->sleepButton->setText("Wake");
    }
    isAsleep = !isAsleep;
}

void MainWindow::on_stopButton_clicked()
{
    if(isStopped) {
        QByteArray *message1 = new QByteArray("Stop", 4);
        emit sendMessage(message1);
        Sleep(200);
        QByteArray *message = new QByteArray("Start", 5);
        emit sendMessage(message);
        ui->stopButton->setText("Stop");
    }
    else {
        QByteArray *message = new QByteArray("Stop", 4);
        emit sendMessage(message);
        ui->stopButton->setText("Start");
    }
    isStopped = !isStopped;
    emit changeStopState(isStopped);
}

void MainWindow::addPort(QList<QString> *names) {
    bool *toRemove = reinterpret_cast<bool*>(malloc(sizeof(bool)*ui->comPorts->count()));
    //QString currentText = ui->comPorts->currentText();
    for(int i = 0; i < ui->comPorts->count(); i++) {
        toRemove[i] = true;
        for(int j = 0; j < names->length(); j++) {
            if(ui->comPorts->itemText(i).compare(names->at(j)) == 0) {
                toRemove[i] = false;
                names->removeAt(j);
                break;
            }
        }
    }
    for(int i = ui->comPorts->count() - 1; i >= 0; i--) {
        if(toRemove[i]) {
            ui->comPorts->removeItem(i);
        }
    }
    for(int i = 0; i < names->length(); i++) {
        ui->comPorts->addItem(names->at(i));
    }
    /*
    if(toRemove[ui->comPorts->currentIndex()]) {
        emit changeStopState(true);
        emit changePorts(ui->comPorts->currentText());
        ui->stopButton->setText("Start");
        isStopped = true;
    }
    */
    free(toRemove);
    free(names);
}
/*
void MainWindow::removePort(QString comPort) {
    for(int i = 0; i < ui->comPorts->count(); i++) {
        if(ui->comPorts->itemText(i).compare(comPort) == 0) {
            ui->comPorts->removeItem(i);
            return;
        }
    }
}
*/

void MainWindow::on_comPorts_currentIndexChanged(const QString &arg1) {
    emit changePorts(arg1);
    emit changeStopState(true);
    ui->stopButton->setText("Start");
    isStopped = true;
}
