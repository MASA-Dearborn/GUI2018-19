#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "qcustomplot.h"
#include <QSerialPort>
#include <QSerialPortInfo>

class dataProcessing : public QObject
{
    Q_OBJECT

public:
    dataProcessing();

signals:
    void updateGraphData();

private:
    QSerialPortInfo radioInfo;
    QSerialPort radio;

};

#endif // DATAPROCESSING_H
