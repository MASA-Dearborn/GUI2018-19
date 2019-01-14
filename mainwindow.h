#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_horizontalAxis_activated(int index);

    void on_autorange_clicked(bool checked);

    void on_manualSize_clicked(bool checked);

    void on_xMin_valueChanged();

    void on_yMin_valueChanged();

    void on_xMax_valueChanged();

    void on_yMax_valueChanged();

    void autoSize();

    void manualSize();

    void updateGraph();

    void on_t_clicked(bool checked);

    void on_x1_clicked(bool checked);

    void on_x2_clicked(bool checked);

    void on_x3_clicked(bool checked);

    void on_parametric_clicked(bool checked);

    void on_parametricMin_valueChanged(double arg1);

    void on_parametricMax_valueChanged(double arg1);

    void parametricRange();

    void scaleValueAxisInKey(double minKey, double maxKey, double overScale = 1.0);
private:
    Ui::MainWindow *ui;
    QCPCurve *x1Graph, *x2Graph, *x3Graph, *tGraph;
    QCustomPlot *plot;
    int length = 100001;
    QVector<double> x1, x2, x3, t, *key;
    int axisMode = 0;
};

#endif // MAINWINDOW_H
