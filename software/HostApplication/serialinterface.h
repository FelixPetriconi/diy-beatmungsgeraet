/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include "datablock.h"
#include "PressureMeasurement.h"

#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>

#include <QObject>
#include <QSerialPort>
#include <QTimer>

namespace DIYV
{

class SerialInterface : public QObject
{
    Q_OBJECT
public:
    SerialInterface();
    ~SerialInterface();

    void registerReceiver(QObject* receiver);
    void sendToController(ControllerBlock data);
    void start();
    void stop();

signals:
    void newMeasurementsArrived(const PressureMeasurements& values);

private slots:
    void sendNewData();

private:
    void sender();
    void receiver();

    QObject* _receiver{nullptr};
    QSerialPort _serialPort;
    std::atomic_bool _stop{false};
    std::queue<ControllerBlock> _sendQueue;
    std::mutex _sendQueueMutex;
    std::condition_variable _sendConditon;
    std::condition_variable _receiveConditon;
    QTimer _testTimer;
    std::thread _sendingThread;
    std::thread _listeningThread;
};

}

#endif // SERIALINTERFACE_H
