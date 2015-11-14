#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include <QDebug>
#include "client.h"

class Client;

class Server : public QTcpServer
{
    Q_OBJECT
private:
    QList<Client *> cliList;
public:
    explicit Server(quint16 port, QObject *parent = 0);
    ~Server();
protected:
    void incomingConnection(qintptr handle);

signals:

private slots:
    void writeData(QByteArray data);
    void readyRead();

public slots:
};

#endif // SERVER_H
