#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include "Modes.h"
#include "TransportObjects.h"
#include "PressureMeasurement.h"

#include <functional>
#include <memory>

#include <QObject>
#include <QTimer>

namespace DIYV
{

class TestInterfaceAdapter;

class DataGenerator : public QObject
{
Q_OBJECT
public:
    DataGenerator(TestInterfaceAdapter& adapter);
    virtual ~DataGenerator() = default;
    void setOperationalMode(OperationalModes mode);

private slots:
    void sendNewData();

private:
    TestInterfaceAdapter& _adapter;
    QTimer _testTimer;
};


class TestInterfaceAdapter
{   
    friend class DataGenerator;

public:
    TestInterfaceAdapter();
    ~TestInterfaceAdapter();
    TestInterfaceAdapter(TestInterfaceAdapter&&) = default;
    TestInterfaceAdapter& operator=(TestInterfaceAdapter&&) = default;

    void sendCommand(ControllerBlock data);
    void setOperationalMode(OperationalModes mode);

    void newMeasurementsArrived(const PressureMeasurements &values);
    void setNewMeasurementArrived(std::function<void(const PressureMeasurements&)> fn);

private:
    void sendNewData();

    std::unique_ptr<DataGenerator> _dataGenerator;
    std::function<void(const PressureMeasurements&)> _newMeasurementArrivedFn;
};


}

#endif // TESTCONTROLLER_H
