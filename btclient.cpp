#include "btclient.h"
#include "mainclient.h"

Btclient::Btclient(QObject *parent)
    :QThread(parent), socket(0)
{

    //QObject::connect(socket, SIGNAL(connected()), this, SLOT(readSocket()));

}

Btclient::~Btclient()
{
    mutex.lock();
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void Btclient::run(){
    mutex.lock();
    mutex.unlock();
    bool val;
    if(!val) return;

   mutex.lock();
   cond.wait(&mutex);
   mutex.unlock();

}

void Btclient::startClient(const QBluetoothServiceInfo &remoteService){

    qDebug() << "Start client";
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

}

void Btclient::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }

}

void Btclient::toggleStartStop(){
    qDebug() << "toggleStartStop";
    sendMessage("start");
}

void Btclient::connected()
{
    emit connected(socket->peerName());
}

void Btclient::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    qDebug() << "sendMessage";
    socket->write(text);
}
