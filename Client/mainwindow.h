#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QLineEdit>
#include "audioinput.h"
#include "audiooutput.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPushButton *mbut;
    Client *client;
    AudioInput *input;
    QWidget *cw;
    QVBoxLayout *ml;
    QLineEdit *ql;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void bClicked();
public slots:
    void succLogin();
};

#endif // MAINWINDOW_H
