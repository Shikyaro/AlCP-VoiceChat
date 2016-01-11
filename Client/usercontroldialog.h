/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#ifndef USERCONTROLDIALOG_H
#define USERCONTROLDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QListWidget>
#include <QMap>
#include <QLabel>
#include "client.h"

class userControlDialog : public QWidget
{
    Q_OBJECT
public:
    explicit userControlDialog(QString username, QMap<QString, comm*> cMap, QWidget *parent = 0);

signals:
    void sendComm(QString cmes);

public slots:
private slots:
    void sendMess();
private:
    QComboBox *commBox;
    QLineEdit *commLen;

    QMap<QString, comm*> cMap;

    QPushButton *goBut;
    QPushButton *cancelBut;

    QGridLayout *umwlay;

    QString uname;
};

#endif // USERCONTROLDIALOG_H
