#include "client.h"

Client::Client(QString host, quint16 port, QObject *parent) : QObject(parent)
{
    QTcpSocket *sock = new QTcpSocket(this);
    sock->connectToHost(QHostAddress(host), port);

    //connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
}

void Client::readyRead()
{
    QByteArray data;

    while (socket->bytesAvailable() > 0)
        data.append(socket->readAll());

    output.writeData(data);
}

void Client::writeData(QByteArray data)
{
    if(socket)
        socket->write(data);
}
