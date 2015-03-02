#include "mainclient.h"
#include "ui_mainclient.h"

MainClient::MainClient(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClient)

{
    ui->setupUi(this);

    //QObject::connect(ui->connect_button, SIGNAL(clicked()),
           // this, SLOT(on_connect_button_clicked()));
    QObject::connect(&client, SIGNAL(sendMessage(QString)),
            this, SLOT(processMessage(QString)));
    QObject::connect(&client, SIGNAL(toggleConnected()),
            this, SLOT(toggleConnectButton()));
    QObject::connect(this, SIGNAL(closeClient()),
            &client, SLOT(setQuitTrue()));
    QObject::connect(this, SIGNAL(startClicked()),
            &client, SLOT(toggleEvent()));
   ui->textEdit->append("Welcome");
   ui->start_button->setEnabled(false);
    //btclient = new Btclient*;
}

MainClient::~MainClient()
{
    delete ui;
}

void MainClient::on_start_button_clicked()
{
    emit startClicked();
}

void MainClient::toggleConnectButton()
{
    if(ui->connect_button->isEnabled()){
        ui->connect_button->setEnabled(false);
        ui->start_button->setEnabled(true);
    }
    else {
        ui->connect_button->setEnabled(true);
        ui->start_button->setEnabled(false);
    }
}

void MainClient::toggleStartButton()
{
    if(ui->start_button->isEnabled())
        ui->start_button->setEnabled(false);
    else
        ui->start_button->setEnabled(true);
}

void MainClient::processMessage(QString str){
    QDateTime current = QDateTime::currentDateTime();

       ui->textEdit->append("["+current.toString()+"] "+str);

    if(str == "Server can't be reached"){
        toggleConnectButton();
    }
}

void MainClient::on_connect_button_clicked()
{
    //client.connectToHost();
    ui->textEdit->append("Connecting...");
    client.startClient();
}

void MainClient::closeEvent(QCloseEvent *event)
{
        emit closeClient();
        event->accept();
}
