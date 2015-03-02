#ifndef BTCLIENT_H
#define BTCLIENT_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <time.h>

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
    bool connectToHost();
    void startClient();
    void beat();
    void sendBeats(int nsec);

signals:
    void sendMessage(QString str);
    void closeClient();
    void finished();
    void toggleConnected();
    void toggleStarted();

public slots:
    void toggleEvent();
    void setQuitTrue();

protected:
    //void run();

private:

    QMutex mutex;
    QWaitCondition cond;
    bool quit;
    QString writeReadStr(const QString&, int writetoreaddelay);
    QString readSocket(char* buffer, int buffer_size);
    int secsToNbeats(int nsec);
    void setTimeout(int seconds);
    struct sockaddr_rc addr;
    int s, status;
     char* dest;
    time_t thisTime;
    struct tm *info;
    char* tFormat;
    char timeBuffer[80];
    int msgSize;
    int beatPeriod;
    int timeoutSecs;
    bool event;
    QString servBeatResponse;
    QString servAcceptedConnectionResponse;
    QString waitStr;
    QString servWaitStr;

    bool connected;
    bool started;
};

#endif // BTCLIENT_H
