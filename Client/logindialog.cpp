#include "logindialog.h"

LoginDialog::LoginDialog(Client *cli, QWidget *parent) : QDialog(parent)
{
    client = cli;
    isConnected = false;

    login = new QLineEdit;
    pass = new QLineEdit;
    login->setMaxLength(25);
    pass->setEchoMode(QLineEdit::Password);
    pass->setMaxLength(25);
    ip = new QLineEdit();
    port = new QLineEdit("6969");

    reg = new QPushButton("Регистрация");
    log = new QPushButton("Вход");
    log->setDefault(true);
    exit = new QPushButton("Выход");

    QGridLayout *mLay = new QGridLayout(this);
    this->setLayout(mLay);

    mLay->addWidget(new QLabel("Логин: "),2,0);
    mLay->addWidget(new QLabel("Пароль: "),3,0);
    mLay->addWidget(login,2,1);
    mLay->addWidget(pass,3,1);
    mLay->addWidget(exit,4,0);
    mLay->addWidget(new QLabel("IP: "),0,0);
    mLay->addWidget(new QLabel("Порт: "),1,0);
    mLay->addWidget(ip,0,1);
    mLay->addWidget(port,1,1);

    QHBoxLayout *bl = new QHBoxLayout;

    mLay->addLayout(bl,4,1);

    bl->addWidget(log);
    bl->addWidget(reg);

    this->setFixedSize(300,150);

    connect(reg,SIGNAL(clicked(bool)),this,SLOT(onReg()));
    connect(log,SIGNAL(clicked(bool)),this,SLOT(onLog()));
    connect(exit,SIGNAL(clicked(bool)),this,SLOT(_onExit()));
}
void LoginDialog::onReg()
{
    //if(client->connectToSrv(ip->text(),port->text().toUInt()))
    client->connectToSrv(ip->text(),port->text().toUInt());
        emit this->s_reg(login->text(), pass->text());
    //else
       // QMessageBox::warning(this, "Ошибка", "Невозможно подключиться к серверу");
}

void LoginDialog::onLog()
{
    //if(client->connectToSrv(ip->text(),port->text().toUInt()))
    client->connectToSrv(ip->text(),port->text().toUInt());
        emit this->s_log(login->text(), pass->text());
    //else
       // QMessageBox::warning(this, "Ошибка", "Невозможно подключиться к серверу");
}

void LoginDialog::_onExit()
{
    emit this->quit();
}
void LoginDialog::onUnSuccLogin()
{
    QMessageBox::warning(this,"Ошибка","Неверный логин и/или пароль.");
}
void LoginDialog::onUnSuccReg()
{
    QMessageBox::warning(this,"Ошибка","Ошибка регистрации\nВозможно такой логин уже существует или он не соответствует правилам\n (Только A-z и _, а также не длиннее 25 символов)");
}
void LoginDialog::onBan()
{
    QMessageBox::warning(this,"Ошибка","Извините, но вы забанены на этом сервере");
}


