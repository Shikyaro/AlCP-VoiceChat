#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include "server.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Server *srv;
    QPushButton *startSrv;
    QLineEdit *portLine;
    QLabel *status;

private slots:
    void onStartSrv();
};

#endif // MAINWINDOW_H
