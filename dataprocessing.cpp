#include "dataprocessing.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>


DataProcessing::DataProcessing()
{
    //qDebug() << "Number of serial ports:" << QSerialPortInfo::availablePorts().count();
    /*
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        qDebug()<<"Name: "<<info.portName();
        qDebug()<<"Description: "<<info.description();
        qDebug()<<"Manufactures: "<<info.manufacturer();
        qDebug()<<info.vendorIdentifier();
        qDebug()<<info.productIdentifier();

    }
    */
    updateGraphData();
}
//void DataProcessing::updateGraphData() {}
