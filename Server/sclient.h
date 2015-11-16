#ifndef SCLIENT_H
#define SCLIENT_H

#define lpsep QChar(0344)

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QStringList>
#include "server.h"


class Server;

class sClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket  *socket;
    Server      *server;
    quint16     blockSize;
    bool        isLoggedIn;

    QString     userName;
    bool        isMuted;


public:
    explicit sClient(qintptr descr, Server *serv, QObject *parent = 0);
    ~sClient();

    static const quint8 c_login     = 1;
    static const quint8 c_SuccLogin = 201;
    static const quint8 c_voice_say = 2;

    QString getName()       {return userName;}
    bool    getLoggedIn()   {return isLoggedIn;}

    void        sendBlock(quint8 command, QByteArray data);
signals:
    void        userDisconnected(sClient* client);

private slots:
    void        onDisconnect();
    void        onReadyRead();
};

#endif // SCLIENT_H
