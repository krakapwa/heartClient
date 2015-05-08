#include "mainclient.h"

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c7");
static const QLatin1String remoteAddress("00:02:72:C9:1B:25");

MainClient::MainClient(QObject *parent) :
    QObject(parent)
{
    bufSize = 5;
    ecg = boost::circular_buffer<int>(bufSize);
    bcgx = boost::circular_buffer<int>(bufSize);
    bcgy = boost::circular_buffer<int>(bufSize);
    bcgz = boost::circular_buffer<int>(bufSize);

    //fill buffers with zeros
    /*
    for(int i=0; i<bufSize;++i){
        ecg.push_back(0);
        bcgx.push_back(0);
        bcgy.push_back(0);
        bcgz.push_back(0);
    }
    */

}

int getSign(QByteArray q){

    QBitArray leftbit(1);
    QByteArray leftbyte = q.left(1);

    //qDebug() << "bits";

    leftbit.setBit(0,leftbyte.at(0)&(1<<7));
    //qDebug() << "bit " + QString::number(leftbit.testBit(0));

   if(leftbit.testBit(0)) return -1;
   else return 1;

}

void MainClient::initGui(){
    qDebug() << "initGui";
    emit disableStartStopButton();
    emit appendText("Click connect to start!");

    /*
    unsigned char tmp = 0xFFFFFF;
    int t;
    bool ok;
    QByteArray q;
    QByteArray MSB;

    q = QByteArray::fromHex("7FFFFF");
    t = q.toHex().toUInt(&ok,16)*getSign(q);
    qDebug() << "t= " + QString::number(t);

    q= QByteArray::fromHex("000000");
    t = q.toHex().toUInt(&ok,16)*getSign(q);
    qDebug() << "t= " + QString::number(t);

    q= QByteArray::fromHex("800000");
    t = q.toHex().toUInt(&ok,16)*getSign(q);
    qDebug() << "t= " + QString::number(t);
    */
}

void MainClient::startDiscovery(const QBluetoothUuid &uuid)
{

    //ui->connectButton->setEnabled(false);
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(0).address();
    m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapter);
    connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

    //ui->console->appendPlainText("Scanning...\n");
    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    m_discoveryAgent->setUuidFilter(uuid);
    m_discoveryAgent->setRemoteAddress(QBluetoothAddress(remoteAddress));
    emit appendText("Scanning...");
    m_discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);

}

void MainClient::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();

    service = serviceInfo;

    qDebug() << "Connecting to service" << service.serviceName()
             << "on" << service.device().name();

    // Create client
    Btclient* client = new Btclient();
    QObject::connect(this, SIGNAL(startClicked()),
            client, SLOT(toggleStartStop()));
    qDebug() << "Connecting...";

    QObject::connect(client, SIGNAL(messageReceived(QString,QString)),
            this, SLOT(showMessage(QString,QString)));
    QObject::connect(client, SIGNAL(disconnectedSig()), this, SLOT(clientDisconnected()));
    QObject::connect(client, SIGNAL(connected(QString)), this, SLOT(clientConnected(QString)));
    QObject::connect(this, SIGNAL(sendMessage(QString)), client, SLOT(sendMessage(QString)));
    QObject::connect(client, SIGNAL(sendNewSamples(QByteArray)),
            this, SLOT(newSamplesReceived(QByteArray)));
    client->startClient(service);
    emit disableConnectButton();
    emit enableStartStopButton();
}

void MainClient::showMessage(const QString &sender, const QString &message)
{
    emit appendText(QString::fromLatin1("%1: %2").arg(sender, message));
    //ui->console->ensureCursorVisible();
}

MainClient::~MainClient()
{
    delete ui;
}


void MainClient::toggleConnectButton()
{
}

void MainClient::toggleStartButton()
{
}

void MainClient::processMessage(QString str){
    QDateTime current = QDateTime::currentDateTime();

    emit appendText("["+current.toString()+"] "+str);
}


void MainClient::closeEvent(QCloseEvent *event)
{
        emit closeClient();
        event->accept();
}

void MainClient::clientDisconnected()
{
    Btclient *client = qobject_cast<Btclient *>(sender());
    client->stopClient();
    if (client) {
        client->deleteLater();
    }
    emit appendText("Disconnected.");
    emit enableConnectButton();
    emit disableStartStopButton();
}

void MainClient::clientConnected(const QString &name)
{
    emit appendText(QString::fromLatin1("Connected to %1 .").arg(name));
    //ui->console->insertPlainText(QString::fromLatin1("Connected to %1 .\n").arg(name));
}

void MainClient::startStopButtonClicked()
{
    emit startClicked();
}

void MainClient::connectButtonClicked()
{
    startDiscovery(QBluetoothUuid(serviceUuid));
}

void MainClient::newSamplesReceived(QByteArray baIn){
    //Calculate numerical values and apply moving average. Window size = bufSize
    QVariantMap map;
    int s;
    QByteArray q;
    //Header is first set of 3 bytes
    //ECG (RA-LA)
    q =  baIn.mid(6,3);
    s =  q.toHex().toUInt(&ok,16);
    s = s*getSign(q);
    ecg.push_back(s);
    map.insert("ECG", std::accumulate(ecg.begin(), ecg.end(), 0)/bufSize);
    //map.insert("ECG", s);
    //BCGx
    q =  baIn.mid(12,3);
    s =  q.toHex().toUInt(&ok,16);
    s = s*getSign(q);
    bcgx.push_back(s);
    map.insert("BCGx", std::accumulate(bcgx.begin(), bcgx.end(), 0)/bufSize);
    //map.insert("BCGx", s);
    //BCGy
    q =  baIn.mid(15,3);
    s =  q.toHex().toUInt(&ok,16);
    s = s*getSign(q);
    bcgy.push_back(s);
    map.insert("BCGy", std::accumulate(bcgy.begin(), bcgy.end(), 0)/bufSize);
    //map.insert("BCGy", s);
    //BCGz
    q =  baIn.mid(18,3);
    s =  q.toHex().toUInt(&ok,16);
    s = s*getSign(q);
    bcgz.push_back(s);
    map.insert("BCGz", std::accumulate(bcgz.begin(), bcgz.end(), 0)/bufSize);
    //map.insert("BCGz", s);

    emit appendSamples(map);
}
