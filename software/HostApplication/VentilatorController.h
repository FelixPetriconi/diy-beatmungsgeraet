#ifndef VENTILATORCONTROLLER_H
#define VENTILATORCONTROLLER_H

#include "SerialInterfaceAdapter.h"
#include "TestInterfaceAdapter.h"

#include <functional>
#include <variant>

namespace DIYV
{

class VentilatorController
{
public:
    VentilatorController();

    void setTestMode(bool mode);

    void setOperationalMode(OperationalModes mode);

    void setNewMeasurementsAvailable(std::function<void(const PressureMeasurements& values)> fn);


private:
    std::variant<SerialInterfaceAdapter, TestInterfaceAdapter> _adapter;
    std::function<void(const PressureMeasurements& values)> _newMeasurmentFn;

};

}


#endif // VENTILATORCONTROLLER_H
