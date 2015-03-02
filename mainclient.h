#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QMainWindow>
#include <QDateTime>
#include <QCloseEvent>
#include "btclient.h"
#include "ui_mainclient.h"

namespace Ui {
class MainClient;
}

class MainClient : public QMainWindow
{
    Q_OBJECT

public:
    MainClient(QMainWindow *parent = 0);
    ~MainClient();
signals:
    void startClicked();
    void connectClicked();
    void closeClient();

private slots:

    void on_start_button_clicked();
    void on_connect_button_clicked();
    void toggleConnectButton();
    void toggleStartButton();

public slots:
   void processMessage(QString);

private:
    Ui::MainClient *ui;
   Btclient client ;
    void closeEvent(QCloseEvent*);
};

#endif // MAINCLIENT_H
