/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    srv = new Server(6969,this);

    QWidget *mw = new QWidget(this);
    this->setCentralWidget(mw);

    QGridLayout *mlay = new QGridLayout();
    mw->setLayout(mlay);

    startSrv = new QPushButton("Запуск");
    portLine = new QLineEdit("6969");
    status = new QLabel("Сервер не запущен");

    mlay->addWidget(new QLabel("Порт: "),0,0);
    mlay->addWidget(portLine,0,1);
    mlay->addWidget(startSrv,0,2);
    mlay->addWidget(status, 1,1);

    connect(startSrv,SIGNAL(clicked(bool)),this,SLOT(onStartSrv()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::onStartSrv()
{
    if(srv->startServer(portLine->text().toUInt()))
    {
        QMessageBox::information(this,"Успех!","Сервер успешно запущен");
        status->setText("Сервер запущен");
    }
    else
        QMessageBox::warning(this,"Ошибка","Невозможно запустить сервер\nПроверьте настройки порта и подключение к базе данных\nВозможно сервер уже запущен");
}
