#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ml = new QVBoxLayout();
    mbut = new QPushButton("Go!");
    cw = new QWidget();
    ml->addWidget(mbut);
    cw->setLayout(ml);
    this->setCentralWidget(cw);
    cw->show();
    connect(mbut, SIGNAL(clicked()), this, SLOT(bClicked()));

    client = new Client("127.0.0.1",6969, this);
    qDebug() << "conn";

    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::bClicked()
{
    qDebug() << "sadfsdf";
    client->login("test1","nopw");

    //client = new Client("127.0.0.1",6969, this);

    //QAudioDeviceInfo devinfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(0);
    //input = new AudioInput(devinfo, this);
    //connect(input, SIGNAL(dataReady(QByteArray)), client, SLOT(writeData(QByteArray)));
}

void MainWindow::succLogin()
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,QString("succes login"),QString("Grats! Login Success"));
    msg->show();
}
