/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#include "VentilatorController.h"

namespace DIYV
{

VentilatorController::VentilatorController()
{

}

void VentilatorController::setTestMode(bool mode)
{
    if (mode)
    {
        _adapter.emplace<1>();
    }
    else
    {
        _adapter.emplace<0>();
    }
    std::visit([&](auto& item) { item.setNewMeasurementArrived([this](const auto& data){ _newMeasurmentFn(data); }); }, _adapter);
}

void VentilatorController::setOperationalMode(OperationalModes mode)
{
    std::visit([&](auto& item) { item.setOperationalMode(mode); }, _adapter );
}

void VentilatorController::setNewMeasurementsAvailable(std::function<void (const PressureMeasurements &)> fn)
{
    _newMeasurmentFn = fn;
}

}
