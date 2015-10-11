#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T14:38:52
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Analyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    analyzer.cpp \
    vehicle.cpp \
    timestep.cpp \
    mymath.cpp \
    pedestrian.cpp \
    results.cpp \
    areawindow.cpp \
    pedestrianwindow.cpp \
    pedestriangenerator.cpp \
    cell.cpp \
    resultswindow.cpp \
    mymodel.cpp

HEADERS  += mainwindow.h \
    analyzer.h \
    vehicle.h \
    timestep.h \
    mymath.h \
    pedestrian.h \
    results.h \
    areawindow.h \
    pedestrianwindow.h \
    pedestriangenerator.h \
    cell.h \
    resultswindow.h \
    mymodel.h

FORMS    += mainwindow.ui \
    areawindow.ui \
    pedestrianwindow.ui \
    resultswindow.ui

OTHER_FILES += \
    ../../Scala/Workspace/SumoData/src/pedestrians.xml
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
macx:QMAKE_MAC_SDK=macosx10.9

