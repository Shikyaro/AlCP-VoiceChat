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
}

MainWindow::~MainWindow()
{

}

void MainWindow::bClicked()
{

    client = new Client("127.0.0.1",6969, this);

    qDebug() << "conn";

    //QAudioDeviceInfo devinfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(0);
    //input = new AudioInput(devinfo, this);
    //connect(input, SIGNAL(dataReady(QByteArray)), client, SLOT(writeData(QByteArray)));
}
