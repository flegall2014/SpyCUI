#ifndef FLIGHTSIMULATOR_H
#define FLIGHTSIMULATOR_H

// Qt
#include <QObject>
#include <QGeoCoordinate>
#include <QGeoPath>
#include <QVector>
#include <QTimer>

// Application
#include "basesimulator.h"
#include "waypoint.h"
#include "spyclib_global.h"

namespace Core {
class SPYCLIBSHARED_EXPORT FlightSimulator : public Core::BaseSimulator
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    FlightSimulator(QObject *pParent=nullptr);

    //! Destructor
    ~FlightSimulator();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Compute points
    void computeFlightPath(const QVector<WayPoint> &geoPath, int iPointCount=100);

    //! Play
    virtual void start();

    //! Stop
    virtual void stop();

private:
    //! Detailed path
    QVector<QGeoCoordinate> m_vDetailedPath;

    //! Heading
    QVector<double> m_vHeading;

    //! Timer
    QTimer m_timer;

    //! Point index
    int m_iPointIndex = 0;

public slots:
    //! Time out
    void onTimeOut();

signals:
    //! PositionChanged
    void positionChanged(const QGeoCoordinate &geoCoord, double dHeading);
};
}

#endif // FLIGHTSIMULATOR_H
