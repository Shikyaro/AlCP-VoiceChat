#ifndef SCLIENT_H
#define SCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include "server.h"


class Server;

class sClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket  *socket;
    Server      *server;
    quint16     blockSize;
    QString     name;
    bool        isLoggedIn;

    void        sendBlock(quint8 command, QByteArray data);

public:
    explicit sClient(qintptr descr, Server *serv, QObject *parent = 0);
    ~sClient();
    static const quint8 c_login     = 1;
    static const quint8 c_SuccLogin = 201;


signals:
    void        userDisconnected(sClient* client);

private slots:
    void        onDisconnect();
    void        onReadyRead();
};

#endif // SCLIENT_H
