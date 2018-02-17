#-------------------------------------------------
#
# Project created by QtCreator 2017-07-20T13:59:53
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gps
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    port.cpp

HEADERS  += mainwindow.h \
    port.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x
