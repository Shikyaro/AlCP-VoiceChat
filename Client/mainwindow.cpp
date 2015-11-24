#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->installEventFilter(this);

    mlay = new QGridLayout();
    ml = new QHBoxLayout();

    chatWidget = new QTextBrowser();
    chatWidget->setMinimumWidth(450);
    userWidget = new QListWidget();

    chatLine = new QLineEdit();
    chatBut = new QPushButton("->");

    ml->addWidget(chatLine);
    ml->addWidget(chatBut);

    cw = new QWidget();
    mlay->addWidget(chatWidget,0,0);
    mlay->addWidget(userWidget,0,1);
    mlay->addLayout(ml,1,0);
    cw->setLayout(mlay);
    this->setCentralWidget(cw);
    cw->show();

    client = new Client("127.0.0.1",6969, this);
    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));

    //this->setEnabled(false);

    ldialog = new LoginDialog(this);
    ldialog->show();

    connect(ldialog,SIGNAL(s_log(QString,QString)),client,SLOT(login(QString,QString)));
    connect(ldialog,SIGNAL(s_reg(QString,QString)),client,SLOT(reg(QString,QString)));
    connect(ldialog,SIGNAL(quit()),this,SLOT(close()));

    connect(client,SIGNAL(unSuccReg()),ldialog,SLOT(onUnSuccReg()));
    connect(client,SIGNAL(unSuccLogin()),ldialog,SLOT(onUnSuccLogin()));
    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));
    connect(client,SIGNAL(newMessage(QString,QString,QString)),this,SLOT(newMessage(QString,QString,QString)));

    connect(chatBut,SIGNAL(clicked(bool)),this,SLOT(sendMessage()));

    this->newMessage("Admin","<img src=qrc:/smiles/cat_head.png>"
                             " test_mess аыоваоывоадфываывлаодывоадвыо","red");
}



MainWindow::~MainWindow()
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
void MainWindow::newMessage(QString username, QString message, QString col)
{
    chatWidget->append(tr("<font color = '%3'>[%1]: %2</font>").arg(username, message, col));
}
void MainWindow::sendMessage()
{
    client->stringParser(chatLine->text());
    chatLine->clear();
}
