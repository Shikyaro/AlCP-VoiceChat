#include "database.h"

database::database(QObject *parent) : QObject(parent)
{

}

database::~database()
{

}

bool database::initialize(QString hostname, QString dbname, QString username, QString password)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostname);
    db.setPort(3306);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);

    return db.open();

}
uint database::getPower(QString userName)
{
    QSqlQuery que;
    que.prepare(QString("SELECT permissionpower "
                "FROM permissions "
                "INNER JOIN users ON permissions.permissionid = users.permissions "
                "WHERE users.username = \"%1\"").arg(userName));
    if(que.exec()){
        que.last();
        return que.value(0).toUInt();
    }else{
        qDebug() << "Exec failed on getPower";
        return 0;
    }
}
bool database::authorize(QString userName, QString password)
{
    QSqlQuery que;
    que.prepare(QString("SELECT `password` "
                        "FROM users "
                        "WHERE users.username = \"%1\"").arg(userName));
    if(que.exec()){
        que.last();
        return (password==que.value(0).toString());
    }else{
        qDebug() << "Exec failed on authorize";
        return false;
    }
}

bool database::newUser(QString userName, QString password)
{
    QSqlQuery que;
    que.prepare(QString("INSERT INTO users (username, `password`, permissions, muted, banned)"
                        "VALUES ('%1','%2',4,0,0)").arg(userName,password));

    if (que.exec()){
        return true;
    }else{
        qDebug() << que.lastError().databaseText();
        return false;
    }
}

