#include "btclient.h"
#include "mainclient.h"

static const int packSize = 33;


Btclient::Btclient(QObject *parent)
    :QThread(parent), socket(0)
{

    bufSize = 27;
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

void Btclient::deserialize(const QByteArray& line){
        //qDebug() << "Deserialized:" + line.toHex();
        bufSig = line.mid(0,2);
        //qDebug() << bufSig.toHex();
    if(bufSig == QByteArray::fromHex("AAAA")){
        if(line.size() == packSize){
            // Remove signature byte and extract bufSize bytes
            buf = line.mid(2,bufSize);
            //qDebug() << buf.toHex();
            emit sendNewSamples(buf);
        }
    }
    else if (bufSig == QByteArray::fromHex("BBBB") ){
        //qDebug() << "got message";
        buf = line.mid(2,bufSize);
        qDebug() << "Received from " + socket->peerName() + ": " + QString::fromUtf8(line.constData(), line.length());
        QString msg = QString::fromUtf8(buf.constData(), buf.length());
        msg.remove(QChar('\n'), Qt::CaseInsensitive);
        emit messageReceived(socket->peerName(),msg);
    }

}

void Btclient::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
            deserialize(line);
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
