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
    //ret = que.value(0).toUInt();
    }else{
        qDebug() << "exec failed";
    }

    return 0;
}

