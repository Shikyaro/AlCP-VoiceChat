#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMessageBox>
#include "client.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(Client *cli, QWidget *parent = 0);

signals:
    void s_log(QString log, QString pass);
    void s_reg(QString log, QString pass);
    void quit();

public slots:
    void onUnSuccLogin();
    void onUnSuccReg();
    void onBan();

private:
    QTabWidget  *mainWid;

    QPushButton *log;
    QPushButton *reg;
    QPushButton *exit;

    QLineEdit   *login;
    QLineEdit   *pass;
    QLineEdit   *ip;
    QLineEdit   *port;

    bool        isConnected;
    Client      *client;
private slots:
    void onReg();
    void onLog();
    void _onExit();

};

#endif // LOGINDIALOG_H
