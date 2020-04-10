/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphrenderer.h"
#include "serialinterface.h"
#include "PressureMeasurement.h"

#include <memory>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


namespace DIYV
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showDisclaimer();
    void startStopPressed(bool);
    void appendNewMeasurements(const PressureMeasurements& values);
    void lockedPressed(bool);

private:
    void init();

    Ui::MainWindow *ui;
    std::unique_ptr<GraphRenderer> _renderer;
    QGraphicsScene* _scene;

    SerialInterface _serialInterface;
};

}

#endif // MAINWINDOW_H
