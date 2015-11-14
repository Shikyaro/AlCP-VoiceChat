#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Server *srv = new Server(6969,this);
}

MainWindow::~MainWindow()
{

}
