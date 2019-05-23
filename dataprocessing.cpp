#include "dataprocessing.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QMetaType>


DataProcessing::DataProcessing()
{
    //Initialize radio on the dedicated thread
    radio = new QSerialPort(this);
    //Output information about the serial ports and connect
    qDebug() << "Number of serial ports:" << QSerialPortInfo::availablePorts().count();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        qDebug()<<"Name: "<<info.portName();
        qDebug()<<"Description: "<<info.description();
        qDebug()<<"Manufactures: "<<info.manufacturer();
        qDebug()<<info.vendorIdentifier();
        qDebug()<<info.productIdentifier();
        radio->setPort(info);
    }
    //Get the radio info, set the baud rate, open the radio,
    //and connect the read signal
    radioInfo = QSerialPortInfo(*radio);
    radio->setBaudRate(QSerialPort::Baud115200);
    qDebug() << radioInfo.portName();
    radio->open(QSerialPort::ReadOnly);
    connect(radio,&QSerialPort::readyRead,this,&DataProcessing::readData);
}

void DataProcessing::readData() {
    //Save incoming data to byte array
    QByteArray radioData = radio->readAll();
    short minutes, hours;
    //Append incoming data to the buffer
    bufferedData.append(radioData);
    //If enough data has been accumulated then process it
    if(bufferedData.length() >= 122) {
        QList<double>* data = new QList<double>;
        //Convert bytes back into floats and append them to the double list
        union { char b[4]; float d;};
        for(int i = 0; i < 4; i++) {
            b[3] = bufferedData.at(3 + 4*i);
            b[2] = bufferedData.at(2 + 4*i);
            b[1] = bufferedData.at(1 + 4*i);
            b[0] = bufferedData.at(0 + 4*i);
            data->append(d);
        }
        //Send data to the GUI thread to be handled
        emit updateGraphData(data, minutes, hours);
    }

}
