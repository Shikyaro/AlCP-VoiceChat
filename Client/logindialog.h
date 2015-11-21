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

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);

signals:
    void s_log(QString log, QString pass);
    void s_reg(QString log, QString pass);
    void quit();

public slots:
    void onUnSuccLogin();
    void onUnSuccReg();

private:
    QTabWidget  *mainWid;

    QPushButton *log;
    QPushButton *reg;
    QPushButton *exit;

    QLineEdit   *login;
    QLineEdit   *pass;
private slots:
    void onReg();
    void onLog();
    void _onExit();

};

#endif // LOGINDIALOG_H
