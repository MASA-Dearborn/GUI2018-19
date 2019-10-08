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
    QDateTime setTime = QDateTime();
    //QDateTime current = QDateTime::currentDateTime();
    //uint msecs = setTime.time().msecsTo(current.time());
    QString name = QString("C:/Users/pr3de/Documents/Dab Data ") + QDateTime::currentDateTime().toString(Qt::ISODate).remove(13,6) + QString("-") + QDateTime::currentDateTime().toString(Qt::ISODate).remove(16,3).remove(0,14) + QString("-") + QDateTime::currentDateTime().toString(Qt::ISODate).remove(0,17) + QString(".csv");
    spreadSheet = new QFile(name);
    if(spreadSheet->open(QFile::WriteOnly|QFile::Truncate)) {
        qDebug() << "works";
        QTextStream output(spreadSheet);
        output << "Timestamp,Latitude,Longitude,GPS Altitude,GPS Speed,X Acceleration,Y Acceleration,Z Acceleration,X Orientation,Y Orientation,Z Orientation,X Angular Velocity,Y Angular Velocity,Z Angular Velocity,X Magnetic Field,Y Magnetic Field,Z Magnetic Field,Temperature,Pressure,Altimeter Altitude,Humidity" ;
    }
    else {
        qDebug() << "Fails";
    }
    spreadSheet->close();
    connect(radioThread, &QThread::finished, radioProcesser, &QObject::deleteLater);
    connect(radioProcesser, &DataProcessing::updateGraphData, this, &MainWindow::updateData);//, Qt::BlockingQueuedConnection);
    connect(this, &MainWindow::sendMessage, radioProcesser, &DataProcessing::sendMessage);
    connect(radioProcesser, &DataProcessing::addPort, this, &MainWindow::addPort);
    connect(this, &MainWindow::changePorts, radioProcesser, &DataProcessing::changePort);
    connect(this, &MainWindow::changeStopState, radioProcesser, &DataProcessing::changeStopState);
    connect(this, &MainWindow::flushRadio, radioProcesser, &DataProcessing::flush);
    radioThread->start();

    plot = ui->graph;

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
    //plot->plotLayout()->setRowStretchFactor(1, 0.001);
    plot->legend->setVisible(true);

    /*
    dedicatedXAccelerationGraph       = new QCPCurve(ui->Acceleration->xAxis, ui->Acceleration->yAxis);
    dedicatedYAccelerationGraph       = new QCPCurve(ui->Acceleration->xAxis, ui->Acceleration->yAxis);
    dedicatedZAccelerationGraph       = new QCPCurve(ui->Acceleration->xAxis, ui->Acceleration->yAxis);
    dedicatedLongitudeGraph           = new QCPCurve(ui->LatitudeLongitude->xAxis, ui->LatitudeLongitude->yAxis);
    dedicatedXAngularVelocityGraph    = new QCPCurve(ui->AngularVelocity->xAxis, ui->AngularVelocity->yAxis);
    dedicatedYAngularVelocityGraph    = new QCPCurve(ui->AngularVelocity->xAxis, ui->AngularVelocity->yAxis);
    dedicatedZAngularVelocityGraph    = new QCPCurve(ui->AngularVelocity->xAxis, ui->AngularVelocity->yAxis);
    dedicatedXOrientationGraph        = new QCPCurve(ui->Orientation->xAxis, ui->Orientation->yAxis);
    dedicatedYOrientationGraph        = new QCPCurve(ui->Orientation->xAxis, ui->Orientation->yAxis);
    dedicatedZOrientationGraph        = new QCPCurve(ui->Orientation->xAxis, ui->Orientation->yAxis);

    dedicatedXAccelerationGraph->setVisible(true);
    dedicatedYAccelerationGraph->setVisible(true);
    dedicatedZAccelerationGraph->setVisible(true);
    dedicatedLongitudeGraph->setVisible(true);
    dedicatedXAngularVelocityGraph->setVisible(true);
    dedicatedYAngularVelocityGraph->setVisible(true);
    dedicatedZAngularVelocityGraph->setVisible(true);
    dedicatedXOrientationGraph->setVisible(true);
    dedicatedYOrientationGraph->setVisible(true);
    dedicatedZOrientationGraph->setVisible(true);

    dedicatedXAccelerationGraph->setPen(QPen(Qt::black));
    dedicatedYAccelerationGraph->setPen(QPen(Qt::gray));
    dedicatedZAccelerationGraph->setPen(QPen(Qt::cyan));
    dedicatedLongitudeGraph->setPen(QPen(Qt::green));
    dedicatedXAngularVelocityGraph->setPen(QPen(QColor(68,222,97)));
    dedicatedYAngularVelocityGraph->setPen(QPen(QColor(169,25,236)));
    dedicatedZAngularVelocityGraph->setPen(QPen(QColor(156,38,92)));
    dedicatedXOrientationGraph->setPen(QPen(QColor(255, 0, 255)));
    dedicatedYOrientationGraph->setPen(QPen(QColor(173,251,37)));
    dedicatedZOrientationGraph->setPen(QPen(QColor(74,37,180)));

    dedicatedXAccelerationGraph->setName("X Acceleration");
    dedicatedYAccelerationGraph->setName("Y Acceleration");
    dedicatedZAccelerationGraph->setName("Z Acceleration");
    dedicatedLongitudeGraph->setName("Longitude");
    dedicatedXAngularVelocityGraph->setName("X Angular Velocity");
    dedicatedYAngularVelocityGraph->setName("Y Angular Velocity");
    dedicatedZAngularVelocityGraph->setName("Z Angular Velocity");
    dedicatedXOrientationGraph->setName("X Orientation");
    dedicatedYOrientationGraph->setName("Y Orientation");
    dedicatedZOrientationGraph->setName("Z Orientation");

    ui->Acceleration->xAxis->setLabel("Time");
    QCPLayoutGrid *accelerationLayout = new QCPLayoutGrid;
    accelerationLayout->setMargins(QMargins(5, 5, 5, 5));
    accelerationLayout->addElement(0,0,ui->Acceleration->legend);
    ui->Acceleration->legend->setFillOrder(QCPLegend::foColumnsFirst);
    ui->Acceleration->plotLayout()->addElement(1, 0, accelerationLayout);
    ui->Acceleration->plotLayout()->setRowStretchFactor(1,0.01);
    ui->Acceleration->legend->setVisible(true);

    ui->LatitudeLongitude->xAxis->setLabel("Latitude");
    ui->LatitudeLongitude->yAxis->setLabel("Longitude");

    ui->AngularVelocity->xAxis->setLabel("Time");
    QCPLayoutGrid *angularVelocityLayout = new QCPLayoutGrid;
    angularVelocityLayout->setMargins(QMargins(5, 5, 5, 5));
    angularVelocityLayout->addElement(0,0,ui->AngularVelocity->legend);
    ui->AngularVelocity->legend->setFillOrder(QCPLegend::foColumnsFirst);
    ui->AngularVelocity->plotLayout()->addElement(1, 0, angularVelocityLayout);
    ui->AngularVelocity->plotLayout()->setRowStretchFactor(1,0.01);
    ui->AngularVelocity->legend->setVisible(true);

    ui->Orientation->xAxis->setLabel("Time");
    QCPLayoutGrid *orientationLayout = new QCPLayoutGrid;
    orientationLayout->setMargins(QMargins(5, 5, 5, 5));
    orientationLayout->addElement(0,0,ui->Orientation->legend);
    ui->Orientation->legend->setFillOrder(QCPLegend::foColumnsFirst);
    ui->Orientation->plotLayout()->addElement(1, 0, orientationLayout);
    ui->Orientation->plotLayout()->setRowStretchFactor(1,0.01);
    ui->Orientation->legend->setVisible(true);
    //ui->->legend->setFillOrder(QCPLegend::foRowsFirst);
    //plot->legend->setWrap(11);
    //ui->Acceleration->ad
    */

    updateGraph();
}

