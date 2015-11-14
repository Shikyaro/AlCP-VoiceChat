#include "client.h"

Client::Client(qintptr descr, Server *serv, QObject *parent) : QObject(parent)
{
    server=serv;

    socket=new QTcpSocket(this);
    socket->setSocketDescriptor(descr);

    isLoggedIn = false;

    qDebug() << "new client" << endl;
}

Client::~Client()
{

}

