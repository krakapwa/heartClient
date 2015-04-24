#ifndef BTCLIENT_H
#define BTCLIENT_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <qbluetoothserviceinfo.h>
#include <QDateTime>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class Btclient: public QThread
{
    Q_OBJECT
public:
    Btclient(QObject *parent = 0);
    ~Btclient();
    void run();
    void singleShot(int nsec);
    void startContinuousDaq();
    void stopContinuousDaq();
    void startClient(const QBluetoothServiceInfo &remoteService);
    void stopClient();

signals:
    void finished();
    void messageReceived(const QString &sender, const QString &message);
    void connected(const QString &name);
    void disconnectedSig();

public slots:
    void disconnected();
    void connected();
    void readSocket();
    void toggleStartStop();
    void sendMessage(const QString &message);

protected:
    //void run();

private:

    QMutex mutex;
    QWaitCondition cond;
    int msgSize;
    int beatPeriod;
    int timeoutSecs;
    bool event;
    QBluetoothSocket *socket;

};

#endif // BTCLIENT_H
