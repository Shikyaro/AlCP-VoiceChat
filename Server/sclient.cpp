#include "sclient.h"

sClient::sClient(qintptr descr, Server *serv, QObject *parent) : QObject(parent)
{
    server=serv;

    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(descr);

    isLoggedIn = false;

    qDebug() << "new client" << endl;

    connect(socket, SIGNAL(disconnected()),  this,   SLOT(onDisconnect()));
    connect(socket, SIGNAL(readyRead()),     this,   SLOT(onReadyRead()));
}

sClient::~sClient()
{

}

void sClient::onDisconnect()
{
    if (isLoggedIn){
        emit userDisconnected(this);
    }else{
        deleteLater();
        qDebug() << "User disc";
    }
}

void sClient::onReadyRead()
{
    quint16 bsize = 0;
    QDataStream in(socket);
    if (bsize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> bsize;
    }
    qDebug() << "bytes " << socket->bytesAvailable();
    qDebug() << "block " << bsize;
    if (socket->bytesAvailable() < bsize)
        return;
    else
        bsize = 0;

    quint8 command;
    in >> command;

    qDebug() << "Received command " << command;
    switch (command) {
    case c_login:
        isLoggedIn = true;
        sendBlock(sClient::c_SuccLogin,NULL);
        break;
    default:
        break;
    }
}

void sClient::sendBlock(quint8 command, QByteArray data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)command;
    out << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}
