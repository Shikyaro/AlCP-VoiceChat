#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("AlCPVC by Alkor Shikyaro");

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

    chatBut->setDefault(true);


    cw = new QWidget();

    mlay->addWidget(chatWidget,0,0);
    mlay->addWidget(userWidget,0,1);
    mlay->addLayout(ml,1,0);
    cw->setLayout(mlay);
    this->setCentralWidget(cw);
    cw->show();

    audSets = new QGroupBox("Настройки звука");
    auSetLay = new QGridLayout();

    audSets->setLayout(auSetLay);

    speakVol = new QSlider(Qt::Vertical);
    microVol = new QSlider(Qt::Vertical);

    auSetLay->addWidget(speakVol,0,0);
    auSetLay->addWidget(microVol,0,1);
    microVol->setRange(0,100);
    microVol->setSliderPosition(100);

    mlay->addWidget(audSets,0,2);

    client = new Client(this);
    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));


    //this->setEnabled(false);
    ldialog = new LoginDialog(client,this);
    ldialog->show();

    connect(ldialog,SIGNAL(s_log(QString,QString)),client,SLOT(login(QString,QString)));
    connect(ldialog,SIGNAL(s_reg(QString,QString)),client,SLOT(reg(QString,QString)));
    connect(ldialog,SIGNAL(quit()),this,SLOT(close()));

    connect(client,SIGNAL(unSuccReg()),ldialog,SLOT(onUnSuccReg()));
    connect(client,SIGNAL(unSuccLogin()),ldialog,SLOT(onUnSuccLogin()));
    connect(client,SIGNAL(succLogin()),this,SLOT(succLogin()));
    connect(client,SIGNAL(newMessage(QString,QString,QString)),this,SLOT(newMessage(QString,QString,QString)));
    connect(client,SIGNAL(nUser(QString)),this,SLOT(newUser(QString)));
    connect(client,SIGNAL(dUser(QString)),this,SLOT(userDisc(QString)));
    connect(client,SIGNAL(userList(QStringList)),this,SLOT(drawUserList(QStringList)));
    connect(client,SIGNAL(isBanned()),ldialog,SLOT(onBan()));

    connect(client,SIGNAL(disc()),this,SLOT(onDisc()));

    connect(chatBut,SIGNAL(clicked(bool)),this,SLOT(sendMessage()));
}
MainWindow::~MainWindow()
{

}

void MainWindow::onDisc()
{
    QTimer::singleShot(10000,this,SLOT(close()));
    QMessageBox::warning(this,"Разрыв соединения","Сервер разорвал соединение");
}

void MainWindow::succLogin()
{
    ldialog->close();
    this->setEnabled(true);

    QAudioDeviceInfo devinfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(0);
    input = new AudioInput(devinfo, this);
    connect(input, SIGNAL(dataReady(QByteArray)), client, SLOT(voiceSay(QByteArray)));
    connect(microVol,SIGNAL(valueChanged(int)),input,SLOT(setMicVol(int)));
}

void MainWindow::newMessage(QString username, QString message, QString col)
{
    chatWidget->append(tr("<font color = '%3'>[%1]: %2</font>").arg(username, message, col));
}
void MainWindow::sendMessage()
{
    if(chatLine->text()!="")
        client->stringParser(chatLine->text());
    chatLine->clear();

}
void MainWindow::newUser(QString username)
{
    userWidget->addItem(username);
}

void MainWindow::userDisc(QString username)
{
    delete userWidget->findItems(username,Qt::MatchExactly).at(0);
}

void MainWindow::drawUserList(QStringList ulist)
{
    QString str;

    foreach (str, ulist) {
        if(str!=""){
            newUser(str);
        }
    }
}
