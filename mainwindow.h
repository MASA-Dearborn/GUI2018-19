#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "dataprocessing.h"
#include <QThread>

namespace Ui {
class MainWindow;
}

struct DataGraph {
    QCPCurve* curve;
    QVector<double> data;
    QString name;
    int index;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void updateData(QList<double>* data);

    void addPort(QList<QString> *names);

    //void removePort(QList<QString> *names);
private slots:

    void on_horizontalAxis_activated(int index);

    void on_autorange_toggled(bool checked);

    void on_manualSize_toggled(bool checked);

    void on_xMin_valueChanged();

    void on_yMin_valueChanged();

    void on_xMax_valueChanged();

    void on_yMax_valueChanged();

    void autoSize();

    void manualSize();

    //void graphRecent();

    void updateGraph();

    void on_parametric_toggled(bool checked);

    void on_parametricMin_valueChanged();

    void on_parametricMax_valueChanged();

    void parametricRange();

    void scaleValueAxisInKey(double minKey, double maxKey, QCustomPlot *targetPlot, double underScale = 1.10, double overScale = 1.10);

    void updateGraphVectorSize();

    void on_recent_toggled(bool checked);

    void recentSize();

    void setNewMeanDeviation();

    int findTimeIndex(double targetTime);

    void on_recentTime_valueChanged();

    void on_enableGraphs_itemChanged(QListWidgetItem *item);

    void on_sleepButton_clicked();

    void on_stopButton_clicked();

    void on_comPorts_currentIndexChanged(const QString &arg1);

    void updatePermanentGraphs();

    void on_pushButton_clicked();

    void on_actionImport_2_triggered();

signals:
    void sendMessage(QByteArray *message);

    void enumeratePorts();

    void changePorts(QString name);

    void changeStopState(bool state);

    void flushRadio();
private:
    Ui::MainWindow *ui;

    /*
    QCPCurve *timeGraph, *latitudeGraph, *longitudeGraph,  *gpsAltitudeGraph, *gpsSpeedGraph, *xAccelerationGraph, *yAccelerationGraph,
    *zAccelerationGraph, *xOrientationGraph, *yOrientationGraph, *zOrientationGraph, *xAngularVelocityGraph, *yAngularVelocityGraph,
    *zAngularVelocityGraph, *xMagneticFieldGraph, *yMagneticFieldGraph, *zMagneticFieldGraph, *temperatureGraph, *pressureGraph,
    *altimeterAltitudeGraph, *humidityGraph, *dedicatedXAccelerationGraph, *dedicatedYAccelerationGraph, *dedicatedZAccelerationGraph,
    *dedicatedLongitudeGraph, *dedicatedXAngularVelocityGraph, *dedicatedYAngularVelocityGraph, *dedicatedZAngularVelocityGraph,
    *dedicatedXOrientationGraph, *dedicatedYOrientationGraph, *dedicatedZOrientationGraph;
    */

    /*
    QVector<double> time, latitude, longitude, gpsAltitude, gpsSpeed, xAcceleration, yAcceleration, zAcceleration, xOrientation,
    yOrientation, zOrientation, xAngularVelocity, yAngularVelocity, zAngularVelocity, xMagneticField, yMagneticField, zMagneticField,
    temperature, pressure, altimeterAltitude, humidity, *key;
    */

    void initializeMainPlot();
    void initializeDataGraphs(QJsonArray graphs);

    QCustomPlot *plot;
    long int length = 100;

    QVector<DataGraph> graphs;
    QVector<double>* key;

    long int graphEntries = 0;
    DataProcessing *radioProcesser = new DataProcessing();
    QThread *radioThread = new QThread();
    enum axisType {MANUAL, AUTO, PARAMETRIC, RECENT};
    axisType axisMode = MANUAL;
    long int recentIndex = -1;
    double epsilon = 0.1;
    bool newMean = true;
    double mean, stdDeviation;
    int parametricMin = -1, parametricMax = -1;
    QList<double>* dubiousData = NULL;
    double sampleSize = 0.05;
    float initSecond;
    int initMinute, initHour, initDay, initMonth;
    bool isAsleep = false, isStopped = true;
    bool reset = false;
    double velocity[3] = {0, 0, 0};
    bool calibrate = false;
    QFile *spreadSheet;
    //QVector<QPen> colors = {QPen(Qt::red), QPen(Qt::blue), QPen(Qt::black), QPen(Qt::gray), QPen(Qt::green), QPen(Qt::magenta), QPen(Qt::)};
};

#endif // MAINWINDOW_H
