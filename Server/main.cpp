/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
