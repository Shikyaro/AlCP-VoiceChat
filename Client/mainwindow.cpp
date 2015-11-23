#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->installEventFilter(this);

    ml = new QVBoxLayout();
    mbut = new QPushButton("Go!");
    cw = new QWidget();
    ql = new QLineEdit();
    ip = new QLineEdit();
    ip->setText("127.0.0.1");
    ml->addWidget(mbut);
    ml->addWidget(ql);
    ml->addWidget(ip);
    cw->setLayout(ml);
    this->setCentralWidget(cw);
    cw->show();
    connect(mbut, SIGNAL(clicked()), this, SLOT(bClicked()));

    client = new Client(ip->text(),6969, this);
    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));

    this->setEnabled(false);

    ldialog = new LoginDialog(this);
    ldialog->show();

    connect(ldialog,SIGNAL(s_log(QString,QString)),client,SLOT(login(QString,QString)));
    connect(ldialog,SIGNAL(s_reg(QString,QString)),client,SLOT(reg(QString,QString)));
    connect(ldialog,SIGNAL(quit()),this,SLOT(close()));

    connect(client,SIGNAL(unSuccReg()),ldialog,SLOT(onUnSuccReg()));
    connect(client,SIGNAL(unSuccLogin()),ldialog,SLOT(onUnSuccLogin()));
    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));
}



MainWindow::~MainWindow()
{

}

void MainWindow::bClicked()
{

}

void MainWindow::succLogin()
{
    ldialog->close();
    this->setEnabled(true);

    QAudioDeviceInfo devinfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(0);
    input = new AudioInput(devinfo, this);
    connect(input, SIGNAL(dataReady(QByteArray)), client, SLOT(voiceSay(QByteArray)));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this && event->type() == QEvent::Close){
        //event->ignore();
        this->showMinimized();
        return true;
    }else{
        return false;
    }
}
