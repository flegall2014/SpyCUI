// Qt
#include <QTime>
#include <QDebug>

// Application
#include "dronemanager.h"
#include "droneemulator.h"
#include "waypoint.h"
#include <defs.h>
#include <cxmlnode.h>
#include "serializehelper.h"
#include <tcpserver.h>
using namespace Model;

//-------------------------------------------------------------------------------------------------

DroneManager::DroneManager(QObject *pParent) : QObject(pParent)
{
    // Build server
    m_pServer = new Core::TCPServer(this);
    connect(m_pServer, &Core::TCPServer::dataReady, this, &DroneManager::onIncomingMessage, Qt::DirectConnection);

    // Video url
    QStringList lVideos;
    lVideos << "file:///d:/projects/SpyC/SpyCProject/video/video1.mp4" <<
        "file:///d:/projects/SpyC/SpyCProject/video/video2.mp4" <<
        "file:///d:/projects/SpyC/SpyCProject/video/video3.mp4";

    // Initial position
    QVector<double> vLatitudes = QVector<double>() << 48.856614 << 40.7127753 << 9.641185499999999;
    QVector<double> vLongitudes = QVector<double>() << 2.3522219000000177 << -74.0059728 << -13.57840120000003;

    // Create enw drone
    for (int i=0; i<3; i++)
    {
        // Set initial position
        double dLatitude = vLatitudes[i];
        double dLongitude = vLongitudes[i];
        QGeoCoordinate initialPos(dLatitude, dLongitude);

        // Build drone emulators
        QString sDroneUID = QString("DRONE %1").arg(i);
        Core::DroneEmulator *pDroneEmulator = new Core::DroneEmulator(sDroneUID, lVideos[i], initialPos, this);
        connect(pDroneEmulator, &Core::DroneEmulator::timeOut, this, &DroneManager::onDroneTimeOut, Qt::QueuedConnection);
        connect(pDroneEmulator, &Core::DroneEmulator::droneError, this, &DroneManager::onDroneError, Qt::QueuedConnection);
        m_vDrones << pDroneEmulator;
    }
}

//-------------------------------------------------------------------------------------------------

DroneManager::~DroneManager()
{

}

//-------------------------------------------------------------------------------------------------

void DroneManager::sendMessage(const QString &sMessage)
{
    if (m_pServer != nullptr)
        m_pServer->sendMessage(sMessage);
}

//-------------------------------------------------------------------------------------------------

Core::DroneEmulator *DroneManager::getDrone(const QString &sDroneUID) const
{
    foreach (Core::DroneEmulator *pDroneEmulator, m_vDrones)
        if (pDroneEmulator->uid() == sDroneUID)
            return pDroneEmulator;
    return nullptr;
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onDroneTimeOut()
{
    Core::DroneEmulator *pSender = dynamic_cast<Core::DroneEmulator *>(sender());
    if (pSender != nullptr)
    {
        QString sCurrentDroneStatus = pSender->currentStatus();
        sendMessage(sCurrentDroneStatus);
    }
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onTakeOffRequest(const QString &sDroneUID)
{
    Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
    if (pTargetDrone != nullptr)
        pTargetDrone->takeOff();
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onFailSafeRequest(const QString &sDroneUID)
{
    Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
    if (pTargetDrone != nullptr)
        pTargetDrone->failSafe();
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onMissionPlanChanged(const QString &sDroneUID)
{
    Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
    if (pTargetDrone != nullptr)
        sendMessage(pTargetDrone->currentStatus());
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onSafetyPlanChanged(const QString &sDroneUID)
{
    Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
    if (pTargetDrone != nullptr)
        sendMessage(pTargetDrone->currentStatus());
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onLandingPlanChanged(const QString &sDroneUID)
{
    Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
    if (pTargetDrone != nullptr)
        sendMessage(pTargetDrone->currentStatus());
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onDroneError(const SpyCore::DroneError &eDroneError, const QString &sDroneUID)
{
    sendMessage(Core::SerializeHelper::serializeDroneError(eDroneError, sDroneUID).toString());
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onFailSafeDone(const QString &sDroneUID)
{
    sendMessage(Core::SerializeHelper::serializeFailSafeDone(sDroneUID).toString());
}

//-------------------------------------------------------------------------------------------------

void DroneManager::onIncomingMessage(const QString &sIcominMessage)
{    
    qDebug() << "DroneManager::onIncomingMessage " << sIcominMessage;

    // Retrieve message type
    QString sMessageType = Core::SerializeHelper::messageType(sIcominMessage);

    // Parse
    Core::CXMLNode msgNode = Core::CXMLNode::parseJSON(sIcominMessage);

    // Safety plan
    if (sMessageType == TAG_SAFETY_PLAN)
    {
        QString sDroneUID;
        QGeoPath geoPath;
        Core::SerializeHelper::deserializeSafetyPlan(sIcominMessage, geoPath, sDroneUID);

        // Retrieve target drone
        Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
        if (pTargetDrone != nullptr)
        {
            // Set safety plan
            pTargetDrone->setSafetyPlan(geoPath);

            // Notify back client
            sendMessage(Core::SerializeHelper::serializeSafetyPlan(geoPath, sDroneUID).toJsonString());
        }
    }
    else
    // Mission plan
    if (sMessageType == TAG_MISSION_PLAN)
    {
        QString sDroneUID;
        WayPointList vWayPointList;
        Core::SerializeHelper::deserializeMissionPlan(sIcominMessage, vWayPointList, sDroneUID);

        // Retrieve target drone
        Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
        if (pTargetDrone != nullptr)
        {
            // Set mission plan
            pTargetDrone->setMissionPlan(vWayPointList);

            // Notify back client
            sendMessage(Core::SerializeHelper::serializeMissionPlan(vWayPointList, sDroneUID).toJsonString());
        }
    }
    else
    // Landing plan
    if (sMessageType == TAG_LANDING_PLAN)
    {
        QString sDroneUID;
        WayPointList vWayPointList;
        Core::SerializeHelper::deserializeLandingPlan(sIcominMessage, vWayPointList, sDroneUID);

        // Retrieve target drone
        Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
        if (pTargetDrone != nullptr)
        {
            // Set landing plan
            pTargetDrone->setLandingPlan(vWayPointList);

            // Notify back client
            sendMessage(Core::SerializeHelper::serializeLandingPlan(vWayPointList, sDroneUID).toJsonString());
        }
    }
    else
    // Take off
    if (sMessageType == TAG_TAKE_OFF)
    {
        // Retrieve take off node
        Core::CXMLNode takeOffNode = msgNode.getNodeByTagName(TAG_TAKE_OFF);

        // Deserialize
        QString sDroneUID;
        Core::SerializeHelper::deserializeTakeOffRequest(takeOffNode, sDroneUID);
        Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
        qDebug() << "NEED TAKE OFF FOR " << sDroneUID << pTargetDrone;
        if (pTargetDrone != nullptr)
            pTargetDrone->takeOff();
    }
    else
    // Fail safe
    if (sMessageType == TAG_FAIL_SAFE)
    {
        // Retrieve fail safe node
        Core::CXMLNode failSafeNode = msgNode.getNodeByTagName(TAG_FAIL_SAFE);

        // Deserialize
        QString sDroneUID;
        Core::SerializeHelper::deserializeTakeOffRequest(failSafeNode, sDroneUID);
        Core::DroneEmulator *pTargetDrone = getDrone(sDroneUID);
        if (pTargetDrone != nullptr)
            pTargetDrone->failSafe();
    }
}
