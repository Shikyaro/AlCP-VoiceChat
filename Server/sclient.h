#ifndef SCLIENT_H
#define SCLIENT_H

#define lpsep QChar(0344)

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QStringList>
#include <QRegExp>
#include "server.h"


class Server;

class sClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket  *socket;
    QTcpSocket  *voiceSock;
    Server      *server;
    quint16     blockSize;
    bool        isLoggedIn;

    QString     userName;
    bool        isMuted;

    bool        isValid(QString userName);
public:
    explicit sClient(qintptr descr, Server *serv, QObject *parent = 0);
    ~sClient();

    static const quint8 c_login     = (quint8)1;
    static const quint8 c_SuccLogin = (quint8)201;
    static const quint8 c_unSucc_L  = (quint8)202;

    static const quint8 c_voice_say = (quint8)2;

    static const quint8 c_reg       = (quint8)3;
    static const quint8 c_Succ_Reg  = (quint8)203;
    static const quint8 c_unSucc_R  = (quint8)204;


    QString     getName()       {return userName;}
    bool        getLoggedIn()   {return isLoggedIn;}

    void        sendBlock(quint8 command, QByteArray data);
    void        sendVoice(QByteArray data);

    void        setVoiceSocket(QTcpSocket* sock);
signals:
    void        userDisconnected(sClient* client);
    void        addVoiceSocket(sClient* cliToDel, QString username, QTcpSocket* sck);

private slots:
    void        onDisconnect();
    void        onReadyRead();
    void        onReadyVoice();
};

#endif // SCLIENT_H
