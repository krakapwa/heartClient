#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T12:23:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = btclientqt
TEMPLATE = app


SOURCES += main.cpp\
        mainclient.cpp \
    btclient.cpp

HEADERS  += mainclient.h \
    btclient.h

FORMS    += mainclient.ui

INCLUDEPATH += /usr/include
LIBS += -L/usr/lib -lpthread -lbluetooth
