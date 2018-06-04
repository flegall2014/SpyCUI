#ifndef SPYCORE_H
#define SPYCORE_H

// Qt
#include <QObject>

// Application
#include "spyclib_global.h"

class SPYCLIBSHARED_EXPORT SpyCore : public QObject
{
    Q_OBJECT
    Q_ENUMS(FlightStatus)
    Q_ENUMS(DroneError)
    Q_ENUMS(PointType)
    Q_ENUMS(PointSpeed)
    Q_ENUMS(AlertAttribues)
    Q_ENUMS(AlertType)
    Q_ENUMS(Status)
    Q_ENUMS(WorkMode)
    Q_ENUMS(ExclusionShape)
    Q_ENUMS(DroneRole)
    Q_ENUMS(ShapeRole)
    Q_ENUMS(GalleryRole)
    Q_ENUMS(MissionPlanError)
    Q_ENUMS(UIOrientation)
    Q_ENUMS(Language)
    Q_ENUMS(MsgType)
    Q_ENUMS(DialogType)
    Q_ENUMS(WayPointRole)
    Q_ENUMS(SettingType)

public:
    //! Constructor
    SpyCore(QObject *pParent=nullptr);

    //! Destructor
    ~SpyCore();

    //! Fly status
    enum FlightStatus {IDLE=Qt::UserRole+1, FLYING};

    //! Drone error
    enum DroneError {NO_SAFETY=Qt::UserRole+1, NO_LANDING_PLAN, NO_MISSION_PLAN};

    //! Point type
    enum PointType {POINT=Qt::UserRole+1, LOITER, EIGHT, HIPPODROM};

    //! Point speed
    enum PointSpeed {ECO=Qt::UserRole+1, OBSERVATION, FAST};

    //! Alert attributes
    enum AlertAttribues {AlertIdentification=Qt::UserRole+1, AlertLevel, AlertMsg, AlertDate};

    //! Alert type
    enum AlertType {NO_ALERT=Qt::UserRole+1, BATTERY, POSITION};

    //! Define a global status
    enum Status {NOMINAL=Qt::UserRole+1, WARNING, CRITICAL};

    //! Define a drone mode
    enum WorkMode {MISSION_PLAN_EDIT=Qt::UserRole+1, SAFETY_EDIT, LANDING_PLAN_EDIT, CARTO_EDIT, PAYLOAD_EDIT, GALLERY_EDIT, EXCLUSION_EDIT};

    //! Exclusion shape
    enum ExclusionShape {CIRCLE=Qt::UserRole+1, RECTANGLE, TRIANGLE};

    //! Drone role
    enum DroneRole {Drone=Qt::UserRole+1};

    //! Shape role
    enum ShapeRole {Shape=Qt::UserRole+1};

    //! Gallery role
    enum GalleryRole {FileNameRole=Qt::UserRole+1, FilePathRole, PositionRole};

    //! Mission plan error
    enum MissionPlanError {EMPTY_SAFETY=Qt::UserRole+1, NOT_ENOUGH_POINTS_IN_SAFETY,
                           EMPTY_MISSION_PLAN, NOT_ENOUGH_POINTS_IN_MISSION_PLAN, EMPTY_EXCLUSION_AREA,
                           EMPTY_LANDING_PLAN, UNEXPECTED_LANDING_PLAN_COUNT};

    //! Setting type
    enum SettingType {ARMY=Qt::UserRole+1, UNIT, MISSION, OPERATOR, MAP_PATH, MISSION_PATH, LOG_PATH, ALERT_PATH, GALLERY_PATH, LANGUAGE_STRING, HAND};

    //! UI orientation
    enum UIOrientation {RIGHT_HAND=Qt::UserRole+1, LEFT_HAND};

    //! Languages
    enum Language {FRENCH=Qt::UserRole+1, ENGLISH, ARABIC};

    //! Message type
    enum MsgType {INFORMATION_MSG=Qt::UserRole+1, WARNING_MSG, CRITICAL_MSG};

    //! Dialog type
    enum DialogType {CONFIRM_TAKE_OFF=Qt::UserRole+1, CONFIRM_FAILSAFE, EMPTY_MISSION_PLAN_ERROR,
                     EMPTY_SAFETY_ERROR, MISSION_PLAN_VALIDATION, NOT_ENOUGH_POINT_IN_MISSION_PLAN,
                     NOT_ENOUGH_POINT_IN_SAFETY, SAFETY_VALIDATION, NO_EXCLUSION_AREA_ERROR,
                     EXCLUSION_AREA_VALIDATION, CONFIRM_REMOVE_CAPTURE, CONFIRM_CLEAR_GALLERY, EMPTY_LANDING_PLAN_ERROR,
                     LANDING_PLAN_VALIDATION, UNEXPECTED_LANDING_PLAN_COUNT_ERROR, EXIT_SPYC};

    //! Waypoint role
    enum WayPointRole {WayPointIndex=Qt::UserRole+1, WayPointCoordinate, WayPointLatitude, WayPointLongitude, WayPointAltitude, WayPointType, WayPointSpeed};
};

Q_DECLARE_METATYPE(SpyCore::FlightStatus)
Q_DECLARE_METATYPE(SpyCore::DroneError)
Q_DECLARE_METATYPE(SpyCore::PointType)
Q_DECLARE_METATYPE(SpyCore::PointSpeed)
Q_DECLARE_METATYPE(SpyCore::AlertAttribues)
Q_DECLARE_METATYPE(SpyCore::AlertType)
Q_DECLARE_METATYPE(SpyCore::Status)
Q_DECLARE_METATYPE(SpyCore::WorkMode)
Q_DECLARE_METATYPE(SpyCore::DroneRole)
Q_DECLARE_METATYPE(SpyCore::ShapeRole)
Q_DECLARE_METATYPE(SpyCore::GalleryRole)
Q_DECLARE_METATYPE(SpyCore::MissionPlanError)
Q_DECLARE_METATYPE(SpyCore::UIOrientation)
Q_DECLARE_METATYPE(SpyCore::Language)
Q_DECLARE_METATYPE(SpyCore::MsgType)
Q_DECLARE_METATYPE(SpyCore::DialogType)
Q_DECLARE_METATYPE(SpyCore::WayPointRole)
Q_DECLARE_METATYPE(SpyCore::SettingType)
Q_DECLARE_METATYPE(SpyCore::ExclusionShape)

#endif // SPYCORE_H