MainWindow::~MainWindow() {
    radioThread->exit();
    radioThread->wait();
    delete ui;
}

void MainWindow::initializeMainPlot() {

}

void MainWindow::initializeEnableGraphs() {
    for (int i = 0; i < graphs.size(); i++) {
        QListWidgetItem *listItem = new QListWidgetItem(graphs[i].name, ui->enableGraphs);
        listItem->setCheckState(Qt::Unchecked);
        ui->enableGraphs->addItem(listItem);
    }
}

void MainWindow::initializeHorizontalAxis() {
    for (int i = 0; i < graphs.size(); i++) {
        ui->horizontalAxis->addItem(graphs[i].name);
    }
}

void MainWindow::initializeDataGraphs(QJsonArray graphParams) {
    if (graphParams.size() == 0) return;

    for (int i = 0; i < graphParams.size(); i++) {
        QJsonObject params = graphParams[i].toObject();
        DataGraph graph;

        graph.name = params.find("name")->toString();
        graph.index = params.find("index")->toInt();
        QJsonArray color = params.find("color")->toArray();

        graph.data.resize(length);

        graph.curve = new QCPCurve(plot->xAxis, plot->yAxis);
        graph.curve->setVisible(false);
        graph.curve->setName(graph.name);
        graph.curve->setPen(QPen(QColor(color.at(0).toInt(), color.at(1).toInt(), color.at(2).toInt())));
        graph.curve->removeFromLegend();

        graphs.append(graph);
    }

    key = &graphs[0].data;
}

