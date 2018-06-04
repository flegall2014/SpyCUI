#ifndef BASESIMULATOR_H
#define BASESIMULATOR_H

// Qt
#include <QObject>
#include <QTimer>

// Application
#include "spyclib_global.h"

namespace Core {
class SPYCLIBSHARED_EXPORT BaseSimulator : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit BaseSimulator(QObject *pParent=nullptr) : QObject(pParent) {}

    //! Destructor
    virtual ~BaseSimulator() {}

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Start
    virtual void start() = 0;

    //! Stop
    virtual void stop() = 0;

protected:
    //! Timer
    QTimer m_timer;

public slots:
    //! Time out
    virtual void onTimeOut() {}
};
}

#endif // BASESIMULATOR_H
