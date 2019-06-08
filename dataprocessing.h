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

    void sendMessage(char *message, int length);
signals:
    void updateGraphData(QList<double>* data);

private:
    QSerialPortInfo radioInfo;
    QSerialPort *radio;
    QByteArray bufferedData;
    bool firstData = true;
};

#endif // DataProcessing_H
