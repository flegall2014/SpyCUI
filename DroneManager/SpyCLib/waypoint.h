#ifndef WAYPOINT_H
#define WAYPOINT_H

// Qt
#include <QGeoCoordinate>
#include <QVector>
#include <QMap>
#include <QVariant>

// Application
#include <spycore.h>
#include "spyclib_global.h"

namespace Core {
class SPYCLIBSHARED_EXPORT WayPoint
{
public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    WayPoint();

    //! Constructor
    WayPoint(const QGeoCoordinate &geoCoord, const SpyCore::PointType &eType=SpyCore::POINT);

    //! Destructor
    virtual ~WayPoint();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return geoCoord
    const QGeoCoordinate &geoCoord() const;

    //! Set geoCoord
    void setGeoCoord(const QGeoCoordinate &geoCoord);

    //! Return type
    const SpyCore::PointType &type() const;

    //! Set type
    void setType(const SpyCore::PointType &eType);

    //! Return speed
    int speed() const;

    //! Set speed
    void setSpeed(int iSpeed);

    //! Return true if clockwise
    bool clockWise() const;

    //! Set clockwise
    void setClockWise(bool bClockWise);

    //! Return metadata
    const QHash<QString, double> &metaData() const;

    //! Set metadata
    void setMetaData(const QString &sKey, double dValue);

private:
    //! Geo coordinate
    QGeoCoordinate m_geoCoord;

    //! Point type
    SpyCore::PointType m_eType = SpyCore::POINT;

    //! Clock wise?
    bool m_bClockWise = true;

    //! Point metadata
    QHash<QString, double> m_hMetaData;

    //! Speed
    SpyCore::PointSpeed m_eSpeed = SpyCore::ECO;
};
}

typedef QVector<Core::WayPoint> WayPointList;

#endif // WAYPOINT_H
