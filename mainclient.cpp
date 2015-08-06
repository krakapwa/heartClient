#include "mainclient.h"

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c7");
//static const QLatin1String remoteAddress("00:02:72:C9:1B:25");
//static const QLatin1String remoteAddress("5C:F3:70:68:08:35");

MainClient::MainClient(QObject *parent) :
    QObject(parent)
{

    //tap filter
    tapFilt = new FirFilt(filter_taps,FILTER_TAP_NUM);

    bufSize = FILTER_TAP_NUM;
    ecg = boost::circular_buffer<int>(bufSize);
    bcgx = boost::circular_buffer<int>(bufSize);
    bcgy = boost::circular_buffer<int>(bufSize);
    bcgz = boost::circular_buffer<int>(bufSize);
    bcgx2 = boost::circular_buffer<int>(bufSize);
    bcgy2 = boost::circular_buffer<int>(bufSize);
    bcgz2 = boost::circular_buffer<int>(bufSize);

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
    std::string line("1,2,-2,");
    std::vector<int> vec;
    using namespace boost;
    tokenizer<escaped_list_separator<char> > tk(
                            line, escaped_list_separator<char>('\\', ',', '\"'));
    for (tokenizer<escaped_list_separator<char> >::iterator i(tk.begin());
         i!=tk.end();++i)
    {
        try   {
            vec.push_back(boost::lexical_cast<int>( *i ));
        }
        catch( boost::bad_lexical_cast & e ){
            //qDebug() << "Exception caught : " + QString::fromStdString(e.what());
        }
    }

}

void MainClient::noAdapters(){

    //emit appendText("No Bluetooth adapters were found. Check that Bluetooth service is active in your devices settings.");
}

void MainClient::startDiscovery(const QBluetoothUuid &uuid)
{

    //ui->connectButton->setEnabled(false);
    const QBluetoothAddress adapter;
    if(localAdapters.isEmpty()) noAdapters();
    else{

        localAdapters.at(0).address();
    }
        m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapter);
        connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

        //ui->console->appendPlainText("Scanning...\n");
        if (m_discoveryAgent->isActive())
            m_discoveryAgent->stop();

        m_discoveryAgent->setUuidFilter(uuid);
        //m_discoveryAgent->setRemoteAddress(QBluetoothAddress(remoteAddress));
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
    QObject::connect(this, SIGNAL(syncClicked()),
            client, SLOT(sendSync()));
    qDebug() << "Connecting...";

    QObject::connect(client, SIGNAL(messageReceived(QString,QString)),
            this, SLOT(showMessage(QString,QString)));
    QObject::connect(client, SIGNAL(disconnectedSig()), this, SLOT(clientDisconnected()));
    QObject::connect(client, SIGNAL(connected(QString)), this, SLOT(clientConnected(QString)));
    QObject::connect(this, SIGNAL(sendMessage(QString)), client, SLOT(sendMessage(QString)));
    QObject::connect(client, SIGNAL(sendNewSamples(std::string)),
            this, SLOT(newSamplesReceived(std::string)));
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

void MainClient::syncButtonClicked()
{
    emit syncClicked();
}
void MainClient::connectButtonClicked()
{
    startDiscovery(QBluetoothUuid(serviceUuid));
}

/*
int MainClient::buttFilt(boost::circular_buffer<int> s, myFilt filt){

    y[n] = b[0] * x[n] + b[1] * x[(n-1+m)%m] + b[2] * x[(n-2+m)%m] + b[3] * x[(n-3+m)%m]
               - a[1] * y[(n-1+m)%m] - a[2] * y[(n-2+m)%m] - a[3] * y[(n-3+m)%m];
}
*/


void MainClient::newSamplesReceived(std::string strIn){

    //qDebug() << strIn.c_str();

    std::vector<int> intIn;

    using namespace boost;
    tokenizer<escaped_list_separator<char> > tk(
                            strIn, escaped_list_separator<char>('\\', ',', '\"'));
    for (tokenizer<escaped_list_separator<char> >::iterator i(tk.begin());
         i!=tk.end();++i)
    {
        try   {
            intIn.push_back(boost::lexical_cast<int>( *i ));
        }
        catch( boost::bad_lexical_cast & e ){
            //qDebug() << "Exception caught : " + QString::fromStdString(e.what());
        }
    }


    //Calculate numerical values and apply moving average. Window size = bufSize
    QVariantMap map;
    //ECG (RA-LA)
    ecg.push_back(intIn[2]);
    map.insert("ECG", tapFilt->applyFilt(ecg));
    map.insert("ECG", intIn[2]);
    //BCGx
    bcgx.push_back(intIn[4]);
    map.insert("BCGx", tapFilt->applyFilt(bcgx));
    //BCGy
    bcgy.push_back(-intIn[3]);
    map.insert("BCGy", tapFilt->applyFilt(bcgy));
    //BCGz
    bcgz.push_back(intIn[5]);
    map.insert("BCGz", tapFilt->applyFilt(bcgz));

    //BCGx2
    bcgx2.push_back(intIn[10]);
    map.insert("BCGx2", tapFilt->applyFilt(bcgx2));
    //BCGy2
    bcgy2.push_back(-intIn[9]);
    map.insert("BCGy2", tapFilt->applyFilt(bcgy2));
    //BCGz2
    bcgz2.push_back(intIn[11]);
    map.insert("BCGz2", tapFilt->applyFilt(bcgz2));

    emit appendSamples(map);
}
