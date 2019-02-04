#ifndef DataProcessing_H
#define DataProcessing_H

#include "qcustomplot.h"
#include <QSerialPort>
#include <QSerialPortInfo>

class DataProcessing : public QObject
{
    Q_OBJECT

public:
    DataProcessing();

public slots:
    void readData();

signals:
    void updateGraphData(double data[15], short minutes, short hours);

private:
    QSerialPortInfo radioInfo;
    QSerialPort radio;
    QByteArray bufferedData;
};

#endif // DataProcessing_H
