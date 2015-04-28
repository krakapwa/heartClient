#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QDateTime>
#include <QCloseEvent>
#include "btclient.h"
#include "ui_mainclient.h"
#include <qbluetoothserviceinfo.h>
#include <qbluetoothsocket.h>
#include <qbluetoothhostinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QBluetoothLocalDevice>

namespace Ui {
class MainClient;
}

class MainClient : public QObject
{
    Q_OBJECT

public:
    MainClient(QObject *parent = 0);
    ~MainClient();
    void startDiscovery(const QBluetoothUuid &uuid);
    void initGui();
signals:
    void startClicked();
    void connectClicked();
    void closeClient();
    void sendMessage(QString);
    void appendText(QVariant text);
    void disableConnectButton();
    void enableConnectButton();
    void disableStartStopButton();
    void enableStartStopButton();
private slots:

    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void startStopButtonClicked();
    void connectButtonClicked();
    void toggleStartButton();
    void showMessage(const QString &sender, const QString &message);
    void toggleConnectButton();
    void clientConnected(const QString &name);
    void clientDisconnected();

public slots:
   void processMessage(QString);

private:
    Ui::MainClient *ui;
   Btclient client ;
    void closeEvent(QCloseEvent*);
    void connectToServer();
    QList<QBluetoothHostInfo> localAdapters;
    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo service;
    void setHeight(QPlainTextEdit* edit, int nRows);
};

#endif // MAINCLIENT_H
