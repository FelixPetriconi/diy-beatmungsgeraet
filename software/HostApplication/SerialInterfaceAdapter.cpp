/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#include "SerialInterfaceAdapter.h"


namespace DIYV
{
class SerialInterfaceAdapter::Interface
{
public:
    Interface(SerialInterfaceAdapter& adapter)
        : _adapter(adapter)
    {
    }

    ~Interface()
    {
        _stop = true;
        _sendConditon.notify_one();
        _receiveConditon.notify_one();

        if (_listeningThread.joinable()) _listeningThread.join();
        if (_sendingThread.joinable()) _sendingThread.join();
    }

    void sendCommand(ControllerBlock data)
    {
        {
            std::unique_lock<std::mutex> block{_sendQueueMutex};
            _sendQueue.push(data);
        }
        _sendConditon.notify_one();
    }

private:

    void sender()
    {
    #if 0
        while (!_stop)
        {
            ControllerBlock data;
            {
                std::unique_lock<std::mutex> block{_sendQueueMutex};
                while (!_stop && !_sendQueue.empty())
                {
                    _sendConditon.wait(block);
                }
                data =_sendQueue.front();
                _sendQueue.pop();
            }
            // send data over the wire
        }
    #endif
    }

    void receiver()
    {
    #if 0
        while (!_stop)
        {
            DataBlock data;
            {
            }
            // send data over the wire
        }
    #endif
    }
    SerialInterfaceAdapter& _adapter;

    QSerialPort _serialPort;
    std::atomic_bool _stop{false};
    std::queue<ControllerBlock> _sendQueue;
    std::mutex _sendQueueMutex;
    std::condition_variable _sendConditon;
    std::condition_variable _receiveConditon;
    std::thread _sendingThread;
    std::thread _listeningThread;
};


SerialInterfaceAdapter::SerialInterfaceAdapter()
    : _interface(new Interface(*this))
{
}

SerialInterfaceAdapter::~SerialInterfaceAdapter()
{
}


void SerialInterfaceAdapter::sendCommand(ControllerBlock data)
{
    _interface->sendCommand(data);
}

void SerialInterfaceAdapter::setOperationalMode(OperationalModes /*mode*/)
{

}

void SerialInterfaceAdapter::setNewMeasurementArrived(std::function<void (const PressureMeasurements &)> fn)
{
    _newMeasurementArrivedFn = fn;
}



}
