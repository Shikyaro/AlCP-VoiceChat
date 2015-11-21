#include "client.h"

Client::Client(QString host, quint16 port, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(host), port);

    blockSize = 0;
    isLoggedIn = false;

    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void Client::readyRead()
{
    blockSize = 0;
    QDataStream in(socket);
    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }
        in >> blockSize;

    }
    qDebug() << blockSize;
    qDebug() << socket->bytesAvailable();
    if (socket->bytesAvailable() < blockSize)
        return;
    else
        blockSize = 0;

    quint8 command;
    command = 0;
    in >> command;
    qDebug() << command;
    switch (command) {
    case sClient::c_SuccLogin:
    {
        emit this->succLogin();
        isLoggedIn = true;
        break;
    }
    case sClient::c_unSucc_L:
    {
        emit this->unSuccLogin();
        break;
    }
    case sClient::c_Succ_Reg:
    {
        emit this->succReg();
        break;
    }
    case sClient::c_unSucc_R:
    {
        emit this->unSuccReg();
        break;
    }
    case sClient::c_voice_say:
    {
        QByteArray dat;
        in >> dat;
        output.writeData(dat);
        break;
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
void Client::reg(QString login, QString password)
{
    QString lp(login);
    lp.append(lpsep);
    lp.append(password);

    sendBlock(sClient::c_reg, lp);
}

void Client::voiceSay(QByteArray data)
{
    sendVoice(data);
}

void Client::sendVoice(QByteArray data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << sClient::c_voice_say << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}
