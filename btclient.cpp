#include "btclient.h"
#include "mainclient.h"

Btclient::Btclient(QObject *parent)
    :QThread(parent), quit(false)
{
    addr = { 0 };
    dest = "00:02:72:C9:1B:25";
    tFormat = "%d-%m-%y_%H-%M-%S";
    msgSize = 256;
    beatPeriod = 2; //seconds
    timeoutSecs = 1;
    servBeatResponse = "Server waiting";
    servAcceptedConnectionResponse= "accepted connection";
    event = false;
    started = false;
    quit = false;
    waitStr = "wait";
}

Btclient::~Btclient()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void Btclient::run(){
    mutex.lock();
    mutex.unlock();
    bool val;
   val = connectToHost();
    if(!val) return;

   mutex.lock();
   cond.wait(&mutex);
   mutex.unlock();

}

void Btclient::setTimeout(int seconds){

    //Set timeout on blocking read
    struct timeval tv;
    tv.tv_sec = seconds;  /* 15 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

}
void Btclient::startClient(){

    if (!isRunning())
        start();
    else
        cond.wakeOne();
}

bool Btclient::connectToHost(){

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );
    status = ::connect(s, (struct sockaddr *)&addr, sizeof(addr));
    QString str;
    char buffer[msgSize];
    if(status==0){
        setTimeout(timeoutSecs);
        str = readSocket(buffer,msgSize);
        emit sendMessage(str);
        connected = true;
        emit toggleConnected();
        beat();
    }
    else{
        close(s);
        emit sendMessage("Server can't be reached");
        toggleConnected();
        return false;
    }
}

QString Btclient::readSocket(char* buffer, int buffer_size)
{
    bzero(buffer,buffer_size);

    const int recvout = recv(s, buffer, buffer_size, 0);
    if(recvout == -1 || recvout == 0)
    {
        //qDebug() << "Error: " << errno;
        qDebug() << "Disconnected";
        connected = false;
        emit toggleConnected();
        return QString::fromUtf8(buffer);
    }
    else if(QString::fromLatin1(buffer)!= waitStr){
        qDebug() << "reading [" << QString::fromLatin1(buffer) << "]";
        return QString::fromUtf8(buffer);
    }
}

QString Btclient::writeReadStr(const QString &qstr, int  writetoreaddelay){
    QString strOut;
    int recvOut=-1;
    QByteArray byteArray = qstr.toUtf8();
    const char* bufout = byteArray.constData();
    char buffer[msgSize];

    //qDebug() << "Sending: " <<QString::fromUtf8(bufout);
    write(s, bufout, msgSize);
    sleep(writetoreaddelay);
        //recvOut = recv(s,msg, msgSize,MSG_WAITALL);
    strOut = readSocket(buffer,msgSize);

    if(qstr != waitStr)
        emit sendMessage("Server: "+QString::fromUtf8(buffer));

    return QString::fromUtf8(buffer);

}

void Btclient::beat(){
    QString servResponse;
    bool servGotBeat = true;

    while(servGotBeat && !quit && !event){

        servResponse=writeReadStr(waitStr,beatPeriod);
        if(servResponse != servBeatResponse){
            servGotBeat = false;
           //emit sendMessage("Connection timeout. Please reconnect");
        }
        else{

            //emit sendMessage("Server: "+servResponse);
        }
    }
    if(event && !started)
        startContinuousDaq();
    if(event && started)
        stopContinuousDaq();
}

int Btclient::secsToNbeats(int nsec){
    // Return a minimum of 1 beat
  int nbeats = nsec/beatPeriod;
  if(nbeats == 0)
      return 1;
  else
      return nbeats;
}

void Btclient::sendBeats(int nsec){
    QString servResponse;
    bool servGotBeat = true;
    int beats = 0;
    int nbeats = secsToNbeats(nsec);

    while(servGotBeat && beats<=nbeats){

        servResponse=writeReadStr(waitStr,beatPeriod);
        if(servResponse != servBeatResponse){
            servGotBeat = false;
           emit sendMessage("Connection timeout. Please reconnect");
        }
        else{

            emit sendMessage("Server: "+servResponse);
        }
        ++beats;
    }
}

void Btclient::singleShot(int nsec){

    if( status == 0 ) {
    writeReadStr("clock",1);
    time(&thisTime);
    info = localtime(&thisTime);
    strftime(timeBuffer,80,tFormat,info);
    //qDebug() << "Formatted date & time : " + QString::fromUtf8(timeBuffer);
    writeReadStr(QString::fromUtf8(timeBuffer),0 );

    writeReadStr("start",0);
    writeReadStr("gimme filename",0);

       sendBeats(nsec);
    //sleep(nsec);
    writeReadStr("stop",0);
    //writeReadStr("kill",0);

    event = false;
    }

    if( status < 0 )
        emit sendMessage("Not connected, click connect first.");


}

void Btclient::startContinuousDaq(){

    if( status == 0 ) {
    writeReadStr("clock",1);
    time(&thisTime);
    info = localtime(&thisTime);
    strftime(timeBuffer,80,tFormat,info);
    //qDebug() << "Formatted date & time : " + QString::fromUtf8(timeBuffer);
    writeReadStr(QString::fromUtf8(timeBuffer),0 );

    writeReadStr("start",0);
    writeReadStr("gimme filename",0);

    event = false;
    started = true;
    beat();

    }

    if( status < 0 )
        emit sendMessage("Not connected, click connect first.");
}

void Btclient::stopContinuousDaq(){

    if( status == 0 ) {
    writeReadStr("stop",1);

    event = false;
    started = false;
    beat();

    }

    if( status < 0 )
        emit sendMessage("Not connected, click connect first.");
}

void Btclient::toggleEvent(){
    if(event==true) event = false;
    else {
        event = true;
    }
    emit sendMessage("event toggled");
}

void Btclient::setQuitTrue(){
    quit = true;
    terminate();
}
