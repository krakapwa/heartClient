#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QMainWindow>
#include <QDateTime>
#include <QCloseEvent>
#include "btclient.h"
#include "ui_mainclient.h"
#include <qbluetoothserviceinfo.h>
#include <qbluetoothsocket.h>
#include <qbluetoothhostinfo.h>
#include <qbluetoothservicediscoveryagent.h>

namespace Ui {
class MainClient;
}

class MainClient : public QMainWindow
{
    Q_OBJECT

public:
    MainClient(QMainWindow *parent = 0);
    ~MainClient();
    void startDiscovery(const QBluetoothUuid &uuid);
signals:
    void startClicked();
    void connectClicked();
    void closeClient();
    void sendMessage(QString);
private slots:

    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void on_startButton_clicked();
    void toggleStartButton();
    void on_connectButton_clicked();
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
};

#endif // MAINCLIENT_H
