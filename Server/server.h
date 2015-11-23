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
    friend class sClient;
private:
    QList<sClient *> cliList;
    database *db;

public:
    explicit Server(quint16 port, QObject *parent = 0);
    ~Server();


    void     sendToAll(quint8 command, QByteArray data, QString senderName, bool exceptSender);
    void     sendVoiceToAll(QByteArray voice, QString senderName);

protected:
    void    incomingConnection(qintptr handle);

signals:

public slots:
    void    onUserDisconnected(sClient *client);
    void    onVoiceSocket(sClient* cliToDel, QString username, QTcpSocket* sck);
};

#endif // SERVER_H
