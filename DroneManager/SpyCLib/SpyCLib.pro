#-------------------------------------------------
#
# Project created by QtCreator 2018-04-07T19:38:20
#
#-------------------------------------------------

QT += gui positioning xml network

TARGET = DroneManager
TEMPLATE = lib
INCLUDEPATH == $$PWD/../SpyCLib

DEFINES += SPYCLIB_LIBRARY

CONFIG(debug, debug|release) {
    TARGET = spyclibd
} else {
    TARGET = spyclib
}

unix {
    DESTDIR = ../bin
    MOC_DIR = ../moc
    OBJECTS_DIR = ../obj
}

win32 {
    DESTDIR = ..\\bin
    MOC_DIR = ..\\moc
    OBJECTS_DIR = ..\\obj
}

unix {
    QMAKE_CLEAN *= $$DESTDIR/*$$TARGET*
    QMAKE_CLEAN *= $$MOC_DIR/*moc_*
    QMAKE_CLEAN *= $$OBJECTS_DIR/*.o*
}

win32 {
    QMAKE_CLEAN *= $$DESTDIR\\*$$TARGET*
    QMAKE_CLEAN *= $$MOC_DIR\\*moc_*
    QMAKE_CLEAN *= $$OBJECTS_DIR\\*.o*
}

HEADERS += \
    spyclib_global.h \
    spycore.h \
    cxmlnode.h \
    serializehelper.h \
    waypoint.h \
    droneemulator.h \
    basesimulator.h \
    batterysimulator.h \
    flightsimulator.h \
    spyclib_global.h \
    tcpserver.h \
    tcpclient.h \
    defs.h

SOURCES += \
    spycore.cpp \
    cxmlnode.cpp \
    serializehelper.cpp \
    waypoint.cpp \
    droneemulator.cpp \
    batterysimulator.cpp \
    flightsimulator.cpp \
    tcpserver.cpp \
    tcpclient.cpp
