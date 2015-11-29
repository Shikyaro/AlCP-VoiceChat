#include "sclient.h"

sClient::sClient(qintptr descr, Server *serv, QObject *parent) : QObject(parent)
{
    server=serv;

    blockSize = 0;
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(descr);

    voiceSock = new QTcpSocket(this);

    isLoggedIn = false;
    isMuted = false;
    userName = "NULL";



    qDebug() << "new client" << endl;

    connect(socket, SIGNAL(disconnected()),  this,   SLOT(onDisconnect()));
    connect(socket, SIGNAL(readyRead()),     this,   SLOT(onReadyRead()));

    connect(voiceSock, SIGNAL(readyRead()),  this,   SLOT(onReadyVoice()));
}

sClient::~sClient()
{

}

void sClient::onDisconnect()
{
    if (isLoggedIn){
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << this->userName;

        server->sendToAll(sClient::c_userDisc,data,this->userName,true);
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
            return;
        }
        in >> blockSize;
    }
    if (socket->bytesAvailable() < blockSize)
        return;
    else
        blockSize = 0;

    quint8 command;
    command = 0;
    in >> command;

    switch (command) {
    case c_login:
    {
        QStringList lp;
        QString logp;
        in >> logp;
        lp = logp.split(lpsep);

        if (server->db->authorize(lp.at(0),lp.at(1))){
            if(!server->db->isBanned(lp.at(0))){
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                userName = lp.at(0);
                out << userName;
                isLoggedIn = true;
                sendBlock(sClient::c_SuccLogin,NULL);
                server->sendToAll(sClient::c_userConn, data, this->userName,true);
                isMuted=server->db->isMuted(this->userName);
            }else{
                if(QDateTime::currentDateTime()>=server->db->getBanTime(lp.at(0)))
                    server->db->unBan(lp.at(0));
                socket->disconnectFromHost();
            }
        }else{
            sendBlock(sClient::c_unSucc_L,NULL);
        }

        break;
    }
    case c_voice_say:
    {
        QString un;
        in >> un;

        emit this->addVoiceSocket(this, un, socket);
        break;
    }
    case c_reg:
    {
        QStringList lp;
        QString logp;
        in >> logp;

        lp = logp.split(lpsep);

        if ((isValid(lp.at(0)))){ 
            if((server->db->newUser(lp.at(0),lp.at(1))))
                sendBlock(c_Succ_Reg,NULL);
            else
                sendBlock(c_unSucc_R,NULL);
        }else{
            sendBlock(c_unSucc_R,NULL);
        }
        break;
    }
    case c_message:
        if(isLoggedIn){
            if (!isMuted){
                QString msg;
                QString mss;
                in >> msg;
                QByteArray data;
                QDataStream out(&data, QIODevice::WriteOnly);
                //mss.append(mssep);
                mss.append(userName);
                mss.append(mssep);
                mss.append(msg);
                mss.append(mssep);
                mss.append(server->db->getColor(userName));
                out << mss;
                server->sendToAll(c_message,data,userName,false);
            }else{

            }
        }
        break;
    case c_onList:
    {
        sendOnline();
        break;
    }
    case c_ban:
    {
        if (server->db->getPower(this->userName)>=banpower)
        {
            QString uname;
            in >> uname;
            //qDebug() << uname;

        }
        else
        {
            //sendBlock(c_err_mess,);
        }
        break;
    }
    case c_mute:
    {
        if (server->db->getPower(this->userName)>=mutepower)
        {
            QString ucom;
            QStringList comlst;

            in >> ucom;
            comlst = ucom.split(",");

            if (server->db->getPower(this->userName)>server->db->getPower(comlst.at(0)))
                server->mute(comlst.at(0),QString(comlst.at(1)).toUInt());
            else
            {

            }
        }
        break;
    }
    default:
        break;
    }

}

void sClient::onReadyVoice()
{
    if(!isMuted)
    {
        QByteArray vc;
        if(voiceSock->bytesAvailable()>0){
            if (!isMuted){
                vc.append(voiceSock->readAll());
                server->sendVoiceToAll(vc, userName);
            }
        }
    }
}

void sClient::sendBlock(quint8 command, QByteArray data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << command;
    if (data != NULL)
        //out << data;
        block.append(data);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}

void sClient::sendVoice(QByteArray data)
{
    voiceSock->write(data);
}

bool sClient::isValid(QString userName)
{
    QRegExp reg("[A-z_]+");
    return (reg.exactMatch(userName));
}

void sClient::setVoiceSocket(QTcpSocket* sock)
{
    voiceSock->setSocketDescriptor(sock->socketDescriptor());
}
void sClient::sendOnline()
{
    QByteArray udata;
    QDataStream uOut(&udata,QIODevice::WriteOnly);

    uOut << server->sendOnline(this);
    this->sendBlock(c_onList, udata);
}
