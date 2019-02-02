#include "dataprocessing.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>


DataProcessing::DataProcessing()
{
    qDebug() << "Number of serial ports:" << QSerialPortInfo::availablePorts().count();

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        qDebug()<<"Name: "<<info.portName();
        qDebug()<<"Description: "<<info.description();
        qDebug()<<"Manufactures: "<<info.manufacturer();
        qDebug()<<info.vendorIdentifier();
        qDebug()<<info.productIdentifier();
        radio.setPort(info);
    }
    radioInfo = QSerialPortInfo(radio);
    qDebug() << radioInfo.portName();
    radio.open(QSerialPort::ReadOnly);
    connect(&radio,&QSerialPort::readyRead,this,&DataProcessing::readData);
    updateGraphData();
}
//void DataProcessing::updateGraphData() {}
void DataProcessing::readData() {
    QByteArray radioData = radio.readAll();
    //double altitude, pressure, humidity, teslaX, teslaY, teslaZ, gyroX, gyroY, gyroZ,
    //        accelerationX, accelerationY, accelerationZ, latitude, longitude, seconds;
    short minutes, hours;
    double data[15];
    if(radioData.length() <= 122) {
        union { char b[8]; double d; };
        for(int i = 0; i < radioData.length()/8; ++i) {
            b[0] = radioData.at(7 + 8*i);
            b[1] = radioData.at(6 + 8*i);
            b[2] = radioData.at(5 + 8*i);
            b[3] = radioData.at(4 + 8*i);
            b[4] = radioData.at(3 + 8*i);
            b[5] = radioData.at(2 + 8*i);
            b[6] = radioData.at(1 + 8*i);
            b[7] = radioData.at(0 + 8*i);
            data[i] = d;
        }
        /*
        altitude      = radioData.mid(0,8).toDouble();
        pressure      = radioData.mid(8,8).toDouble();
        humidity      = radioData.mid(16,8).toDouble();
        teslaX        = radioData.mid(24,8).toDouble();
        teslaY        = radioData.mid(32,8).toDouble();
        teslaZ        = radioData.mid(40,8).toDouble();
        gyroX         = radioData.mid(48,8).toDouble();
        gyroY         = radioData.mid(56,8).toDouble();
        gyroZ         = radioData.mid(64,8).toDouble();
        accelerationX = radioData.mid(72,8).toDouble();
        accelerationY = radioData.mid(80,8).toDouble();
        accelerationZ = radioData.mid(88,8).toDouble();
        latitude      = radioData.mid(96,8).toDouble();
        longitude     = radioData.mid(104,8).toDouble();
        seconds       = radioData.mid(112,8).toDouble();
        minutes       = radioData.mid(120,1).toShort();
        hours         = radioData.mid(121,1).toShort();
        //qDebug() << altitude;
        */
        for( int i = 0; i < 15; i++) {
            qDebug() << data[i];
        }
        //qDebug() << data;
    }
    updateGraphData();
}
