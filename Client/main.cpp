/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    /*QStringList paths = QCoreApplication::libraryPaths();
        paths.append(".");
        paths.append("plugins");
        paths.append("platforms");
        paths.append("imageformats");
        QCoreApplication::setLibraryPaths(paths);*/

    QApplication a(argc, argv);

    //a.addLibraryPath(".");
    a.addLibraryPath("/plugins");
    //a.addLibraryPath("./plugins/platforms");


    MainWindow w;
    w.show();

    return a.exec();
}
