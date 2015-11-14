#include "server.h"

Server::Server(quint16 port, QObject *parent) : QTcpServer(parent)
{
    if (listen(QHostAddress("127.0.0.1"), port))
        qDebug() << "Started" << endl;
    else
        qDebug() << "NOt started" << endl;
}

Server::~Server()
{

}
void Server::incomingConnection(qintptr handle)
{
     Client *client = new Client(handle, this, this);

     cliList.append(client);

}

void Server::writeData(QByteArray data)
{

}
void Server::readyRead()
{

}

