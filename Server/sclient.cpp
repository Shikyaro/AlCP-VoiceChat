/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
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
        //emit userDisconnected(this);
    }else{
        //deleteLater();
        qDebug() << "User disc";
    }
    emit userDisconnected(this);
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
        lp = logp.split(" ");

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
                sendBlock(sClient::c_ban,NULL);
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

        lp = logp.split(" ");

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
    {
        QString msg;
        in >> msg;
        if(isLoggedIn){
                QString mss;
                if (!server->db->isMuted(this->userName)){
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
                 createError("Вы не можте писать сообщения в чат");
            }

        }
        break;
    }
    case c_onList:
    {
        sendOnline();
        break;
    }
    case c_ban:
    {
        QString uname;
        in >> uname;
        if (server->db->getPower(this->userName)>server->db->getComPower("ban"))
        {
            QStringList comList;

            comList = uname.split(",");

            if (server->db->getPower(this->userName)>server->db->getPower(comList.at(0))){

                server->ban(comList.at(0),QString(comList.at(1)).toLongLong());
                server->serverMessage(tr("Пользователь \"%1\" получил бан на %2 секунд").arg(comList.at(0),comList.at(1)));
            }
            else
            {
                createError("У вас нет прав на применение этой команды к этому пользователю");
            }
        }else
             createError("У вас нет прав на использование этой команды");
        break;
    }
    case c_mute:
    {
        QString ucom;
        in >> ucom;
        if (server->db->getPower(this->userName)>server->db->getComPower("mute"))
        {

            QStringList comlst;

            comlst = ucom.split(",");

            if (server->db->getPower(this->userName)>server->db->getPower(comlst.at(0))){
                server->mute(comlst.at(0),QString(comlst.at(1)).toLongLong());
                server->serverMessage(tr("Пользователь '%1' получил мут на %2 секунд").arg(comlst.at(0),comlst.at(1)));
            }
            else
            {
                createError("У вас нет прав на применение этой команды к этому пользователю");
            }
        }else
            createError("У вас нет прав на использование этой команды");
        break;
    }
    case c_kick:
    {
        QString ucom;
        in >> ucom;
        if (server->db->getPower(this->userName)>=server->db->getComPower("kick"))
        {
            if (server->db->getPower(this->userName)>server->db->getPower(ucom)){
                server->kick(ucom);
                server->serverMessage(tr("Пользователь %1 был выкинут из чата").arg(ucom));
            }
            else
            {
                createError("У вас нет прав на применение этой команды к этому пользователю");
            }
        }else{
            createError("У вас нет прав на использование этой команды");
        }
        break;
    }
    case c_unmute:
    {
        QString ucom;
        in >> ucom;
        if(server->db->getPower(this->userName)>=server->db->getComPower("mute"))
        {
            if (server->db->getPower(this->userName)>server->db->getPower(ucom))
                server->unmute(ucom);
            else
            {
                createError("У вас нет прав на применение этой команды к этому пользователю");
            }
        }else
            createError("У вас нет прав на использование этой команды");
        break;
    }
    case c_unban:
    {
        QString ucom;
        in >> ucom;
        if(server->db->getPower(this->userName)>=server->db->getComPower("ban"))
        {
            if (server->db->getPower(this->userName)>server->db->getPower(ucom))
                server->unban(ucom);
            else
            {
                createError("У вас нет прав на применение этой команды к этому пользователю");
            }
        } else
            createError("У вас нет прав на использование этой команды");
        break;
    }
    case c_chperm:
    {
        QString ucom;
        in >> ucom;
        if (server->db->getPower(this->userName)>server->db->getComPower("chperm"))
        {
            QStringList comlst;
            comlst = ucom.split(",");
            if ((server->db->getPower(this->userName)>server->db->getPower(comlst.at(0)))&&(QString(comlst.at(1)).toUInt()!=1))
                server->db->setPerm(comlst.at(0),QString(comlst.at(1)).toUInt());
            else
            {
                createError("У вас нет прав на применение этой команды к этому пользователю");
            }
        } else
            createError("У вас нет прав на использование этой команды");
        break;
    }
    default:
        break;
    }

}

void sClient::kick()
{
    if (voiceSock)
        voiceSock->disconnectFromHost();
    if (socket)
        socket->disconnectFromHost();

}

void sClient::createError(QString errT)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << errT;

    this->sendBlock(c_err_mess, data);
}

void sClient::onReadyVoice()
{
    QByteArray vc;
    if(voiceSock->bytesAvailable()>0){
        if (!isMuted){
            vc.append(voiceSock->readAll());
                if(!server->db->isMuted(this->userName))
                    server->sendVoiceToAll(vc, userName);
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
    QRegularExpression reg("[A-Za-z0-9_]+$");
    QRegularExpressionMatch rmat = (reg.match(userName));
    return rmat.isValid();
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
