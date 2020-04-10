/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#ifndef GRAPHRENDERER_H
#define GRAPHRENDERER_H

#include "PressureMeasurement.h"

#include <QGraphicsScene>

#include <vector>

namespace DIYV
{

class GraphRenderer
{
public:
    using Measurements = std::vector<PressureMeasurement>;

    GraphRenderer(QGraphicsScene& scene);

    QGraphicsScene& scene();

    void setXScale(double scale);

    void setYScale(double scale);

    void setRecentTimeWindow(std::chrono::milliseconds recentWindow);

    void appendNewValues(const Measurements& values);

    void setAllValues(PressureMeasurements values);

    void clear();

    void update();

    void start();

private:

    QPolygonF convertMeasurements(Measurements::const_iterator begin, Measurements::const_iterator end, std::chrono::steady_clock::time_point offset) const;

    QGraphicsScene& _scene;
    QGraphicsPathItem* _currentGraph = nullptr;
    QGraphicsPathItem* _previousGraph = nullptr;
    double _xScale = 60.0;
    double _yScale = 1.0;
    std::chrono::milliseconds _recentWindow;
    std::chrono::steady_clock::time_point _currentStart;
    std::vector<PressureMeasurement> _values;
    void addDescription();
};

}
#endif // GRAPHRENDERER_H
