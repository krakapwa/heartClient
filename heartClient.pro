#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T12:23:19
#
#-------------------------------------------------

QT       += core bluetooth qml quick

TARGET = btclientqt
TEMPLATE = app


SOURCES += main.cpp\
        mainclient.cpp \
    btclient.cpp

HEADERS  += mainclient.h \
    btclient.h \
    qtquickcontrolsapplication.h

RESOURCES += qml.qrc

LIBS += -L/usr/lib
