#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

class database : public QObject
{
    friend class Server;
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);
    bool initialize(QString hostname, QString dbname, QString username, QString password);
    uint getPower(QString userName);
    uint getComPower(QString comName);
    bool authorize(QString userName, QString password);
    bool newUser(QString userName, QString password);
    bool setPerm(QString username, uint perms);
    bool ban(QString username, QDateTime bantime);
    bool mute(QString username, QDateTime mutetime);
    bool isBanned(QString username);
    bool isMuted(QString username);
    bool unMute(QString username);
    bool unBan(QString username);
    QDateTime getBanTime(QString username);
    QDateTime getMuteTime(QString username);
    QString getColor(QString userName);
    ~database();

signals:

public slots:

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
