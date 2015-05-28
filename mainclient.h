#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QDateTime>
#include <QCloseEvent>
#include "btclient.h"
#include <qbluetoothserviceinfo.h>
#include <qbluetoothsocket.h>
#include <qbluetoothhostinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QBluetoothLocalDevice>
#include <boost/circular_buffer.hpp>
#include <QBitArray>
#include <firfilt.h>

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
    void syncClicked();
    void connectClicked();
    void closeClient();
    void sendMessage(QString);
    void appendText(QVariant text);
    void appendSamples(QVariant);
    void disableConnectButton();
    void enableConnectButton();
    void disableStartStopButton();
    void enableStartStopButton();
private slots:

    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void startStopButtonClicked();
    void syncButtonClicked();
    void connectButtonClicked();
    void toggleStartButton();
    void showMessage(const QString &sender, const QString &message);
    void toggleConnectButton();
    void clientConnected(const QString &name);
    void clientDisconnected();
    void newSamplesReceived(QByteArray baIn);

public slots:
   void processMessage(QString);

private:
    Ui::MainClient *ui;
   Btclient client ;
    void closeEvent(QCloseEvent*);
    void connectToServer();
    void noAdapters();
    QList<QBluetoothHostInfo> localAdapters;
    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo service;
    bool ok;
    QVariantMap dataToPlot;
    int tmp;
    int bufSize;
    boost::circular_buffer<int> ecg;
    boost::circular_buffer<int> bcgx;
    boost::circular_buffer<int> bcgy;
    boost::circular_buffer<int> bcgz;
    FirFilt * tapFilt;
};



#endif // MAINCLIENT_H
