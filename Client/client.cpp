#include "client.h"

Client::Client(QString host, quint16 port, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(host), port);

    blockSize = 0;
    isLoggedIn = false;

    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
}

void Client::readyRead()
{
    QDataStream in(socket);
    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }
    if (socket->bytesAvailable() < blockSize)
        return;
    else
        blockSize = 0;
    quint8 command;
    in >> command;

    switch (command) {
    case sClient::c_SuccLogin:
        emit succLogin();
        isLoggedIn = true;
        break;
    case sClient::c_voice_say:
    {
        QByteArray dat;
        in >> dat;
        output.writeData(dat);
    }
    default:
        break;
    }
}

template <class dataBlock>
void Client::sendBlock(quint8 command, dataBlock data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << command;
    out << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}

void Client::login(QString login, QString password)
{
    QString lp(login);
    lp.append(lpsep);
    lp.append(password);

    sendBlock(sClient::c_login, lp);
}

void Client::voiceSay(QByteArray data)
{
    sendBlock(sClient::c_voice_say, data);
}
