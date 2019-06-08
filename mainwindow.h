#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "dataprocessing.h"
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void updateData(QList<double>* data);

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

    void scaleValueAxisInKey(double minKey, double maxKey, double underScale = 1.0, double overScale = 1.0);

    void updateGraphVectorSize();

    void on_recent_toggled(bool checked);

    void recentSize();

    void setNewMeanDeviation();

    int findTimeIndex(double targetTime);

    void on_recentTime_valueChanged();

    void on_enableGraphs_itemChanged(QListWidgetItem *item);

    void on_sleepButton_clicked();
    void on_stopButton_clicked();

signals:
    void sendMessage(char *message, int length);
private:
    Ui::MainWindow *ui;
    QCPCurve *timeGraph, *latitudeGraph, *longitudeGraph,  *gpsAltitudeGraph, *gpsSpeedGraph, *xAccelerationGraph, *yAccelerationGraph,
    *zAccelerationGraph, *xOrientationGraph, *yOrientationGraph, *zOrientationGraph, *xAngularVelocityGraph, *yAngularVelocityGraph,
    *zAngularVelocityGraph, *xMagneticFieldGraph, *yMagneticFieldGraph, *zMagneticFieldGraph, *temperatureGraph, *pressureGraph,
    *altimeterAltitudeGraph, *humidityGraph;
    QCustomPlot *plot;
    long int length = 100;
    QVector<double> time, latitude, longitude, gpsAltitude, gpsSpeed, xAcceleration, yAcceleration, zAcceleration, xOrientation,
    yOrientation, zOrientation, xAngularVelocity, yAngularVelocity, zAngularVelocity, xMagneticField, yMagneticField, zMagneticField,
    temperature, pressure, altimeterAltitude, humidity, *key;
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
    bool isAsleep = false, isStopped = false;
};

#endif // MAINWINDOW_H
