// Qt
#include <QGeoCoordinate>
#include <QDebug>
#include <QFile>

// Application
#include "serializehelper.h"
#include "waypoint.h"
#include "defs.h"
using namespace Core;

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeDroneStatus(const DroneEmulator &drone)
{
    // Create status node
    CXMLNode rootNode;
    CXMLNode statusNode(TAG_DRONE_STATUS);
    statusNode.attributes()[ATTR_DRONE_UID] = drone.uid();
    statusNode.attributes()[ATTR_FLIGHT_STATUS] = QString::number(drone.flightStatus());
    statusNode.attributes()[ATTR_VIDEO_URL] = drone.videoUrl();

    // Serialize position
    CXMLNode positionNode = serializePosition(drone.position(), drone.heading());
    statusNode << positionNode;

    // Serialize battery level
    CXMLNode batteryLevelNode = serializeBatteryLevel(drone.batteryLevel(), drone.returnLevel());
    statusNode << batteryLevelNode;

    rootNode.nodes() << statusNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializeDroneStatus(const QString &sDroneStatus, QString &sDroneUID, SpyCore::FlightStatus &eFlightStatus, QGeoCoordinate &position, double &dHeading, int &iBatteryLevel, int &iReturnLevel, QString &sVideoUrl)
{
    CXMLNode msgNode = CXMLNode::parseJSON(sDroneStatus);

    // Retrieve drone status node
    CXMLNode droneStatusNode = msgNode.getNodeByTagName(TAG_DRONE_STATUS);
    sDroneUID = droneStatusNode.attributes()[ATTR_DRONE_UID];
    sVideoUrl = droneStatusNode.attributes()[ATTR_VIDEO_URL];
    eFlightStatus = (SpyCore::FlightStatus)droneStatusNode.attributes()[ATTR_FLIGHT_STATUS].toInt();

    // Retrieve position node
    if (!droneStatusNode.nodes().isEmpty())
    {
        CXMLNode positionNode = droneStatusNode.nodes().first().getNodeByTagName(TAG_POSITION);
        deserializePosition(positionNode, position, dHeading);

        // Retrieve battery node
        CXMLNode batteryNode = droneStatusNode.nodes().first().getNodeByTagName(TAG_BATTERY);
        deserializeBatteryLevel(batteryNode, iBatteryLevel, iReturnLevel);
    }
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializePosition(const QGeoCoordinate &geoCoord, double dHeading)
{
    CXMLNode rootNode;
    CXMLNode positionNode(TAG_POSITION);
    positionNode.attributes()[ATTR_LATITUDE] = QString::number(geoCoord.latitude());
    positionNode.attributes()[ATTR_LONGITUDE] = QString::number(geoCoord.longitude());
    positionNode.attributes()[ATTR_ALTITUDE] = QString::number(geoCoord.altitude());
    positionNode.attributes()[ATTR_HEADING] = QString::number(dHeading);
    rootNode.nodes() << positionNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializePosition(const CXMLNode &positionNode, QGeoCoordinate &geoCoord, double &dHeading)
{
    double dLatitude = positionNode.attributes()[ATTR_LATITUDE].toDouble();
    double dLongitude = positionNode.attributes()[ATTR_LONGITUDE].toDouble();
    double dAltitude = positionNode.attributes()[ATTR_ALTITUDE].toDouble();
    dHeading = positionNode.attributes()[ATTR_HEADING].toDouble();
    geoCoord.setLatitude(dLatitude);
    geoCoord.setLongitude(dLongitude);
    geoCoord.setAltitude(dAltitude);
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeBatteryLevel(double dLevel, double dReturn)
{
    CXMLNode rootNode;
    CXMLNode batteryNode(TAG_BATTERY);
    batteryNode.attributes()[ATTR_LEVEL] = QString::number(dLevel);
    batteryNode.attributes()[ATTR_RETURN] = QString::number(dReturn);
    rootNode.nodes() << batteryNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializeBatteryLevel(const CXMLNode &batteryNode, int &iLevel, int &iReturnLevel)
{
    iLevel = batteryNode.attributes()[ATTR_LEVEL].toInt();
    iReturnLevel = batteryNode.attributes()[ATTR_RETURN].toInt();
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeMissionPlan(const QVector<Core::WayPoint> &vWayPoints, const QString &sDroneUID)
{
    return writeGeoPath(vWayPoints, TAG_MISSION_PLAN, sDroneUID);
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializeMissionPlan(const QString &sMissionPlan, QVector<WayPoint> &vWayPoints, QString &sDroneUID)
{
    // Read waypoints
    CXMLNode rootNode = CXMLNode::parseJSON(sMissionPlan);
    CXMLNode missionPlanNode = rootNode.getNodeByTagName(TAG_MISSION_PLAN);
    sDroneUID = missionPlanNode.attributes()[ATTR_DRONE_UID];
    readPlan(missionPlanNode, vWayPoints);
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeSafetyPlan(const QGeoPath &wayPoints, const QString &sDroneUID)
{
    return writeGeoPath(wayPoints, TAG_SAFETY_PLAN, sDroneUID);
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializeSafetyPlan(const QString &sSafetyPlan, QGeoPath &geoPath, QString &sDroneUID)
{
    // Read waypoints
    CXMLNode rootNode = CXMLNode::parseJSON(sSafetyPlan);
    CXMLNode safetyPlanNode = rootNode.getNodeByTagName(TAG_SAFETY_PLAN);
    sDroneUID = safetyPlanNode.attributes()[ATTR_DRONE_UID];
    geoPath = readGeoPath(safetyPlanNode);
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeLandingPlan(const QVector<Core::WayPoint> &vWayPoints, const QString &sDroneUID)
{
    return writeGeoPath(vWayPoints, TAG_LANDING_PLAN, sDroneUID);
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializeLandingPlan(const QString &sLandingPlan, QVector<WayPoint> &vWayPoints, QString &sDroneUID)
{
    // Read waypoints
    CXMLNode rootNode = CXMLNode::parseJSON(sLandingPlan);
    CXMLNode landingPlanNode = rootNode.getNodeByTagName(TAG_LANDING_PLAN);
    sDroneUID = landingPlanNode.attributes()[ATTR_DRONE_UID];
    readPlan(landingPlanNode, vWayPoints);
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeDroneError(const SpyCore::DroneError &eDroneError, const QString &sDroneUID)
{
    CXMLNode rootNode;
    CXMLNode errorNode(TAG_DRONE_ERROR);
    errorNode.attributes()[ATTR_DRONE_UID] = sDroneUID;
    errorNode.attributes()[ATTR_ERROR] = QString::number((int)eDroneError);
    rootNode.nodes() << errorNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deSerializeDroneError(const QString &sErrorNode, int &iErrorCode, QString &sDroneUID)
{
    CXMLNode msgNode = CXMLNode::parseJSON(sErrorNode);
    CXMLNode droneErrorNode = msgNode.getNodeByTagName(TAG_DRONE_ERROR);
    sDroneUID = droneErrorNode.attributes()[ATTR_DRONE_UID];
    iErrorCode = droneErrorNode.attributes()[ATTR_ERROR].toInt();
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeTakeOffRequest(const QString &sDroneUID)
{
    CXMLNode rootNode;
    CXMLNode failSafeNode(TAG_TAKE_OFF);
    failSafeNode.attributes()[ATTR_DRONE_UID] = sDroneUID;
    rootNode.nodes() << failSafeNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializeTakeOffRequest(const CXMLNode &takeOffRequestNode, QString &sDroneUID)
{
    sDroneUID = takeOffRequestNode.attributes()[ATTR_DRONE_UID];
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeFailSafeRequest(const QString &sDroneUID)
{
    CXMLNode rootNode;
    CXMLNode failSafeNode(TAG_FAIL_SAFE);
    failSafeNode.attributes()[ATTR_DRONE_UID] = sDroneUID;
    rootNode.nodes() << failSafeNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::serializeFailSafeDone(const QString &sDroneUID)
{
    CXMLNode rootNode;
    CXMLNode failSafeDoneNode(TAG_FAIL_SAFE_DONE);
    failSafeDoneNode.attributes()[ATTR_DRONE_UID] = sDroneUID;
    rootNode.nodes() << failSafeDoneNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::deserializeFailSafeRequest(const CXMLNode &failSafeRequestNode, QString &sDroneUID)
{
    sDroneUID = failSafeRequestNode.attributes()[ATTR_DRONE_UID];
}

//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::writeGeoPath(const QVector<Core::WayPoint> &plan, const QString &sPlanType, const QString &sDroneUID)
{
    CXMLNode rootNode;
    CXMLNode planNode(sPlanType);
    planNode.attributes()[ATTR_DRONE_UID] = sDroneUID;
    foreach (Core::WayPoint wayPoint, plan)
    {
        // Static attributes
        CXMLNode wayPointNode(TAG_WAY_POINT);
        wayPointNode.attributes()[ATTR_LATITUDE] = QString::number(wayPoint.geoCoord().latitude());
        wayPointNode.attributes()[ATTR_LONGITUDE] = QString::number(wayPoint.geoCoord().longitude());
        wayPointNode.attributes()[ATTR_ALTITUDE] = QString::number( wayPoint.geoCoord().altitude());
        wayPointNode.attributes()[ATTR_WAY_POINT_TYPE] = QString::number((int)wayPoint.type());
        wayPointNode.attributes()[ATTR_WAY_POINT_SPEED] = QString::number((int)wayPoint.speed());
        wayPointNode.attributes()[ATTR_WAY_POINT_CLOCKWISE] = QString::number((int)wayPoint.clockWise());

        // Metadata
        if (!wayPoint.metaData().isEmpty())
        {
            CXMLNode wayPointMetaDataNode(TAG_WAY_POINT_METADATA);
            for (QHash<QString, double>::const_iterator it=wayPoint.metaData().begin(); it!=wayPoint.metaData().end(); ++it)
                wayPointMetaDataNode.attributes()[it.key()] = QString::number(it.value());
            wayPointNode.nodes() << wayPointMetaDataNode;
        }
        planNode.nodes() << wayPointNode;
    }
    rootNode.nodes() << planNode;
    return rootNode;
}


//-------------------------------------------------------------------------------------------------

CXMLNode SerializeHelper::writeGeoPath(const QGeoPath &plan, const QString &sPlanType, const QString &sDroneUID)
{
    CXMLNode rootNode;
    CXMLNode planNode(sPlanType);
    planNode.attributes()[ATTR_DRONE_UID] = sDroneUID;
    for (int i=0; i<plan.size(); i++)
    {
        // Get geocoord
        QGeoCoordinate geoCoord = plan.coordinateAt(i);

        // Static attributes
        CXMLNode wayPointNode(TAG_WAY_POINT);
        wayPointNode.attributes()[ATTR_LATITUDE] = QString::number(geoCoord.latitude());
        wayPointNode.attributes()[ATTR_LONGITUDE] = QString::number(geoCoord.longitude());
        wayPointNode.attributes()[ATTR_ALTITUDE] = QString::number( geoCoord.altitude());
        planNode.nodes() << wayPointNode;
    }
    rootNode.nodes() << planNode;
    return rootNode;
}

//-------------------------------------------------------------------------------------------------

void SerializeHelper::readPlan(const CXMLNode &node, WayPointList &vWayPointList)
{
    QVector<CXMLNode> vWayPointNodes = node.getNodesByTagName(TAG_WAY_POINT);
    foreach (CXMLNode wayPointNode, vWayPointNodes)
    {
        // Static attributes
        double dLatitude = wayPointNode.attributes()[ATTR_LATITUDE].toDouble();
        double dLongitude = wayPointNode.attributes()[ATTR_LONGITUDE].toDouble();
        double dAltitude = wayPointNode.attributes()[ATTR_ALTITUDE].toDouble();
        int iType = wayPointNode.attributes()[ATTR_WAY_POINT_TYPE].toInt();
        int iSpeed = wayPointNode.attributes()[ATTR_WAY_POINT_SPEED].toInt();
        bool bClockWise = (bool)wayPointNode.attributes()[ATTR_WAY_POINT_CLOCKWISE].toInt();

        QGeoCoordinate geoCoord(dLatitude, dLongitude, dAltitude);
        Core::WayPoint wayPoint(geoCoord, (SpyCore::PointType)iType);
        wayPoint.setSpeed((SpyCore::PointSpeed)iSpeed);
        wayPoint.setClockWise(bClockWise);
        vWayPointList << wayPoint;
    }
}

//-------------------------------------------------------------------------------------------------

QGeoPath SerializeHelper::readGeoPath(const CXMLNode &node)
{
    QVector<CXMLNode> vWayPointNodes = node.getNodesByTagName(TAG_WAY_POINT);
    QGeoPath geoPath;
    foreach (CXMLNode wayPointNode, vWayPointNodes)
    {
        // Static attributes
        double dLatitude = wayPointNode.attributes()[ATTR_LATITUDE].toDouble();
        double dLongitude = wayPointNode.attributes()[ATTR_LONGITUDE].toDouble();
        double dAltitude = wayPointNode.attributes()[ATTR_ALTITUDE].toDouble();
        QGeoCoordinate geoCoord(dLatitude, dLongitude, dAltitude);
        geoPath.addCoordinate(geoCoord);
    }
    return geoPath;
}

//-------------------------------------------------------------------------------------------------

QString SerializeHelper::messageType(const QString &sMessage)
{
    CXMLNode rootNode = CXMLNode::parseJSON(sMessage);
    if (!rootNode.nodes().isEmpty())
        return rootNode.nodes().first().tag();
    return QString("");
}
