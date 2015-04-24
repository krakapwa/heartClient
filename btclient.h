#ifndef BTCLIENT_H
#define BTCLIENT_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <qbluetoothserviceinfo.h>

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

signals:
    void sendMessage(QString str);
    void finished();
    void messageReceived(const QString &sender, const QString &message);
    void disconnected();

public slots:
    void readSocket();
    //void connected(const QString &name);
    void connected();
    void toggleStartStop();

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
