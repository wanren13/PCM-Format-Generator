#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T15:26:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapGenerator
TEMPLATE = app

include(xlsx/qtxlsx.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp \
    wizard.cpp \
    map.cpp \
    sensor.cpp \
    printer.cpp \
    generator.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    parser.h \
    wizard.h \
    map.h \
    sensor.h \
    printer.h \
    generator.h \
    helpdialog.h \
    checkerSensor.h

FORMS    += mainwindow.ui \
    wizard.ui \
    helpdialog.ui

RESOURCES += \
    Resources.qrc

#QMAKE_LFLAGS += \INCREMENTAL:NO
