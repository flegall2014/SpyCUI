#ifndef DRONEMANAGER_H
#define DRONEMANAGER_H

// Qt
#include <QObject>
#include <QVector>
#include <QHash>
#include <QTimer>

// Application
#include <spycore.h>
namespace Core {
    class DroneEmulator;
    class TCPServer;
}

namespace Model {
class DroneManager : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    DroneManager(QObject *pParent=nullptr);

    //! Destructor
    ~DroneManager();

    //! Send message
    void sendMessage(const QString &sMessage);

private:
    //! Get drone by UID
    Core::DroneEmulator *getDrone(const QString &sDroneUID) const;

private:
    //! Drones
    QVector<Core::DroneEmulator *> m_vDrones;

    //! TCPServer
    Core::TCPServer *m_pServer = nullptr;

    //! Upload plans?
    bool m_bUploadPlans = false;

public slots:
    //! Drone time out
    void onDroneTimeOut();

    //! Take off drone
    void onTakeOffRequest(const QString &DroneUID);

    //! Fail safe
    void onFailSafeRequest(const QString &DroneUID);

    //! Mission plan changed
    void onMissionPlanChanged(const QString &sDroneUID);

    //! Safety plan changed
    void onSafetyPlanChanged(const QString &sDroneUID);

    //! Landing plan changed
    void onLandingPlanChanged(const QString &sDroneUID);

    //! Drone error
    void onDroneError(const SpyCore::DroneError &eDroneError, const QString &sDroneUID);

    //! Fail safe done
    void onFailSafeDone(const QString &sDroneUID);

    //! New connection from ground station
    void onNewConnectionFromGroundStation();

    //! Process incoming message
    void onIncomingMessage(const QString &sIcominMessage);

    //! Upload plans
    void onUploadPlans();

signals:
    //! Upload plans
    void uploadPlans();
};
}

#endif // DRONEMANAGER_H
