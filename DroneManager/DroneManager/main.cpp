// Qt
#include <QApplication>
#include <QSettings>

// Application
#include "dronemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new Model::DroneManager(nullptr);
    return a.exec();
}
