#include "sclient.h"

sClient::sClient(qintptr descr, Server *serv, QObject *parent) : QObject(parent)
{
    server=serv;

    blockSize = 0;
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(descr);

    isLoggedIn = false;
    isMuted = false;
    userName = "NULL";



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
    QDataStream in(socket);
    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
        {
            qDebug() << "LOWER " << (int)sizeof(quint16);
            return;
        }
        in >> blockSize;
    }
    qDebug() << "bytes " << socket->bytesAvailable();
    qDebug() << "block " << blockSize;
    if (socket->bytesAvailable() < blockSize)
        return;
    else
        blockSize = 0;

    quint8 command;
    command = 0;
    in >> command;

    qDebug() << "Received command " << command;
    switch (command) {
    case c_login:
    {
        QStringList lp;
        QString logp;
        in >> logp;
        lp = logp.split(lpsep);
        userName = lp.at(0);
        qDebug() << "usr: " << userName << " pwd: " << lp.at(1) << " " << lpsep;
        isLoggedIn = true;

        sendBlock(sClient::c_SuccLogin,NULL);

        break;
    }
    case c_voice_say:
    {
        if(!isMuted){
            QByteArray vb;
            in >> vb;

            server->sendToAll(c_voice_say, vb, userName, true);
        }
    }
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
    //out << data;
    data.resize(data.length());
    block.append(data);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}
