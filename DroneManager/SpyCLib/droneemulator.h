#ifndef DRONEEMULATOR_H
#define DRONEEMULATOR_H

// Qt
#include <QObject>
#include <QTimer>
#include <QGeoCoordinate>
#include <QGeoPath>
#include <QVector>

// Application
#include "spyclib_global.h"
#include <waypoint.h>
#include <spycore.h>

namespace Core {
class FlightSimulator;
class BatterySimulator;
class SPYCLIBSHARED_EXPORT DroneEmulator : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    DroneEmulator(const QString &sDroneUID, const QString &sVideoUrl, const QGeoCoordinate &initialPos, QObject *pParent=nullptr);

    //! Destructor
    virtual ~DroneEmulator();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return uid
    const QString &uid() const;

    //! Return fly status
    const SpyCore::FlightStatus &flightStatus() const;

    //! Return current status
    QString currentStatus() const;

    //! Return position
    const QGeoCoordinate &position() const;

    //! Return heading
    double heading() const;

    //! Return battery level
    int batteryLevel() const;

    //! Return return level
    int returnLevel() const;

    //! Return safety plan
    const QGeoPath &safetyPlan() const;

    //! Return mission plan
    const QVector<WayPoint> &missionPlan() const;

    //! Return landing plan
    const QVector<WayPoint> &landingPlan() const;

    //! Return video url
    const QString &videoUrl() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Take off
    void takeOff();

    //! Fail safe
    void failSafe();

    //! Set safety plan
    void setSafetyPlan(const QGeoPath &geoPath);

    //! Set mission plan
    void setMissionPlan(const WayPointList &vWayPointList);

    //! Set landing plan
    void setLandingPlan(const WayPointList &vWayPointList);

private:
    //! UID
    QString m_sDroneUID = "";

    //! Status
    SpyCore::FlightStatus m_eFlightStatus = SpyCore::IDLE;

    //! Video url
    QString m_sVideoUrl = "";

    //! Safety
    QGeoPath m_safetyPlan;

    //! Mission plan
    QVector<WayPoint> m_missionPlan;

    //! Landing plan
    QVector<WayPoint> m_landingPlan;

    //! Exclusion area
    QList<QGeoPath> m_lExclusionArea;

    //! Flight simulator
    FlightSimulator *m_pFlightSimulator = nullptr;

    //! Battery simulator
    BatterySimulator *m_pBatterySimulator = nullptr;

    //! Current position
    QGeoCoordinate m_position;

    //! Heading
    double m_dHeading = 0;

    //! Battery level
    int m_iLevel = 0;

    //! Return level
    int m_iReturnLevel = 0;

    //! Drone timer
    QTimer m_droneTimer;

public slots:
    //! Position changed
    void onPositionChanged(const QGeoCoordinate &geoCoord, double dHeading);

    //! Battery level changed
    void onBatteryLevelChanged(int iLevel, int iReturn);

signals:
    //! Time out
    void timeOut();

    //! New message
    void droneError(const SpyCore::DroneError &eDroneError, const QString &sDroneUID);
};
}

#endif // DRONEEMULATOR_H
