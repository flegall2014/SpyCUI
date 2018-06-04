#ifndef SERIALIZEHELPER_H
#define SERIALIZEHELPER_H

// Application
#include "droneemulator.h"
#include "waypoint.h"
#include <cxmlnode.h>
#include "spyclib_global.h"
class BaseShape;

namespace Core {
class SPYCLIBSHARED_EXPORT SerializeHelper
{
public:
    //! Serialize drone status
    static CXMLNode serializeDroneStatus(const DroneEmulator &drone);

    //! Deserialize drone status
    static void deserializeDroneStatus(const QString &sDroneStatus, QString &sDroneUID, SpyCore::FlightStatus &eFlightStatus, QGeoCoordinate &position, double &dHeading, int &iBatteryLevel, int &iReturnLevel, QString &sVideoUrl);

    //! Serialize drone position
    static CXMLNode serializePosition(const QGeoCoordinate &geoCoord, double dHeading);

    //! Deserialize position
    static void deserializePosition(const CXMLNode &positionNode, QGeoCoordinate &geoCoord, double &dHeading);

    //! Serialize battery level
    static CXMLNode serializeBatteryLevel(double dLevel, double dReturn);

    //! Deserialize battery level
    static void deserializeBatteryLevel(const CXMLNode &batteryNode, int &iLevel, int &iReturnLevel);

    //! Serialize mission plan
    static CXMLNode serializeMissionPlan(const QVector<Core::WayPoint> &vWayPoints, const QString &sDroneUID);

    //! Serialize mission plan
    static void deserializeMissionPlan(const QString &sMissionPlan, QVector<WayPoint> &vWayPoints, QString &sDroneUID);

    //! Serialize safety plan
    static CXMLNode serializeSafetyPlan(const QGeoPath &vWayPoints, const QString &sDroneUID);

    //! Deserialize safety plan
    static void deserializeSafetyPlan(const QString &sSafetyPlan, QGeoPath &geoPath, QString &sDroneUID);

    //! Serialize landing plan
    static CXMLNode serializeLandingPlan(const QVector<Core::WayPoint> &vWayPoints, const QString &sDroneUID);

    //! Serialize mission plan
    static void deserializeLandingPlan(const QString &sMissionPlan, QVector<WayPoint> &vWayPoints, QString &sDroneUID);

    //! Serialize drone error
    static CXMLNode serializeDroneError(const  SpyCore::DroneError &eDroneError, const QString &sDroneUID);

    //! Serialize drone error
    static void deSerializeDroneError(const QString &sErrorNode, int &iErrorCode, QString &sDroneUID);

    //! Serialize take off request
    static CXMLNode serializeTakeOffRequest(const QString &sDroneUID);

    //! Deserialize take off reques
    static void deserializeTakeOffRequest(const CXMLNode &takeOffRequestNode, QString &sDroneUID);

    //! Serialize fail safe
    static CXMLNode serializeFailSafeRequest(const QString &sDroneUID);

    //! Serialize fail safe done
    static CXMLNode serializeFailSafeDone(const QString &sDroneUID);

    //! Deserialize fail safe request
    static void deserializeFailSafeRequest(const CXMLNode &failSafeRequest, QString &sDroneUID);

    //! Write geopath
    static CXMLNode writeGeoPath(const QVector<Core::WayPoint> &plan, const QString &sPlanType, const QString &sDroneUID);

    //! Write geopath
    static CXMLNode writeGeoPath(const QGeoPath &plan, const QString &sPlanType, const QString &sDroneUID);

    //! Read plan (mission/landing)
    static void readPlan(const CXMLNode &node, WayPointList &vWayPointList);

    //! Read geopath (safety)
    static QGeoPath readGeoPath(const CXMLNode &node);

    //! Return message type
    static QString messageType(const QString &sMessage);
};
}

#endif // SERIALIZEHELPER_H
