#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include <QDebug>
#include "sclient.h"
#include "database.h"

class sClient;

class Server : public QTcpServer
{
    Q_OBJECT
private:
    QList<sClient *> cliList;
    database *db;

public:
    explicit Server(quint16 port, QObject *parent = 0);
    ~Server();


    void     sendToAll(quint8 command, QByteArray data, QString senderName, bool exceptSender); /*test*/

protected:
    void    incomingConnection(qintptr handle);

signals:

public slots:
    void    onUserDisconnected(sClient *client);
};

#endif // SERVER_H
