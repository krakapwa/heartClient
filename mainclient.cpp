#include "mainclient.h"
#include "ui_mainclient.h"

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c7");
static const QLatin1String remoteAddress("00:02:72:C9:1B:25");

MainClient::MainClient(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClient)

{
    ui->setupUi(this);

   ui->console->append("Welcome");
   ui->startButton->setEnabled(false);
    //btclient = new Btclient*;
}

void MainClient::startDiscovery(const QBluetoothUuid &uuid)
{

    ui->connectButton->setEnabled(false);

    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(0).address();
    m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapter);
    connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

    ui->console->setText(tr("Scanning..."));
    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    m_discoveryAgent->setUuidFilter(uuid);
    m_discoveryAgent->setRemoteAddress(QBluetoothAddress(remoteAddress));
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
    QObject::connect(client, SIGNAL(sendMessage(QString)),
            this, SLOT(processMessage(QString)));
    QObject::connect(this, SIGNAL(startClicked()),
            client, SLOT(toggleStartStop()));
    qDebug() << "Connecting...";

    QObject::connect(client, SIGNAL(messageReceived(QString,QString)),
            this, SLOT(showMessage(QString,QString)));
    QObject::connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    QObject::connect(client, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
    QObject::connect(this, SIGNAL(sendMessage(QString)), client, SLOT(sendMessage(QString)));
    qDebug() << "Start client";
    client->startClient(service);


}

void MainClient::showMessage(const QString &sender, const QString &message)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1: %2\n").arg(sender, message));
    ui->chat->ensureCursorVisible();
}

void MainClient::connectToServer()
{

    // scan for services

    startDiscovery(QBluetoothUuid(serviceUuid));

        // Create client
        qDebug() << "Going to create client";
        Btclient *client = new Btclient(this);
qDebug() << "Connecting...";

        connect(client, SIGNAL(messageReceived(QString,QString)),
                this, SLOT(showMessage(QString,QString)));
        connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(client, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
        connect(this, SIGNAL(sendMessage(QString)), client, SLOT(sendMessage(QString)));
qDebug() << "Start client";
        //client->startClient(service);

        //clients.append(client);


    ui->connectButton->setEnabled(true);
}

MainClient::~MainClient()
{
    delete ui;
}

void MainClient::on_startButton_clicked()
{
    emit startClicked();
}

void MainClient::toggleConnectButton()
{
    if(ui->connectButton->isEnabled()){
        ui->connectButton->setEnabled(false);
        ui->startButton->setEnabled(true);
    }
    else {
        ui->connectButton->setEnabled(true);
        ui->startButton->setEnabled(false);
    }
}

void MainClient::toggleStartButton()
{
    if(ui->startButton->isEnabled())
        ui->startButton->setEnabled(false);
    else
        ui->startButton->setEnabled(true);
}

void MainClient::processMessage(QString str){
    QDateTime current = QDateTime::currentDateTime();

       ui->console->append("["+current.toString()+"] "+str);
}

void MainClient::on_connectButton_clicked()
{
    qDebug() << "lol";
    ui->console->append("Discovering...");
    startDiscovery(QBluetoothUuid(serviceUuid));
}

void MainClient::closeEvent(QCloseEvent *event)
{
        emit closeClient();
        event->accept();
}