void MainWindow::on_horizontalAxis_activated(int index) {
    key = &graphs[index].data;
    ui->graph->xAxis->setLabel(graphs[index].name);

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
        if(minTime > graphs[0].data[graphEntries - 1]) {
            return;
        }
        //Check if current indicies match times to some degree of accuracy
        //determined by epsilon. If they match then the window is already
        //sized appropriately and no change is needed. If they do not, continue.
        if(parametricMin >= 0 && parametricMax >= 0) {
            if(abs(graphs[0].data[parametricMin] - minTime) < epsilon && abs(graphs[0].data[parametricMax] - maxTime) < epsilon) {
                return;
            }
        }
        //Give min and max indicies out of bounds values to guarantee any
        //valid index is the correct one
        parametricMin = -1;
        parametricMax = -1;
        //Clamp inputs in case they are out of bounds
        if(maxTime > graphs[0].data[graphEntries - 1]) {
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
        //qDebug() << parametricMin << " " << parametricMax << "\n";
        //Change domain to match the found limits, and then autosize the range appropriately
        ui->graph->xAxis->setRange((*key)[parametricMin], (*key)[parametricMax]);
        scaleValueAxisInKey((*key)[parametricMin], (*key)[parametricMax], plot);
    }
}

void MainWindow::recentSize() {
    if(axisMode == RECENT) {
        //If the target time is larger than the maximum time of the data, clamp
        //the input to showing all of the data instead
        if(ui->recentTime->value() > graphs[0].data[graphEntries - 1]) {
            axisMode = AUTO;
            autoSize();
            axisMode = RECENT;
        }
        else {
            //Find the index of the given target time. If it is not found return,
            //if it is found scale the graph appropriately
            int index = findTimeIndex(graphs[0].data[graphEntries - 1] - ui->recentTime->value());
            if(index == -1) { return; }
            //qDebug() << index << "\n";
            ui->graph->xAxis->setRange((*key)[index], (*key)[graphEntries - 1]);
            scaleValueAxisInKey((*key)[index], (*key)[graphEntries - 1], plot);
        }
    }
}

