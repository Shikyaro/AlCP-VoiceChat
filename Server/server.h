#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include <QDebug>
#include <QTimer>
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
    QTimer *muteBanTimer;

public:
    explicit Server(quint16 port, QObject *parent = 0);
    ~Server();

    bool     startServer(quint16 port);
    void     sendToAll(quint8 command, QByteArray data, QString senderName, bool exceptSender);
    void     sendVoiceToAll(QByteArray voice, QString senderName);
    QString  sendOnline(sClient* cli);
    void     mute(QString username, uint secs);
    void     ban(QString username, uint secs);
    void     kick(QString username);

protected:
    void    incomingConnection(qintptr handle);

signals:

public slots:
    void    onUserDisconnected(sClient *client);
    void    onVoiceSocket(sClient* cliToDel, QString username, QTcpSocket* sck);
private slots:
    void    checkMute();
};

#endif // SERVER_H
