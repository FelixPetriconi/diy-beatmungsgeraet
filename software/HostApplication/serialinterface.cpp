/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#include "serialinterface.h"

#include <cmath>

namespace
{
    const auto ramp = 200;
    const auto inspiration = 1000 - ramp;
    const auto exspiration = inspiration + 2000;
    const auto peep = 8;
    const auto maxp = 28;
    const auto interval = 10;
    const auto updateInterval = 100;

    auto calculateTestPressure(int tms)
    {
        if (tms < ramp)
        {
            return peep + static_cast<int>(std::round(tms* (maxp-peep) / ramp));
        }
        else if (tms < inspiration)
        {
            return maxp;
        }
        else if (tms < exspiration)
        {
            return static_cast<int>(std::round(maxp - (tms-inspiration) * (maxp-peep) / (exspiration-inspiration)));
        }
        else
        {
            return 0;
        }
    }

    const std::vector<int> testValues = []
        {
            std::vector<int> result;
            for (auto i = 0; i < exspiration; i += interval)
            {
                result.push_back(calculateTestPressure(i));
            }

            return result;
        }();

    std::size_t currentTestMeasurePoint=0;

    std::chrono::steady_clock::time_point currentTimePoint;
}

DIYV::SerialInterface::SerialInterface()
{
    _testTimer.setSingleShot(false);
    connect(&_testTimer, SIGNAL(timeout()), this, SLOT(sendNewData()));
}

DIYV::SerialInterface::~SerialInterface()
{
    _stop = true;
    _sendConditon.notify_one();
    _receiveConditon.notify_one();

    if (_listeningThread.joinable()) _listeningThread.join();
    if (_sendingThread.joinable()) _sendingThread.join();
}

void DIYV::SerialInterface::registerReceiver(QObject *receiver)
{
    _receiver = receiver;
}

void DIYV::SerialInterface::sendToController(DIYV::ControllerBlock data)
{
    {
        std::unique_lock<std::mutex> block{_sendQueueMutex};
        _sendQueue.push(data);
    }
    _sendConditon.notify_one();
}

void DIYV::SerialInterface::start()
{
    _testTimer.start(updateInterval);
}

void DIYV::SerialInterface::stop()
{
    _testTimer.stop();
}

void DIYV::SerialInterface::sendNewData()
{
    currentTimePoint = std::chrono::steady_clock::now() - std::chrono::milliseconds(updateInterval);
    PressureMeasurements data(updateInterval/interval);
    std::generate(data.begin(), data.end(),
        []
        {
            PressureMeasurement result{currentTimePoint, testValues[currentTestMeasurePoint++]};
            currentTimePoint += std::chrono::milliseconds(interval);
            if (currentTestMeasurePoint == testValues.size())
            {
                currentTestMeasurePoint = 0;
            }
            return result;
    });
    emit newMeasurementsArrived(data);
}

void DIYV::SerialInterface::sender()
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

void DIYV::SerialInterface::receiver()
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
