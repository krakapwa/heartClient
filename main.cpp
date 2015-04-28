#include "mainclient.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    MainClient mc;

    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    // connect our QML signal to our C++ slot
    QObject::connect(window, SIGNAL(connectClicked()),
                         &mc, SLOT(connectButtonClicked()));
    QObject::connect(window, SIGNAL(startStopClicked()),
                         &mc, SLOT(startStopButtonClicked()));
    // connect our C++ signal to our QML slot
    // NOTE: if we want to pass an parameter to our QML slot, it has to be
    // a QVariant.
    QObject::connect(&mc, SIGNAL(appendText(QVariant)),
                         window, SLOT(appendText(QVariant)));
    QObject::connect(&mc, SIGNAL(disableConnectButton()),
                         window, SLOT(disableConnectButton()));
    QObject::connect(&mc, SIGNAL(enableConnectButton()),
                         window, SLOT(enableConnectButton()));
    QObject::connect(&mc, SIGNAL(disableStartStopButton()),
                         window, SLOT(disableStartStopButton()));
    QObject::connect(&mc, SIGNAL(enableStartStopButton()),
                         window, SLOT(enableStartStopButton()));

    mc.initGui();
    //w.show();
    return app.exec();
}
