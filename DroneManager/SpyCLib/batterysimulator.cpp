// Qt
#include <QDebug>

// Application
#include "batterysimulator.h"
using namespace Core;

//-------------------------------------------------------------------------------------------------

BatterySimulator::BatterySimulator(QObject *pParent) : BaseSimulator(pParent)
{
    m_timer.setInterval(500);
    connect(&m_timer, &QTimer::timeout, this, &BatterySimulator::onTimeOut, Qt::QueuedConnection);
}

//-------------------------------------------------------------------------------------------------

BatterySimulator::~BatterySimulator()
{

}

//-------------------------------------------------------------------------------------------------

void BatterySimulator::start()
{
    m_timer.start();
}

//-------------------------------------------------------------------------------------------------

void BatterySimulator::onTimeOut()
{
    emit batteryLevelChanged(m_iBatteryLevel, m_iReturnLevel);

    m_iBatteryLevel--;
    if (m_iBatteryLevel < 0)
        m_iBatteryLevel = 100;
    m_iReturnLevel = (100-m_iBatteryLevel)/2;
}

//-------------------------------------------------------------------------------------------------

void BatterySimulator::stop()
{
    m_timer.stop();
}
