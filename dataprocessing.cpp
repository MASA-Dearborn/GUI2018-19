#include "dataprocessing.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QMetaType>


DataProcessing::DataProcessing()
{
    radio = new QSerialPort(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DataProcessing::enumeratePorts);
    connect(radio, &QSerialPort::errorOccurred, this, &DataProcessing::radioUnplugged);
    timer->setInterval(100);
    timer->start();
    /*
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        radio->setPort(info);
        break;
    }
    radioInfo = QSerialPortInfo(*radio);
    radio->setBaudRate(QSerialPort::Baud19200);
    radio->open(QSerialPort::WriteOnly);
    radio->write("Stop");
    radio->close();
    radio->open(QSerialPort::ReadWrite);
    radio->flush();
    radio->write("Start");
    */
    connect(radio,&QSerialPort::readyRead,this,&DataProcessing::readData);
}
void DataProcessing::readData() {

    if(isStopped) {
        radio->flush();
        return;
    }

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
        qDebug() << bufferedData.length() << "\n";
        //free(data);
        emit updateGraphData(data); //Send data to GUI thread
    }

}

void DataProcessing::sendMessage(QByteArray *message) {
    if(radio->isWritable()) {
        qDebug() << "Write";
        radio->write(*message);
    }
    free(message);
}

void DataProcessing::enumeratePorts() {
    QList<QString> *names = new QList<QString>;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        names->append(info.portName());
    }
    emit addPort(names);
}

void DataProcessing::radioUnplugged(QSerialPort::SerialPortError error) {
    if(error == QSerialPort::ResourceError) {
        radio->close();
    }
}

void DataProcessing::changePort(QString name) {
    if(radio->isOpen()) {
        radio->write("Stop");
        radio->close();
    }
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        if(name.compare(info.portName()) == 0) {
            radio->setPort(info);
            radio->open(QSerialPort::WriteOnly);
            radio->setBaudRate(QSerialPort::Baud19200);
            QByteArray *message = new QByteArray("Stop", 4);
            sendMessage(message);
            radio->close();
            radio->open(QSerialPort::ReadWrite);
            radio->setBaudRate(QSerialPort::Baud19200);
            radio->flush();
            isStopped = true;
            break;
        }
    }
}

void DataProcessing::changeStopState(bool state) {
    isStopped = state;
}
