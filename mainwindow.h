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
    void updateData(QList<double>* data, short minutes, short hours);

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

    void on_t_clicked(bool checked);

    void on_x1_clicked(bool checked);

    void on_x2_clicked(bool checked);

    void on_x3_clicked(bool checked);

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

private:
    Ui::MainWindow *ui;
    QCPCurve *x1Graph, *x2Graph, *x3Graph, *tGraph;
    QCustomPlot *plot;
    long int length = 100;
    QVector<double> x1, x2, x3, t, *key;
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
};

#endif // MAINWINDOW_H
