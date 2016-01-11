/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#include "client.h"
Client::Client(QObject *parent) : QObject(parent)
{

    socket = new QTcpSocket(this);

    voiceSock = new QTcpSocket(this);

    blockSize = 0;
    isLoggedIn = false;

    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(onDisconnect()));
    connect(voiceSock, SIGNAL(readyRead()),this,SLOT(readVoice()));

    commandMap["/ban"] = new comm(sClient::c_ban, 2);
    commandMap["/unban"] = new comm(sClient::c_unban, 1);
    commandMap["/mute"] = new comm(sClient::c_mute, 2);
    commandMap["/unmute"] = new comm(sClient::c_unmute, 1);
    commandMap["/kick"] = new comm(sClient::c_kick, 1);
    commandMap["/chperm"] = new comm(sClient::c_chperm, 2);

    smilesMap["cat"] = "<img src=qrc:/smiles/cat_head.png>";
}

void Client::readyRead()
{
    QDataStream in(socket);  //поток ввода с сокета
    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) //если байтов меньше 2
        {
            return; //выпадаем из функции
        }
        in >> blockSize; //если нет, то считываем размер блока
        qDebug() << "block " << blockSize;
        qDebug() << socket->bytesAvailable();
    }
    if (socket->bytesAvailable() < blockSize) //если размер блока меньше, чем пришло байт
        return; //выпадаем
    else

        blockSize = 0; //обнуляем размер блока, если пришли все данные блока

    quint8 command;
    command = 0;
    in >> command; //считываем команду
    qDebug() << command;
    switch (command) { //обрабатываем команду
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
        break;
    }
    case sClient::c_ban:
    {
        emit this->isBanned();
        break;
    }
    case sClient::c_err_mess:
    {
        QString errm;
        in >> errm;

        emit this->errMess(errm);
        break;
    }
    default:
        break;
    }
}

void Client::onDisconnect()
{
    if(voiceSock->isOpen())
        voiceSock->disconnectFromHost();
    emit this->disc();
}

template <class dataBlock>
void Client::sendBlock(quint8 command, dataBlock data)
{
    QByteArray block; //создаем блок для записи
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0; //резервируем первые 2 байта под размер
    out << command; //записываем команду
    if (data!=NULL){ //если у команды есть данные
        out << data; //пишем их
    }
    out.device()->seek(0); //перемещаемся в начало блока
    out << (quint16)(block.size() - sizeof(quint16)); //записываем в 2 зарезервированных байта размер блока
    socket->write(block); //пишем блок в сокет
}

bool Client::connectToSrv(QString host, quint16 port)
{
    cHost = host;
    cPort = port;
    socket->connectToHost(QHostAddress(host), port);
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

QString Client::hashPW(QString msg)
{
    QByteArray *spw = new QByteArray();
    QString ret;
    QDataStream out(spw,QIODevice::WriteOnly);
    out << msg;
    ret = QCryptographicHash::hash(*spw,QCryptographicHash::Sha3_256).toHex();

    return ret;
}

void Client::setAudio(bool st)
{
    output.setAudio(st);
}

void Client::login(QString login, QString password)
{
    QString pw;
    pw=hashPW(password);
    userName = login;
    QString lp(login);
    lp.append(" ");
    lp.append(pw);
    sendBlock(sClient::c_login, lp);
}

void Client::reg(QString login, QString password)
{
    QStringList spacesList = login.split(" ");
    if(spacesList.length()==1)
    {
        QString pw;
        pw=hashPW(password);
        QString lp(login);
        lp.append(" ");
        lp.append(pw);
        sendBlock(sClient::c_reg, lp);
    }
    else
        emit this->unSuccReg();
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

void Client::setOutVol(int vol)
{
    qreal vl = vol;
    vl = vl/100;
    if(output.getenb())
        output.setVol(vl);
}

void Client::handleCommand(QString mess)
{
    QStringList commsyn;
    commsyn = mess.split(" ");
    if(commandMap.contains(commsyn.at(0))){
    QMap<QString, comm*>::iterator i = commandMap.find(commsyn.at(0));
        if(commsyn.length()>=i.value()->getCount()+1){
            QString comst;
            for(uint j=1; j<=i.value()->getCount(); j++){
                if(j==2 && commsyn.at(j)=="")
                    comst.append("4");
                else
                    comst.append(commsyn.at(j));
                    comst.append(",");
            }
            comst.remove(comst.length()-1,1);
            qDebug() << comst;
            sendBlock(i.value()->getId(), comst);
        }else{
            emit this->errMess("Недостаточно аргументов команды");
        }
    }else{
        emit this->errMess("Нет такой команды");
    }
}

void Client::replaceSmiles(QString mess)
{
    QString endStr = mess;

    QMapIterator<QString, QString> i(smilesMap);
    while (i.hasNext()){
        i.next();
        endStr.replace(tr("<sm=%1>").arg(i.key()),i.value());
    }
    sendBlock(sClient::c_message, endStr);
}

void Client::stringParser(QString str)
{
    QString endStr = str;
    if(endStr[0]=='/')
    {
        handleCommand(endStr);
    }else{
        replaceSmiles(str);
    }
}
