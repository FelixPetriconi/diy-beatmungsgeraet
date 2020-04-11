/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#ifndef DATABLOCK_H
#define DATABLOCK_H

#include <cstdint>

namespace DIYV
{

struct DataBlock
{
    std::int32_t status;
    std::int32_t pressure;
    std::int32_t o2;
    std::int32_t c02;
    std::uint8_t counter;
};

enum class TherapyMode
{
    PCV
};

enum class Command
{
    Stop,
    Start,
    Calibrate,
    SelftTest
};

struct ControllerBlock
{
    Command command;
    double peep;
    double maxPressure;
    double irRatio;
    int frequency;
    std::uint8_t counter;
};

}



#endif // DATABLOCK_H
