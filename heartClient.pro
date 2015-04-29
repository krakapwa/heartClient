#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T12:23:19
#
#-------------------------------------------------

QT       += core bluetooth qml quick
CONFIG += qt plugin c++11
TEMPLATE = app


SOURCES += main.cpp\
        mainclient.cpp \
    btclient.cpp \
    quickplotqmlplugin.cpp \
    qptightscaleengine.cpp \
    qpscrollingcurve.cpp \
    qpscaleengine.cpp \
    qpplotitem.cpp \
    qpfixedscaleengine.cpp \
    qpcurve.cpp

HEADERS  += mainclient.h \
    btclient.h \
    qtquickcontrolsapplication.h \
    quickplotqmlplugin.h \
    qptightscaleengine.h \
    qpscrollingcurve.h \
    qpscaleengine.h \
    qpplotitem.h \
    qpfixedscaleengine.h \
    qpcurve.h \
    boost/circular_buffer.hpp

OTHER_FILES += \
    qml/main.qml \
    qml/Axes.qml \
    qml/AxisLabels.qml \
    qml/PlotArea.qml \
    qml/Tick.qml

RESOURCES += qml.qrc

#LIBS += -L/usr/lib
