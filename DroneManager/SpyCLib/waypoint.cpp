// Application
#include "waypoint.h"
using namespace Core;

//-------------------------------------------------------------------------------------------------

WayPoint::WayPoint()
{
    qRegisterMetaType<WayPoint>("WayPoint");
}

//-------------------------------------------------------------------------------------------------

WayPoint::WayPoint(const QGeoCoordinate &geoCoord, const SpyCore::PointType &eType) : m_geoCoord(geoCoord), m_eType(eType)
{

}

//-------------------------------------------------------------------------------------------------

WayPoint::~WayPoint()
{

}

//-------------------------------------------------------------------------------------------------

const QGeoCoordinate &WayPoint::geoCoord() const
{
    return m_geoCoord;
}

//-------------------------------------------------------------------------------------------------

void WayPoint::setGeoCoord(const QGeoCoordinate &geoCoord)
{
    m_geoCoord = geoCoord;
}

//-------------------------------------------------------------------------------------------------

const SpyCore::PointType &WayPoint::type() const
{
    return m_eType;
}

//-------------------------------------------------------------------------------------------------

void WayPoint::setType(const SpyCore::PointType &eType)
{
    m_eType = eType;
}

//-------------------------------------------------------------------------------------------------

bool WayPoint::clockWise() const
{
    return m_bClockWise;
}

//-------------------------------------------------------------------------------------------------

void WayPoint::setClockWise(bool bClockWise)
{
    m_bClockWise = bClockWise;
}

//-------------------------------------------------------------------------------------------------

const QHash<QString, double> &WayPoint::metaData() const
{
    return m_hMetaData;
}

//-------------------------------------------------------------------------------------------------

void WayPoint::setMetaData(const QString &sKey, double dValue)
{
    m_hMetaData[sKey] = dValue;
}

//-------------------------------------------------------------------------------------------------

int WayPoint::speed() const
{
    return (int)m_eSpeed;
}

//-------------------------------------------------------------------------------------------------

void WayPoint::setSpeed(int iSpeed)
{
    m_eSpeed = (SpyCore::PointSpeed)iSpeed;
}
