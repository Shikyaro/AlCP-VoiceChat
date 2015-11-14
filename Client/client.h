#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "audiooutput.h"
#include "../Server/sclient.h"


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString host, quint16 port, QObject *parent = 0);

signals:
    void succLogin();

public slots:
    void sendBlock(quint8 command, QByteArray data);
    void login(QString login, QString password);
    //void logout();

private slots:
    void readyRead();

private:
    QTcpSocket *socket;
    AudioOutput output;
    quint16 blockSize;
};

#endif // CLIENT_H
