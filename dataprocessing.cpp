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
    radio->setBaudRate(QSerialPort::Baud19200);
    qDebug() << radioInfo.portName();
    radio->open(QSerialPort::WriteOnly);
    radio->write("Stop");
    //Sleep(500);
    radio->close();
    radio->open(QSerialPort::ReadWrite);
    radio->flush();
    radio->write("Start");
    connect(radio,&QSerialPort::readyRead,this,&DataProcessing::readData);
}
void DataProcessing::readData() {
    QByteArray radioData = radio->readAll(); //Add all available data to a temporary array
    bufferedData.append(radioData); //Add all collected data to global array
    if(bufferedData.length() >= 88) {

        QList<double>* data = new QList<double>; //Create list for converted data
        union { char b[4]; float d;}; //Convert bytes to floats
        for(int i = 0; i < 21; i++) {
            b[3] = bufferedData.at(3 + 4*i);
            b[2] = bufferedData.at(2 + 4*i);
            b[1] = bufferedData.at(1 + 4*i);
            b[0] = bufferedData.at(0 + 4*i);
            data->append(d);
        }
        data->append((int)bufferedData.at(84));
        data->append((int)bufferedData.at(85));
        data->append((int)bufferedData.at(86));
        data->append((int)bufferedData.at(87));
        //qDebug() << data->at(0) << "\n";
        //union { char a; short n; }; //Convert bytes to shorts
        bufferedData.remove(0,88); //Remove data from global array
        //free(data);
        emit updateGraphData(data); //Send data to GUI thread
    }

}

void DataProcessing::sendMessage(char *message, int length) {
    if(radio->isWritable()) {
        radio->write(message, length);
        radio->flush();
    }
}
