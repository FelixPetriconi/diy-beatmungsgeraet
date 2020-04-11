/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#include "TestInterfaceAdapter.h"
#include <cmath>
#include <QObject>
#include <QTimer>

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

namespace DIYV
{

DataGenerator::DataGenerator(TestInterfaceAdapter& adapter)
    : _adapter(adapter)
{
    _testTimer.setSingleShot(false);
    connect(&_testTimer, SIGNAL(timeout()), this, SLOT(sendNewData()));
}

void DataGenerator::setOperationalMode(OperationalModes mode)
{
    switch (mode)
    {
        case OperationalModes::Stop:
            _testTimer.stop();
        break;
    case OperationalModes::Start:
        _testTimer.start(updateInterval);
        break;
    default:
        assert(!"Unknown mode");

    }
}


void DataGenerator::sendNewData()
{
    _adapter.sendNewData();
}


TestInterfaceAdapter::TestInterfaceAdapter()
    : _dataGenerator(new DataGenerator(*this))
{
}

TestInterfaceAdapter::~TestInterfaceAdapter()
{
}

void TestInterfaceAdapter::sendCommand(ControllerBlock /*data*/)
{
}

void TestInterfaceAdapter::setOperationalMode(OperationalModes mode)
{
    _dataGenerator->setOperationalMode(mode);
}

void TestInterfaceAdapter::setNewMeasurementArrived(std::function<void (const PressureMeasurements &)> fn)
{
    _newMeasurementArrivedFn = fn;
}

void TestInterfaceAdapter::sendNewData()
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
    _newMeasurementArrivedFn(data);
}


}
