#include "server.h"

Server::Server(quint16 port, QObject *parent) : QTcpServer(parent)
{
    if (listen(QHostAddress::Any,port))
        qDebug() << "Сервер запущен" << endl;
    else
        qDebug() << "NOt started" << endl;

    db = new database(this);
    if(db->initialize("127.0.0.1","alcpvc","root","alcpvc"))
        qDebug() << "БД подключено";
    else
        qDebug() << "БД не подключено!!!!";

    muteBanTimer = new QTimer();
    muteBanTimer->start(10000);

    connect(muteBanTimer,SIGNAL(timeout()),this,SLOT(checkMute()));
}

Server::~Server()
{

}
void Server::incomingConnection(qintptr handle)
{
     sClient *client = new sClient(handle, this, this);

     connect(client,SIGNAL(userDisconnected(sClient*)),this,SLOT(onUserDisconnected(sClient*)));
     connect(client,SIGNAL(addVoiceSocket(sClient*,QString,QTcpSocket*)),this,SLOT(onVoiceSocket(sClient*,QString,QTcpSocket*)));

     cliList.append(client);

}
void Server::onUserDisconnected(sClient *client)
{
    cliList.removeAt(cliList.indexOf(client));

    qDebug() << "User " << client->getName() << " disconnected";
}

void Server::sendToAll(quint8 command, QByteArray data, QString senderName, bool exceptSender)
{
    sClient *cli;

    foreach (cli, cliList) {
            if((exceptSender)){
                if(cli->getName()!=senderName)
                    if(cli->getLoggedIn())
                        cli->sendBlock(command, data);
        }else{
            if(cli->getLoggedIn())
                cli->sendBlock(command, data);
        }
    }
}

void Server::onVoiceSocket(sClient *cliToDel, QString username, QTcpSocket *sck)
{
    sClient *cli;

    foreach (cli, cliList) {
        if (cli->getName()==username)
            cli->setVoiceSocket(sck);
    }

    cliList.removeAt(cliList.indexOf(cliToDel));
}

void Server::sendVoiceToAll(QByteArray voice, QString senderName)
{
    sClient *cli;
    foreach (cli, cliList) {
        if(cli->getName()!=senderName)
            if(cli->getLoggedIn())
                cli->sendVoice(voice);
    }

}

QString Server::sendOnline(sClient *cli)
{
    QString users;
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    for (uint i = 0; i<cliList.length(); i++)
    {
        if(cli->getName()!=cliList.at(i)->getName() && cliList.at(i)->getName()!= "NULL")
        {
            users.append(cliList.at(i)->getName());
            users.append(",");
        }
    }
    users.remove(users.length()-1,1);

    return users;
}

void Server::checkMute()
{
    sClient *cli;
    foreach (cli, cliList) {
        if(cli->getName()!=NULL)
            if((cli->getMuted())||(db->isMuted(cli->getName())))
                if(QDateTime::currentDateTime() > db->getMuteTime(cli->getName())){
                    db->unMute(cli->getName());
                    cli->setMuted(false);
                }
    }
}

void Server::mute(QString username, uint secs)
{
    db->mute(username,QDateTime::currentDateTime().addSecs(secs));

    sClient *cli;

    foreach (cli, cliList) {
        if(cli->getName()==username)
            cli->setMuted(true);
    }


}


