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

    void sendMessage(QByteArray *message);

    void changePort(QString name);

    void changeStopState(bool state);
private slots:
    void enumeratePorts();

    void radioUnplugged(QSerialPort::SerialPortError error);
signals:
    void updateGraphData(QList<double>* data);

    void addPort(QList<QString> *names);
private:
    QSerialPortInfo radioInfo;
    QSerialPort *radio;
    QByteArray bufferedData;
    QTimer *timer;
    bool isStopped = true;
};

#endif // DataProcessing_H
