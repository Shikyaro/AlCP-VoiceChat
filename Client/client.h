#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "audiooutput.h"
#include "../Server/sclient.h"
#include <QStringList>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString host, quint16 port, QObject *parent = 0);

signals:
    void    succLogin();

public slots:
    void    login(QString login, QString password);
    void    voiceSay(QByteArray data);

private slots:
    void    readyRead();

private:

    template <class dataBlock>
    void    sendBlock(quint8 command, dataBlock data);

    QTcpSocket  *socket;
    AudioOutput output;
    quint16     blockSize;
    bool        isLoggedIn;
};

#endif // CLIENT_H
