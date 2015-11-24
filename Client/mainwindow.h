#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QLineEdit>
#include <QListWidget>
#include "audioinput.h"
#include "audiooutput.h"
#include "logindialog.h"
#include <QTextBrowser>
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Client *client;
    AudioInput *input;
    QWidget *cw;
    QListWidget *userWidget;
    QHBoxLayout *ml;
    QGridLayout *mlay;
    LoginDialog *ldialog;
    QTextBrowser *chatWidget;

    QLineEdit *chatLine;
    QPushButton *chatBut;


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void sendMessage();
public slots:
    void succLogin();
    void newMessage(QString username, QString message, QString col);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
