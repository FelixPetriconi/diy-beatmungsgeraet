/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#include "GraphRenderer.h"

#include "TemplateHelpers.h"

#include <QGraphicsPathItem>
#include <QFontMetrics>
#include <algorithm>

#include <iostream>

namespace
{
    auto currentPenColor = QPen(Qt::white);
    auto previousPenColor = QPen(Qt::red);

    const auto XAxistStart = 20.0;
    const auto XAxisEnd = 600.0;
    const auto YAxisStart = 300.0;
    const auto YAxisEnd = 5.0;

    const auto mBarMax = 45.0;
}
namespace DIYV
{

GraphRenderer::GraphRenderer(QGraphicsScene& scene)
    : _scene(scene)
{
    currentPenColor.setWidth(2);
    previousPenColor.setWidth(2);
    _scene.setBackgroundBrush(QBrush{Qt::blue});
    _currentGraph = _scene.addPath(QPainterPath(), currentPenColor, QBrush{Qt::NoBrush});
    _previousGraph = _scene.addPath(QPainterPath(), previousPenColor, QBrush{Qt::NoBrush});

    addDescription();
}

void GraphRenderer::addDescription()
{

    QPainterPath description;
    {
        QPolygonF axis;
        // arrow
        axis.append(QPointF{XAxistStart+4, YAxisEnd+5 });
        axis.append(QPointF{XAxistStart, YAxisEnd });
        axis.append(QPointF{XAxistStart-4, YAxisEnd+5 });

        axis.append(QPointF{XAxistStart, YAxisEnd });
        axis.append(QPointF{XAxistStart, YAxisStart});
        axis.append(QPointF{XAxisEnd, YAxisStart});

        axis.append(QPointF{XAxisEnd-5, YAxisStart-4});
        axis.append(QPointF{XAxisEnd, YAxisStart});
        axis.append(QPointF{XAxisEnd-5, YAxisStart+4});

        description.addPolygon(axis);
    }

    std::vector<int> yReferenceLinePositions;
    {
        QPainterPath referenceLinePath;
        for (auto i = 5; i < static_cast<int>(mBarMax); i += 5)
        {
            auto yPos = YAxisStart - (YAxisStart-YAxisEnd) / mBarMax * i;
            QPolygonF yUnitLines;
            yUnitLines.append(QPointF(XAxistStart, yPos));
            yUnitLines.append(QPointF(XAxisEnd, yPos));
            referenceLinePath.addPolygon(yUnitLines);
            yReferenceLinePositions.push_back(yPos);
        }
        QPen pen(QBrush(Qt::cyan), 1.0, Qt::DotLine);
        _scene.addPath(referenceLinePath, pen);
    }

    QString yAxisDescription("mBar");
    QFont font("Helvetica", 12);
    QFontMetrics metric(font);
    description.addText(QPointF(XAxistStart + 4.0, 5.0+metric.boundingRect(yAxisDescription).height()), font, yAxisDescription);

    for (std::size_t i = 1u, n = 10; i < yReferenceLinePositions.size(); i += 2, n += 10)
    {
        auto text = QString::number(n);
        auto box = metric.tightBoundingRect(text);
        description.addText(QPointF(XAxistStart - box.width() - 2, yReferenceLinePositions[i]+box.height()/2), font, text);
    }

    _scene.addPath(description, currentPenColor);
}


void GraphRenderer::setXScale(double range)
{
    _xScale = range;
}

void GraphRenderer::setYScale(double scale)
{
    _yScale = scale;
}

void GraphRenderer::setRecentTimeWindow(std::chrono::milliseconds recentWindow)
{
    _recentWindow = recentWindow;
}

void GraphRenderer::appendNewValues(const std::vector<PressureMeasurement>& values)
{
    if (_currentStart == std::chrono::steady_clock::time_point{} && !values.empty())
    {
        _currentStart = values[0].timePoint;
    }
    _values.insert(_values.end(), values.begin(), values.end());

    update();
}

void GraphRenderer::setAllValues(PressureMeasurements values)
{
    _values = std::move(values);

    update();
}

void GraphRenderer::clear()
{
    _values.clear();
}

void GraphRenderer::update()
{
    if (_currentStart + _recentWindow < std::chrono::steady_clock::now())
    {
        _currentStart += _recentWindow;
        std::swap(_previousGraph, _currentGraph);
        _currentGraph->setPen(currentPenColor);
        _previousGraph->setPen(previousPenColor);
    }

    auto itCurrentStart = std::find_if(_values.rbegin(), _values.rend(), [this](const auto& item) { return item.timePoint <= _currentStart; });
    QPolygonF currentPoints;
    if (itCurrentStart == _values.rend())
    {
        convertMeasurements(_values.begin(), _values.end(), _values[0].timePoint);
    }
    else
    {
        currentPoints = convertMeasurements(make_iterator(itCurrentStart), _values.end(), _currentStart);
    }
    QPainterPath path;
    path.addPolygon(currentPoints);
    _currentGraph->setPath(path);
}

void GraphRenderer::start()
{
    _values.clear();
    _currentStart = std::chrono::steady_clock::time_point{};
    _currentGraph->setPath(QPainterPath{});
    _previousGraph->setPath(QPainterPath{});
}

QPolygonF GraphRenderer::convertMeasurements(Measurements::const_iterator begin, Measurements::const_iterator end,
                                             std::chrono::steady_clock::time_point offset) const
{
    QPolygonF result;
    result.reserve(end - begin);
    auto timeScale = (XAxisEnd-XAxistStart)/std::chrono::duration_cast<std::chrono::milliseconds>(_recentWindow).count();
    std::transform(begin, end, std::back_inserter(result), [offset, timeScale](const auto& item) {
        double timePoint = XAxistStart + timeScale * std::chrono::duration_cast<std::chrono::milliseconds>(item.timePoint - offset).count() ;
        auto value = YAxisStart + item.value/mBarMax * (YAxisEnd-YAxisStart);

        return QPointF(timePoint, value );});

    return result;
}



}
