#include "dataprocessing.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QMetaType>


DataProcessing::DataProcessing()
{
    radio = new QSerialPort(this);
    qDebug() << "Number of serial ports:" << QSerialPortInfo::availablePorts().count();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        qDebug()<<"Name: "<<info.portName();
        qDebug()<<"Description: "<<info.description();
        qDebug()<<"Manufactures: "<<info.manufacturer();
        qDebug()<<info.vendorIdentifier();
        qDebug()<<info.productIdentifier();
        radio->setPort(info);
    }
    radioInfo = QSerialPortInfo(*radio);
    radio->setBaudRate(QSerialPort::Baud115200);
    qDebug() << radioInfo.portName();
    radio->open(QSerialPort::ReadOnly);
    connect(radio,&QSerialPort::readyRead,this,&DataProcessing::readData);
}
void DataProcessing::readData() {
    QByteArray radioData = radio->readAll(); //Add all available data to a temporary array
    short minutes, hours;
    bufferedData.append(radioData); //Add all collected data to global array
    if(bufferedData.length() >= 122) {
        QList<double>* data = new QList<double>; //Create list for converted data
        union { char b[4]; float d;}; //Convert bytes to floats
        for(int i = 0; i < 4; i++) {
            b[3] = bufferedData.at(3 + 4*i);
            b[2] = bufferedData.at(2 + 4*i);
            b[1] = bufferedData.at(1 + 4*i);
            b[0] = bufferedData.at(0 + 4*i);
            data->append(d);
        }
        union { char a; short n; }; //Convert bytes to shorts
        a = bufferedData.at(120);
        minutes = n;
        a = bufferedData.at(121);
        hours = n;
        bufferedData.remove(0,122); //Remove data from global array
        emit updateGraphData(data, minutes, hours); //Send data to GUI thread
    }

}
