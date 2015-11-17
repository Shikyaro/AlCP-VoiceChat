#include "server.h"

Server::Server(quint16 port, QObject *parent) : QTcpServer(parent)
{
    if (listen(QHostAddress::Any,port))
        qDebug() << "Сервер запущен" << endl;
    else
        qDebug() << "NOt started" << endl;
}

Server::~Server()
{

}
void Server::incomingConnection(qintptr handle)
{
     sClient *client = new sClient(handle, this, this);

     connect(client,SIGNAL(userDisconnected(sClient*)),this,SLOT(onUserDisconnected(sClient*)));

     cliList.append(client);

}
void Server::onUserDisconnected(sClient *client)
{
    cliList.removeAt(cliList.indexOf(client));

    qDebug() << "Disced!";
}

void Server::sendToAll(quint8 command, QByteArray data, QString senderName, bool exceptSender)
{
    sClient *cli;

    foreach (cli, cliList) {
            /*if((cli->getName()!=senderName)&&(cli->getLoggedIn())&&(exceptSender)){
                 cli->sendBlock(command, data);
        }else{
            if(cli->getLoggedIn())*/
        //qDebug() << data.size();
            cli->sendBlock(command, data);
        }
    }
//}

