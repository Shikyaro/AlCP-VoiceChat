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

QString database::getColor(QString userName)
{
    QSqlQuery que;
    que.prepare(QString("SELECT permissioncolor "
                "FROM permissions "
                "INNER JOIN users ON permissions.permissionid = users.permissions "
                "WHERE users.username = \"%1\"").arg(userName));
    if(que.exec()){
        que.last();
        return que.value(0).toString();
    }else{
        qDebug() << "Exec failed on getColor";
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

bool database::ban(QString username, QDateTime bantime)
{
    QSqlQuery que;
    que.prepare(QString("UPDATE users "
                        "SET banned = 1, banend = '%1-%2-%3 %4:%5:%6' "
                        "WHERE username = '%7'").arg(tr("%1").arg(bantime.date().year()),
                                                   tr("%1").arg(bantime.date().month()),
                                                   tr("%1").arg(bantime.date().day()),
                                                   tr("%1").arg(bantime.time().hour()),
                                                   tr("%1").arg(bantime.time().minute()),
                                                   tr("%1").arg(bantime.time().second()),
                                                   username));
    if(que.exec())
        return true;
    else{
        qDebug() << que.lastError().databaseText();
        return false;
    }
}


bool database::mute(QString username, QDateTime mutetime)
{
    QSqlQuery que;
    que.prepare(QString("UPDATE users "
                        "SET muted = 1, muteend = '%1-%2-%3 %4:%5:%6' "
                        "WHERE username = '%7'").arg(tr("%1").arg(mutetime.date().year()),
                                                   tr("%1").arg(mutetime.date().month()),
                                                   tr("%1").arg(mutetime.date().day()),
                                                   tr("%1").arg(mutetime.time().hour()),
                                                   tr("%1").arg(mutetime.time().minute()),
                                                   tr("%1").arg(mutetime.time().second()),
                                                   username));
    if(que.exec())
        return true;
    else{
        qDebug() << que.lastError().databaseText();
        return false;
    }
}

QDateTime database::getBanTime(QString username)
{
    QDateTime retTime;
    QSqlQuery que;
    que.prepare(QString("SELECT Year(banend), Month(banend), Day(banend),"
                        "Hour(banend), Minute(banend), Second(banend)"
                        "FROM users "
                        "WHERE users.username = \"%1\"").arg(username));
    if(que.exec()){
        que.last();
        retTime.setDate(QDate(que.value(0).toInt(),que.value(1).toInt(),que.value(2).toInt()));
        retTime.setTime(QTime(que.value(3).toInt(),que.value(4).toInt(),que.value(5).toInt()));
        return (retTime);
    }else{
        qDebug() << "Exec failed on banTime";
        return QDateTime(QDate(0,0,0),QTime(0,0));
    }
}

QDateTime database::getMuteTime(QString username)
{
    QDateTime retTime;
    QSqlQuery que;
    que.prepare(QString("SELECT Year(muteend), Month(muteend), Day(muteend),"
                        "Hour(muteend), Minute(muteend), Second(muteend)"
                        "FROM users "
                        "WHERE users.username = \"%1\"").arg(username));
    if(que.exec()){
        que.last();
        retTime.setDate(QDate(que.value(0).toInt(),que.value(1).toInt(),que.value(2).toInt()));
        retTime.setTime(QTime(que.value(3).toInt(),que.value(4).toInt(),que.value(5).toInt()));
        return (retTime);
    }else{
        qDebug() << "Exec failed on muteTime";
        return QDateTime(QDate(0,0,0),QTime(0,0));
    }
}

bool database::isBanned(QString username)
{
    QSqlQuery que;
    que.prepare(QString("SELECT banned "
                        "FROM users "
                        "WHERE users.username = \"%1\"").arg(username));
    if(que.exec()){
        que.last();
        return (que.value(0).toBool());
    }else{
        qDebug() << "Exec failed on isBan";
        return false;
    }

}

bool database::isMuted(QString username)
{

    QSqlQuery que;
    que.prepare(QString("SELECT muted "
                        "FROM users "
                        "WHERE users.username = \"%1\"").arg(username));
    if(que.exec()){
        que.last();
        return (que.value(0).toBool());
    }else{
        qDebug() << "Exec failed on isMute";
        return false;
    }
}

bool database::unMute(QString username)
{
    QSqlQuery que;
    que.prepare(QString("UPDATE users "
                        "SET muted = 0, muteend = NULL "
                        "WHERE users.username = \"%1\"").arg(username));
    if(que.exec()){
        return true;
    }else{
        qDebug() << "Exec failed on unMute";
        return false;
    }
}

bool database::unBan(QString username)
{
    QSqlQuery que;
    que.prepare(QString("UPDATE users "
                        "SET banned = 0, banend = NULL "
                        "WHERE users.username = \"%1\"").arg(username));
    if(que.exec()){
        return true;
    }else{
        qDebug() << "Exec failed on unBan";
        return false;
    }
}

