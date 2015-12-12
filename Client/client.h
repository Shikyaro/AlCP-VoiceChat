#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "audiooutput.h"
#include "../Server/sclient.h"
#include <QStringList>
#include <QMap>

class comm
{
public:
    comm(quint8 c_id, uint arg_c){comm_id = c_id; arg_count = arg_c;}
    quint8 getId(){return comm_id;}
    uint getCount(){return arg_count;}

private:
    quint8 comm_id;
    uint arg_count;
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    bool    connectToSrv(QString host, quint16 port);
    QMap<QString,comm*> getCommMap(){return commandMap;}

signals:
    void    succLogin();
    void    unSuccLogin();
    void    succReg();
    void    unSuccReg();
    void    isBanned();
    void    newMessage(QString uname, QString msg, QString clr);
    void    nUser(QString uname);
    void    dUser(QString uname);
    void    userList(QStringList usrs);
    void    disc();
    void    errMess(QString err);

public slots:
    void    login(QString login, QString password);
    void    reg(QString login, QString password);
    void    voiceSay(QByteArray data);
    void    readVoice();
    void    stringParser(QString str);
    void    setOutVol(int vol);

private slots:
    void    readyRead();
    void    onDisconnect();

private:
    template <class dataBlock>
    void    sendBlock(quint8 command, dataBlock data);

    void    handleCommand(QString mess);
    void    replaceSmiles(QString mess);

    void    addVoiceSock();



    QTcpSocket  *socket;
    QTcpSocket  *voiceSock;
    AudioOutput output;
    quint16     blockSize;
    bool        isLoggedIn;

    QString     cHost;
    quint16     cPort;

    QString     userName;

    QMap<QString, comm*> commandMap;
    QMap<QString, QString> smilesMap;
};

#endif // CLIENT_H
