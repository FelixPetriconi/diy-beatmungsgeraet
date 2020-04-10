/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#ifndef PRESSUREMEASUREMENT_H
#define PRESSUREMEASUREMENT_H

#include <chrono>
#include <vector>

namespace DIYV
{
struct PressureMeasurement
{
    std::chrono::steady_clock::time_point timePoint;
    int value;
};

using PressureMeasurements = std::vector<PressureMeasurement>;

}


#endif // PRESSUREMEASUREMENT_H
