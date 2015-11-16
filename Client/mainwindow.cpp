#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ml = new QVBoxLayout();
    mbut = new QPushButton("Go!");
    cw = new QWidget();
    ql = new QLineEdit();
    ip = new QLineEdit();
    ml->addWidget(mbut);
    ml->addWidget(ql);
    ml->addWidget(ip);
    cw->setLayout(ml);
    this->setCentralWidget(cw);
    cw->show();
    connect(mbut, SIGNAL(clicked()), this, SLOT(bClicked()));

}



MainWindow::~MainWindow()
{

}

void MainWindow::bClicked()
{
    client = new Client(ip->text(),6969, this);
    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));

    qDebug() << "conn";

    qDebug() << "sadfsdf";
    client->login(ql->text(),"nopw");

    QAudioDeviceInfo devinfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(0);
    input = new AudioInput(devinfo, this);
    connect(input, SIGNAL(dataReady(QByteArray)), client, SLOT(voiceSay(QByteArray)));
}

void MainWindow::succLogin()
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,QString("succes login"),QString("Grats! Login Success"));
    msg->show();
}
