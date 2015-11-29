#include "client.h"

Client::Client(QString host, quint16 port, QObject *parent) : QObject(parent)
{
    cHost = host;
    cPort = port;

    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(host), port);

    voiceSock = new QTcpSocket(this);

    blockSize = 0;
    isLoggedIn = false;

    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(voiceSock, SIGNAL(readyRead()),this,SLOT(readVoice()));
}

void Client::readyRead()
{
    QDataStream in(socket);
    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }
        in >> blockSize;
        qDebug() << "block " << blockSize;
        qDebug() << socket->bytesAvailable();
    }
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
        voiceSock->connectToHost(cHost,cPort);
        addVoiceSock();
        sendBlock(sClient::c_onList,NULL);
        break;
    }
    case sClient::c_unSucc_L:
    {
        emit this->unSuccLogin();
        userName = "NULL";
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
    case sClient::c_message:
    {
        QString unme;
        QStringList mslist;
        in >> unme;
        mslist = unme.split(mssep);
        this->newMessage(mslist.at(0), mslist.at(1), mslist.at(2));
        break;
    }
    case sClient::c_userConn:
    {
        QString uname;
        in >> uname;

        emit this->nUser(uname);
        break;
    }
    case sClient::c_userDisc:
    {
        QString uname;
        in >> uname;

        emit this->dUser(uname);
        break;
    }
    case sClient::c_onList:
    {
        QString usrs;
        in >> usrs;
        qDebug() << usrs;
        QStringList uList;
        uList = usrs.split(",");

        emit this->userList(uList);
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
    if (data!=NULL){
        out << data;
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}

void Client::addVoiceSock()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << sClient::c_voice_say;
    out << userName;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    voiceSock->write(block);
}

void Client::login(QString login, QString password)
{
    userName = login;
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
    if (voiceSock)
        voiceSock->write(data);
}

void Client::readVoice()
{
    QByteArray voice;

    if(voiceSock->bytesAvailable() > 0)
    {
        voice.append(voiceSock->readAll());
        output.writeData(voice);
    }
}

void Client::stringParser(QString str)
{
    QString endStr = str;
    QStringList commSyn;
    if(endStr[0]=='/')
    {
        if (endStr.startsWith("/ban",Qt::CaseInsensitive)){
            commSyn = endStr.split(" ");
            QString comst;
            comst.append(commSyn.at(1));
            comst.append(",");
            comst.append(commSyn.at(2));

            sendBlock(sClient::c_ban, comst);
        }else if(endStr.startsWith("/mute",Qt::CaseInsensitive)){
            commSyn = endStr.split(" ");
            QString comst;
            comst.append(commSyn.at(1));
            comst.append(",");
            comst.append(commSyn.at(2));

            sendBlock(sClient::c_mute, comst);
        }
    }else{
        endStr.replace("<sm=cat>","<img src=qrc:/smiles/cat_head.png>",Qt::CaseInsensitive);


        sendBlock(sClient::c_message, endStr);
    }
}
