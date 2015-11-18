#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);
    bool initialize(QString hostname, QString dbname, QString username, QString password);
    uint getPower(QString userName);
    ~database();

signals:

public slots:

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
