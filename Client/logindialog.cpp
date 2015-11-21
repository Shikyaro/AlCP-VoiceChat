#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    login = new QLineEdit;
    pass = new QLineEdit;
    login->setMaxLength(25);
    pass->setEchoMode(QLineEdit::Password);
    pass->setMaxLength(25);

    reg = new QPushButton("Регистрация");
    log = new QPushButton("Вход");
    exit = new QPushButton("Выход");

    QGridLayout *mLay = new QGridLayout(this);
    this->setLayout(mLay);

    mLay->addWidget(new QLabel("Логин: "),0,0);
    mLay->addWidget(new QLabel("Пароль: "),1,0);
    mLay->addWidget(login,0,1);
    mLay->addWidget(pass,1,1);
    mLay->addWidget(exit,2,0);

    QHBoxLayout *bl = new QHBoxLayout;

    mLay->addLayout(bl,2,1);

    bl->addWidget(log);
    bl->addWidget(reg);

    this->setFixedSize(300,100);

    connect(reg,SIGNAL(clicked(bool)),this,SLOT(onReg()));
    connect(log,SIGNAL(clicked(bool)),this,SLOT(onLog()));
    connect(exit,SIGNAL(clicked(bool)),this,SLOT(_onExit()));
}
void LoginDialog::onReg()
{
    emit this->s_reg(login->text(), pass->text());
}

void LoginDialog::onLog()
{
    emit this->s_log(login->text(), pass->text());
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


