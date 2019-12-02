#include "eggfinderreader.h"

EggFinderReader::EggFinderReader()
{
    radio = new QSerialPort(this);
    timer = new QTimer(this);
    startTimer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EggFinderReader::enumeratePorts);
    connect(radio, &QSerialPort::errorOccurred, this, &EggFinderReader::radioUnplugged);
    connect(startTimer, &QTimer::timeout, this, &EggFinderReader::start);
    timer->setInterval(100);
    timer->start();
    startTimer->setInterval(300);
    startTimer->start();

    connect(radio,&QSerialPort::readyRead,this,&EggFinderReader::readData);
}

void EggFinderReader::readData() {

}
