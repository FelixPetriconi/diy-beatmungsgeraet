/*******************************************************************************************
*    Copyright 2020 Felix Petriconi
*    Distributed under the Boost Software License, Version 1.0.
*    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
********************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TransportObjects.h"

#include <QMessageBox>
#include <QTimer>

namespace DIYV
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    QTimer::singleShot(0, this, SLOT(showDisclaimer()));
}

MainWindow::~MainWindow()
{
    _renderer.reset();
    delete ui;
}

void MainWindow::setOperationalModeFn(std::function<void(OperationalModes)> fn)
{
    _operationalModeFn = fn;
}

void MainWindow::appendNewMeasurements(const PressureMeasurements &values)
{
    if (ui->_startStop->isChecked())
    {
        _renderer->appendNewValues(values);
    }
}


void MainWindow::init()
{
    connect(ui->_startStop, SIGNAL(clicked(bool)), this, SLOT(startStopPressed(bool)));
    connect(ui->_screenLocked, SIGNAL(clicked(bool)), this, SLOT(lockedPressed(bool)));

    ui->_therapyMode->insertItem(0, tr("PCV"));
    ui->_screenLocked->setChecked(false);
    ui->_startStop->setChecked(false);

    _scene = new QGraphicsScene(0, 0, 640, 330, this);
    ui->_graphicsView->setScene(_scene);
    ui->_graphicsView->setSceneRect(0, 0, 640, 330);
    _renderer = std::make_unique<GraphRenderer>(*_scene);
    _renderer->setRecentTimeWindow(std::chrono::milliseconds(12000));

    startStopPressed(false);
    lockedPressed(false);
}

void MainWindow::showDisclaimer()
{
    QMessageBox messageBox(QMessageBox::Warning, "Disclaimer",
                           tr("This is not a medical device and\nit must not be used in a clinical environment.\nDo you accept?"),
                           QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, this);

    messageBox.move(size().width()-messageBox.size().width() / 2, size().height()-messageBox.size().height() / 2);
    auto choice = messageBox.exec();

    if (choice != QMessageBox::StandardButton::Yes)
    {
        qApp->quit();
    }
}

void MainWindow::startStopPressed(bool value)
{
    ui->_startStop->setText(value? tr("Stop") : tr("Start"));
    ControllerBlock command;
    command.command = value? Command::Start : Command::Stop;
    command.peep = ui->_peep->value();
    command.peep = ui->_maxPressure->value();
    command.irRatio = ui->_irRatio->value();
    command.frequency = ui->_frequency->value();
    if (_operationalModeFn) _operationalModeFn(value? OperationalModes::Start : OperationalModes::Stop);
}

void MainWindow::lockedPressed(bool locked)
{
    ui->_screenLocked->setText(locked? tr("Unlock") : tr("Lock"));
    ui->_therapyMode->setEnabled(!locked);
    ui->_peep->setEnabled(!locked);
    ui->_maxPressure->setEnabled(!locked);
    ui->_irRatio->setEnabled(!locked);
    ui->_frequency->setEnabled(!locked);
}


}
