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
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Btclient::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        qDebug() << "Received from " + socket->peerName() + ": " + QString::fromUtf8(line.constData(), line.length());
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}

void Btclient::toggleStartStop(){
    QDateTime currentTime = QDateTime::currentDateTime();
    qDebug() << currentTime;
    sendMessage("startStop " + currentTime.toString("dd-MM-yyyy_hh-mm-ss"));
}

void Btclient::connected()
{
    emit connected(socket->peerName());
}


void Btclient::disconnected()
{
    emit disconnectedSig();
}

void Btclient::stopClient()
{
    delete socket;
    socket = 0;
    //emit disconnected();
}

void Btclient::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}
