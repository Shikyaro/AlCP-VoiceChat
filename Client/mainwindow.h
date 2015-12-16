#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QLineEdit>
#include <QListWidget>
#include "audioinput.h"
#include "audiooutput.h"
#include "logindialog.h"
#include <QTextBrowser>
#include <QGroupBox>
#include <QSlider>
#include <QComboBox>
#include <smilebutton.h>
#include <usercontroldialog.h>

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

    QGroupBox *audSets;
    QGridLayout *auSetLay;
    QSlider *speakVol;
    QSlider *microVol;

    QGridLayout *smilesLay;


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void sendMessage();
public slots:
    void succLogin();
    void newMessage(QString username, QString message, QString col);
    void newUser(QString username);
    void userDisc(QString username);
    void drawUserList(QStringList ulist);
    void onDisc();
    void addSm(QString smt);

    void ShowUserContMenu(const QPoint &cmenu);
    void showError(QString err);

    //bool eventFilter(QObject *obj, QEvent *event);
protected:
};

#endif // MAINWINDOW_H
