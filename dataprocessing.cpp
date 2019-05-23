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
    QByteArray radioData = radio->readAll();
    //double altitude, pressure, humidity, teslaX, teslaY, teslaZ, gyroX, gyroY, gyroZ,
    //        accelerationX, accelerationY, accelerationZ, latitude, longitude, seconds;
    short minutes, hours;
    //double *data = (double*) malloc(15*sizeof(double));
    //QList<double> data;// = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    bufferedData.append(radioData);
    //qDebug() << bufferedData.length();
    if(bufferedData.length() >= 122) {
        //qDebug() << bufferedData.length();
        QList<double>* data = new QList<double>;
        //qDebug() << "working";
        ///*
        union { char b[4]; float d;};
        for(int i = 0; i < 4; i++) {
            b[3] = bufferedData.at(3 + 4*i);
            b[2] = bufferedData.at(2 + 4*i);
            b[1] = bufferedData.at(1 + 4*i);
            b[0] = bufferedData.at(0 + 4*i);
            data->append(d);
            //qDebug() << d;
        }
        //*/
        /*
        union { char b[8]; double d;};

        for(int i = 0; i < 15; ++i) {
            b[0] = bufferedData.at(7 + 8*i);
            b[1] = bufferedData.at(6 + 8*i);
            b[2] = bufferedData.at(5 + 8*i);
            b[3] = bufferedData.at(4 + 8*i);
            b[4] = bufferedData.at(3 + 8*i);
            b[5] = bufferedData.at(2 + 8*i);
            b[6] = bufferedData.at(1 + 8*i);
            b[7] = bufferedData.at(0 + 8*i);
            qDebug() << d;
        }
        */
        union { char a; short n; };
        a = bufferedData.at(120);
        minutes = n;
        a = bufferedData.at(121);
        hours = n;
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
        //qDebug() << bufferedData.length();
        bufferedData.remove(0,122);
        qDebug() << bufferedData.length() << "\n";
        //qDebug() << data;
        //qDebug() << "working";
        emit updateGraphData(data, minutes, hours);
        //emit testConnection();
        //qApp->processEvents();
        //QCoreApplication::processEvents();
        //qDebug() << QThread::currentThread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
        //Sleep(10);
    }

}
