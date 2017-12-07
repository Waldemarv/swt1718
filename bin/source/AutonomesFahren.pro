#-------------------------------------------------
#
# Project created by QtCreator 2017-10-29T19:53:32
#
#-------------------------------------------------

QT       += core gui xml
CONFIG   += c++11 console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutonomesFahren
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        editor.cpp \
        map.cpp \
        tile.cpp \
        obstacle.cpp \
        straight.cpp \
        turn.cpp \
    intersection.cpp \
    tintersection.cpp \
    startingtile.cpp \
    endingtile.cpp \
    smartvehicle.cpp \
    simulatorwindow.cpp \
    memento.cpp \
    caretaker.cpp \
    sensor.cpp \
    simulatorcmdl.cpp

HEADERS += \
        editor.h \
        map.h \
        point.h \
        tile.h \
        obstacle.h \
        straight.h \
        turn.h \
    intersection.h \
    tintersection.h \
    startingtile.h \
    endingtile.h \
    smartvehicle.h \
    simulatorwindow.h \
    memento.h \
    caretaker.h \
    sensor.h \
    simulatorcmdl.h

FORMS += \
        editor.ui \
    simulatorwindow.ui

RESOURCES += \
        icons.qrc
