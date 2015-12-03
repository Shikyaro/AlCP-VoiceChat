#ifndef SCLIENT_H
#define SCLIENT_H

#define lpsep QChar(0344)
#define mssep QChar(0345)

#define banpower int(49)
#define mutepower int(24)
#define kickpower int(24)

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
    void        sendOnline();
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

    static const quint8 c_message   = (quint8)4;
    static const quint8 c_ban       = (quint8)5;
    static const quint8 c_mute      = (quint8)6;
    static const quint8 c_chperm    = (quint8)7;
    static const quint8 c_unban     = (quint8)9;
    static const quint8 c_unmute    = (quint8)10;
    static const quint8 c_kick      = (quint8)11;
    static const quint8 c_err_mess  = (quint8)12;

    static const quint8 c_userConn  = (quint8)13;
    static const quint8 c_userDisc  = (quint8)14;

    static const quint8 c_onList    = (quint8)15;


    QString     getName()       {return userName;}
    bool        getLoggedIn()   {return isLoggedIn;}
    bool        getMuted()      {return isMuted;}

    void        setMuted(bool ism) {isMuted = ism;}

    void        sendBlock(quint8 command, QByteArray data);
    void        sendVoice(QByteArray data);

    void        setVoiceSocket(QTcpSocket* sock);

    void        kick();
signals:
    void        userDisconnected(sClient* client);
    void        addVoiceSocket(sClient* cliToDel, QString username, QTcpSocket* sck);

private slots:
    void        onDisconnect();
    void        onReadyRead();
    void        onReadyVoice();
};

#endif // SCLIENT_H
