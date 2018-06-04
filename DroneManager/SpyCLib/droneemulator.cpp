// Qt
#include <QDebug>
#include <QTime>

// Application
#include <cxmlnode.h>
#include "droneemulator.h"
#include "flightsimulator.h"
#include "batterysimulator.h"
#include "serializehelper.h"
using namespace Core;

//-------------------------------------------------------------------------------------------------

DroneEmulator::DroneEmulator(const QString &sDroneUID, const QString &sVideoUrl, const QGeoCoordinate &initalPosition, QObject *pParent) : QObject(pParent),
    m_sDroneUID(sDroneUID), m_sVideoUrl(sVideoUrl), m_position(initalPosition)
{
    // Register type
    qRegisterMetaType<SpyCore::DroneError>("SpyCore::DroneError");

    // Flight simulator
    m_pFlightSimulator = new FlightSimulator(this);
    connect(m_pFlightSimulator, &FlightSimulator::positionChanged, this, &DroneEmulator::onPositionChanged, Qt::QueuedConnection);

    // Battery simulator
    m_pBatterySimulator = new BatterySimulator(this);
    connect(m_pBatterySimulator, &BatterySimulator::batteryLevelChanged, this, &DroneEmulator::onBatteryLevelChanged, Qt::QueuedConnection);

    // Timer
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    m_droneTimer.setInterval(250);
    connect(&m_droneTimer, &QTimer::timeout, this, &DroneEmulator::timeOut, Qt::QueuedConnection);
    m_droneTimer.start();
}

//-------------------------------------------------------------------------------------------------

DroneEmulator::~DroneEmulator()
{

}

//-------------------------------------------------------------------------------------------------

const QString &DroneEmulator::uid() const
{
    return m_sDroneUID;
}

//-------------------------------------------------------------------------------------------------

const SpyCore::FlightStatus &DroneEmulator::flightStatus() const
{
    return m_eFlightStatus;
}

//-------------------------------------------------------------------------------------------------

QString DroneEmulator::currentStatus() const
{
    CXMLNode droneStatusNode = SerializeHelper::serializeDroneStatus(*this);
    return droneStatusNode.toJsonString();
}

//-------------------------------------------------------------------------------------------------

const QGeoCoordinate &DroneEmulator::position() const
{
    return m_position;
}

//-------------------------------------------------------------------------------------------------

double DroneEmulator::heading() const
{
    return m_dHeading;
}

//-------------------------------------------------------------------------------------------------

int DroneEmulator::batteryLevel() const
{
    return m_iLevel;
}

//-------------------------------------------------------------------------------------------------

int DroneEmulator::returnLevel() const
{
    return m_iReturnLevel;
}

//-------------------------------------------------------------------------------------------------

const QGeoPath &DroneEmulator::safetyPlan() const
{
    return m_safetyPlan;
}

//-------------------------------------------------------------------------------------------------

const QVector<WayPoint> &DroneEmulator::missionPlan() const
{
    return m_missionPlan;
}

//-------------------------------------------------------------------------------------------------

const QVector<WayPoint> &DroneEmulator::landingPlan() const
{
    return m_landingPlan;
}

//-------------------------------------------------------------------------------------------------

const QString &DroneEmulator::videoUrl() const
{
    return m_sVideoUrl;
}

//-------------------------------------------------------------------------------------------------

void DroneEmulator::onPositionChanged(const QGeoCoordinate &geoCoord, double dHeading)
{
    m_position = geoCoord;
    m_dHeading = dHeading;
}

//-------------------------------------------------------------------------------------------------

void DroneEmulator::onBatteryLevelChanged(int iLevel, int iReturn)
{
    m_iLevel = iLevel;
    m_iReturnLevel = iReturn;
}

//-------------------------------------------------------------------------------------------------

void DroneEmulator::takeOff()
{
    if (m_safetyPlan.isEmpty())
        emit droneError(SpyCore::NO_SAFETY, m_sDroneUID);
    else
    if (m_landingPlan.isEmpty())
        emit droneError(SpyCore::NO_LANDING_PLAN, m_sDroneUID);
    else
    if (m_missionPlan.isEmpty())
        emit droneError(SpyCore::NO_MISSION_PLAN, m_sDroneUID);
    else
    {
        m_pFlightSimulator->computeFlightPath(m_missionPlan);
        m_pFlightSimulator->start();
        m_pBatterySimulator->start();
        m_eFlightStatus = SpyCore::FlightStatus::FLYING;
    }
}

//-------------------------------------------------------------------------------------------------

void DroneEmulator::failSafe()
{
    m_pFlightSimulator->stop();
    m_pBatterySimulator->stop();
    m_eFlightStatus = SpyCore::FlightStatus::IDLE;
}

//-------------------------------------------------------------------------------------------------

void DroneEmulator::setSafetyPlan(const QGeoPath &geoPath)
{
    m_safetyPlan = geoPath;
}

//-------------------------------------------------------------------------------------------------

void DroneEmulator::setMissionPlan(const WayPointList &vWayPointList)
{
    m_missionPlan = vWayPointList;
}

//-------------------------------------------------------------------------------------------------

void DroneEmulator::setLandingPlan(const WayPointList &vWayPointList)
{
    m_landingPlan = vWayPointList;
}
