#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include <QDebug>
#include "sclient.h"

class sClient;

class Server : public QTcpServer
{
    Q_OBJECT
private:
    QList<sClient *> cliList;
public:
    explicit Server(quint16 port, QObject *parent = 0);
    ~Server();
protected:
    void incomingConnection(qintptr handle);

signals:

public slots:
    void onUserDisconnected(sClient *client);
};

#endif // SERVER_H
