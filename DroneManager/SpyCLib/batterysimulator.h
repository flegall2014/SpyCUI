#ifndef BATTERYSIMULATOR_H
#define BATTERYSIMULATOR_H

// Qt
#include <QObject>
#include <QTimer>

// Application
#include "basesimulator.h"
#include "spyclib_global.h"

namespace Core {
class SPYCLIBSHARED_EXPORT BatterySimulator : public Core::BaseSimulator
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    BatterySimulator(QObject *pParent=nullptr);

    //! Destructor
    ~BatterySimulator();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Start
    virtual void start();

    //! Stop
    virtual void stop();

private:
    //! Battery level
    int m_iBatteryLevel = 100;

    //! Return level
    int m_iReturnLevel = 0;

public slots:
    //! Time out
    void onTimeOut();

signals:
    //! Battery level changed
    void batteryLevelChanged(int iLevel, int iReturn);
};
}

#endif // BATTERYSIMULATOR_H