int MainWindow::findTimeIndex(double targetTime) {
    //Calculate a new mean and standard deviation if the previous one was found
    //to be inaccurate
    if(newMean) { setNewMeanDeviation(); }
    //Check that the target time is within the dataset
    if(targetTime < graphs[0].data[0] || targetTime > graphs[0].data[graphEntries -1]) { return -1; }
    int minIndex, maxIndex;
    //Get initial guess at the minimum and maximum index of the target time
    //by using statistics
    minIndex = graphEntries - (graphs[0].data[graphEntries - 1] - targetTime)/(mean - stdDeviation) - 2;
    maxIndex = graphEntries - (graphs[0].data[graphEntries - 1] - targetTime)/(mean + stdDeviation);
    //Ensure that the indices are valid. Only one that should ever be called is
    //the first one
    if(maxIndex >= graphEntries) { maxIndex = graphEntries - 1; }
    if(minIndex >= graphEntries) { minIndex = graphEntries - 1; }
    if(minIndex < 0) { minIndex = 0; }
    if(maxIndex < 0) { maxIndex = 0; }
    //Check if the target time is within the initial guesses for the
    //minimum and maximum indices. If not, then shift the minimum and maximum
    //up or down by one standard deviation until it is found or
    //until it is determined the time does not lie in the dataset or is extremely
    //far outside the guess made by the algorithm. If it is outside the guess of
    //the algorithm then set it to recalculate the mean and standard deviation
    //qDebug() << "Find time\n";
    for(int i = 2;; i++) {
        if(minIndex < 0) { minIndex = 0; newMean = true;}
        if(maxIndex < 0) { maxIndex = 0; newMean = true;}
        if(maxIndex > (graphEntries - 1)) { maxIndex = graphEntries - 1; }
        if(minIndex > (graphEntries - 1)) { minIndex = graphEntries - 1; }
        //qDebug() << maxIndex << " " << minIndex << " " << (graphEntries - 1) << "\n";
        if(targetTime <= graphs[0].data[maxIndex]) { break; }
        if(i > 4) { newMean = true; return -1; }
        minIndex = graphEntries - (graphs[0].data[graphEntries - 1] - targetTime)/(mean + (i - 1)*stdDeviation) - 2;
        maxIndex = graphEntries - (graphs[0].data[graphEntries - 1] - targetTime)/(mean + i*stdDeviation);
    }
    //qDebug() << "Started to find time\n";
    for(int i = 2;; i++) {
        if(minIndex < 0) { minIndex = 0; }
        if(maxIndex < 0) { maxIndex = 0; }
        if(maxIndex > (graphEntries - 1)) { maxIndex = graphEntries - 1; newMean = true;}
        if(minIndex > (graphEntries - 1)) { minIndex = graphEntries - 1; newMean = true;}
        if(targetTime >= graphs[0].data[minIndex]) { break; }
        if(i > 4) { newMean = true; return -1; }
        minIndex = graphEntries - (graphs[0].data[graphEntries - 1] - targetTime)/(mean - i*stdDeviation) - 2;
        maxIndex = graphEntries - (graphs[0].data[graphEntries - 1] - targetTime)/(mean - (i - 1)*stdDeviation);
    }
    //qDebug() << "Almost found time\n";
    //Perform a binary search within the minimum and maximum
    //bounds that have been found
    while((maxIndex - minIndex) > 1) {
        if(graphs[0].data[(minIndex + maxIndex)/2] < targetTime) {
            minIndex = (minIndex + maxIndex)/2;
        }
        else {
            maxIndex = (minIndex + maxIndex)/2;
        }
    }
    //qDebug() << "Found time\n";
    return maxIndex;
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
        sampleData[i] = graphs[0].data[entry + 1] - graphs[0].data[entry];
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

void MainWindow::scaleValueAxisInKey(double minKey, double maxKey, QCustomPlot *targetPlot, double underScale, double overScale) {
    bool inRange; //Provided to the getValueRange function to report back if there was a result in the provided range; should be unnecessary to use but required as an input
    QCPRange *domain = new QCPRange(minKey, maxKey); //Create new range corresponding to the selected domain
    double minValue = DBL_MAX, maxValue = -DBL_MAX; //Initialize the upper/lower bounds to their most extreme counterparts to ensure that the range is in the dataset
    QCPRange tempRange;
    //Iterate over all plots and get their min/max values to determine the absolute min/max of the domain
    for(int i = 0; i < targetPlot->plottableCount(); ++i) {
        if(targetPlot->plottable(i)->visible()) {
            tempRange = targetPlot->plottable(i)->getValueRange(inRange, QCP::sdBoth, *domain); //Create new range corresponding to the range of the selected graph
            if(tempRange.lower < minValue) {
                minValue = tempRange.lower;
            }
            if(tempRange.upper > maxValue) {
                maxValue = tempRange.upper;
            }
        }
    }
    targetPlot->yAxis->setRange(minValue*underScale, maxValue*overScale); //Set the newly found range, adjusting for any extra space above or below
    targetPlot->replot();
}

void MainWindow::updateData(QList<double>* data) {
    //Check if recieved data is corrupted by comparing timestamps. If timestap is
    //close to previous timestamp accept it. If not, check if it's close to the
    //next timestamp. Ignore the first piece of data since there's nothing to
    //compare to.
    qDebug() << "Data length: " << data->length() << "\n";
    double timeStamp;
    QString timeStampString = ("Timestamp:\n" + QString::number(data->at(23)) + ":" + QString::number(data->at(22)) + ":" + QString::number(data->at(21)) + ":" + QString::number(data->at(0)));
    //qDebug() << "Update\n";
    if(graphEntries != 0) {

    }
    else {
        initSecond = data->at(0);
        initMinute = data->at(21);
        initHour   = data->at(22);
        initDay    = data->at(23);
        //initMonth  = data->at(24);
        timeStamp  = 0;
    }
    //qDebug() << timeStamp << "\n";
    //Add current piece of data to main data vector because it
    //passed the corruption tests
    addData:
    /*
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
    */

    // Iterate the data and graphs to add data to individual lists
    for (int i = 0; i < graphs.length(); i++) {
        graphs[i].data.append((*data)[graphs[i].index]);
        graphs[i].curve->addData((*key)[graphEntries], graphs[i].data[graphEntries]);
    }

    //Free the data after it's added to the main vector to avoid memory leaks
    free(data);
    //Add the new data to the graphs

    /*
    dedicatedXAccelerationGraph->addData(time[graphEntries], xAcceleration[graphEntries]);
    dedicatedYAccelerationGraph->addData(time[graphEntries], yAcceleration[graphEntries]);
    dedicatedZAccelerationGraph->addData(time[graphEntries], zAcceleration[graphEntries]);
    dedicatedLongitudeGraph->addData(latitude[graphEntries], longitude[graphEntries]);
    dedicatedXAngularVelocityGraph->addData(time[graphEntries], xAngularVelocity[graphEntries]);
    dedicatedYAngularVelocityGraph->addData(time[graphEntries], yAngularVelocity[graphEntries]);
    dedicatedZAngularVelocityGraph->addData(time[graphEntries], zAngularVelocity[graphEntries]);
    dedicatedXOrientationGraph->addData(time[graphEntries], xOrientation[graphEntries]);
    dedicatedYOrientationGraph->addData(time[graphEntries], yOrientation[graphEntries]);
    dedicatedZOrientationGraph->addData(time[graphEntries], zOrientation[graphEntries]);
    */

    /*
    if(graphEntries > 1) {
        velocity[0] += (time[graphEntries] - time[graphEntries - 1])*(xAcceleration[graphEntries - 1] + xAcceleration[graphEntries])/2;
        velocity[1] += (time[graphEntries] - time[graphEntries - 1])*(yAcceleration[graphEntries - 1] + yAcceleration[graphEntries])/2;
        velocity[2] += (time[graphEntries] - time[graphEntries - 1])*(zAcceleration[graphEntries - 1] + zAcceleration[graphEntries])/2;
        QString speedString = ("Speed: " + QString::number(sqrt(pow(velocity[0],2) + pow(velocity[1],2) + pow(velocity[2],2))));
        //ui->Speed->setText(speedString);
    }*/

    //QString latitudeLongitudeString = "Latitude:\n" + QString::number(latitude[graphEntries]) + "\nLongitude:\n" + QString::number(longitude[graphEntries]);
    //ui->LatLonText->setText(latitudeLongitudeString);
    //ui->Acceleration->replot();

    if(spreadSheet->open(QFile::WriteOnly|QFile::Append)) {
        QTextStream output(spreadSheet);

        for (int i = 0; i < graphs.size(); i++) {
            output << QString::number(graphs[i].data.last()) + QString(",");
        }

        spreadSheet->close();
    }

    ++graphEntries;
    if(graphEntries > 1)
        updatePermanentGraphs();
    //Call graphing functions to update the graph
    parametricRange();
    autoSize();
    recentSize();
    manualSize();
    //updatePermanentGraphs();
    //Check how close the used data is to the maximum data the vector can hold
    //and increase the vector size as needed
}

void MainWindow::updateGraph() {
    //Update each graph to the new key, and call the graph type functions to update it
    /*
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
    */

    for (int i = 0; i < graphs.length(); i++) {
        graphs[i].curve->setData(graphs[0].data.mid(0, graphEntries),
                                 (*key).mid(0, graphEntries),
                                 graphs[i].data.mid(0, graphEntries),
                                 true);
    }

    parametricRange();
    autoSize();
    recentSize();
    manualSize();
}

//Doubles the size of all vectors
void MainWindow::updateGraphVectorSize() {
    for (int i = 0; i < graphs.length(); i++) {
        graphs[i].data.resize(2 * graphs[i].data.length());
    }
}

void MainWindow::on_enableGraphs_itemChanged(QListWidgetItem *item)
{
    for (int i = 0; i < graphs.length(); i++) {
        if (item->text().compare(graphs[i].name) == 0) {
            graphs[i].curve->setVisible(item->checkState());
            if (item->checkState()) {
                graphs[i].curve->addToLegend();
            } else {
                graphs[i].curve->removeFromLegend();
            }
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
        //QByteArray *message1 = new QByteArray("Stop", 4);
        //emit sendMessage(message1);
        //Sleep(200);
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

void MainWindow::updatePermanentGraphs() {
    int index = findTimeIndex(graphs[0].data[graphEntries - 1] - 15);
    //qDebug() << "Index: " << index << " " << "Graph entries minus one: " << (graphEntries - 1) << "\n";
    if(index == -1) {
        ui->Acceleration->xAxis->rescale(true);
        ui->Acceleration->yAxis->rescale(true);
        ui->AngularVelocity->xAxis->rescale(true);
        ui->AngularVelocity->yAxis->rescale(true);
        ui->Orientation->xAxis->rescale(true);
        ui->Orientation->yAxis->rescale(true);
    }
    else {
        ui->Acceleration->xAxis->setRange(graphs[0].data[index], graphs[0].data[graphEntries - 1]);
        scaleValueAxisInKey(graphs[0].data[index], graphs[0].data[graphEntries - 1], ui->Acceleration);
        ui->AngularVelocity->xAxis->setRange(graphs[0].data[index], graphs[0].data[graphEntries - 1]);
        scaleValueAxisInKey(graphs[0].data[index], graphs[0].data[graphEntries - 1], ui->AngularVelocity);
        ui->Orientation->xAxis->setRange(graphs[0].data[index], graphs[0].data[graphEntries - 1]);
        scaleValueAxisInKey(graphs[0].data[index], graphs[0].data[graphEntries - 1], ui->Orientation);
    }
    ui->LatitudeLongitude->xAxis->rescale(true);
    ui->LatitudeLongitude->yAxis->rescale(true);
    ui->Acceleration->replot();
    ui->AngularVelocity->replot();
    ui->LatitudeLongitude->replot();
    ui->Orientation->replot();
}

void MainWindow::on_pushButton_clicked() {
    emit flushRadio();
}

void MainWindow::on_actionImport_2_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Saved Data"), "",
            tr("Spreadsheet (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
            return;
        else {

            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }

            QTextStream in(&file); //Header length is 295
            //in.setVersion(QDataStream::Qt_4_5);

            //QString output;
            //in >> output;
            QString spreadSheetData = in.readAll();
            QStringRef subString(&spreadSheetData, 0, 295);
            if(subString == QString("Timestamp,Latitude,Longitude,GPS Altitude,GPS Speed,X Acceleration,Y Acceleration,Z Acceleration,X Orientation,Y Orientation,Z Orientation,X Angular Velocity,Y Angular Velocity,Z Angular Velocity,X Magnetic Field,Y Magnetic Field,Z Magnetic Field,Temperature,Pressure,Altimeter Altitude,Humidity")) {
                spreadSheetData.remove(0,296);

                while(spreadSheetData.length() > 0) {
                    QList<double> *data = new QList<double>();
                    int timeData[3];
                    for(int i = 0;; i++) {
                        if(i >= spreadSheetData.length()) {
                            return;
                        }
                        QStringRef separators(&spreadSheetData, i, 1);
                        //if(separators == QString(",")) {
                            if(spreadSheetData.at(i) == QString(",")) {
                                qDebug() << "Seperator\n";
                                if(data->length() == 0) {
                                    //spreadSheetData.remove(0,8);
                                    QStringRef days(&spreadSheetData, 8, 2);
                                    QStringRef hours(&spreadSheetData, 11, 2);
                                    QStringRef minutes(&spreadSheetData, 14, 2);
                                    QStringRef seconds(&spreadSheetData, 17, i - 17);
                                    data->append(seconds.toDouble());
                                    timeData[0] = minutes.toInt();
                                    timeData[1] = hours.toInt();
                                    timeData[2] = days.toInt();
                                }
                                else {
                                    QStringRef newData(&spreadSheetData, 0, i);
                                    data->append(newData.toDouble());
                                }
                                spreadSheetData.remove(0, i + 1);
                                i = 0;
                            }
                            else if(spreadSheetData.at(i) == QString("\n")) {
                                qDebug() << "Seperator n\n";
                                QStringRef newData(&spreadSheetData, 0, i);
                                data->append(newData.toDouble());
                                data->append(timeData[0]);
                                data->append(timeData[1]);
                                data->append(timeData[2]);
                                spreadSheetData.remove(0, i + 1);
                                if(data->length() == 24) {
                                    updateData(data);
                                    break;
                                }
                                else {
                                    free(data);
                                    break;
                                }
                            }
                        //}
                    }
                }

            }
            qDebug() << spreadSheetData; //Header length is 295

    }
}

void MainWindow::on_actionLoad_Config_triggered() {
    QString filename = QFileDialog::getOpenFileName(this,
            tr("Open Configuration File"), "",
            tr("Json (*.json);;All Files (*)"));

    if (!filename.isEmpty()) {
        QFile file(filename);

        file.open(QIODevice::ReadOnly);
        QString contents = file.readAll();
        file.close();

        QJsonDocument json = QJsonDocument::fromJson(contents.toUtf8());

        experimentName = json["ExperimentName"].toString();

        initializeDataGraphs(json["DataGraphs"].toArray());
        initializeEnableGraphs();
        initializeHorizontalAxis();
    }
}
