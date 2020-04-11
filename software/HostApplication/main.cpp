/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#include "mainwindow.h"
#include "VentilatorController.h"

#include <QApplication>

using namespace DIYV;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VentilatorController controller;
    controller.setTestMode(true);
    MainWindow w;

    w.setOperationalModeFn([&controller](OperationalModes mode) { controller.setOperationalMode(mode); });
    controller.setNewMeasurementsAvailable([&w](const PressureMeasurements& val) { w.appendNewMeasurements(val); });

    w.show();
    return a.exec();
}
