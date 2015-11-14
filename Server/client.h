#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include "server.h"

class Server;

class Client : public QObject
{
    Q_OBJECT
private:
    QTcpSocket  *socket;
    Server      *server;
    quint16     blockSize;
    QString     name;
    bool        isLoggedIn;

public:
    explicit Client(qintptr descr, Server *serv, QObject *parent = 0);
    ~Client();

signals:

public slots:
};

#endif // CLIENT_H
