#include "mainwindow.h"
#include "client.h"
#include "audioinput.h"
#include "audiooutput.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
        paths.append(".");
        paths.append("plugins");
        paths.append("platforms");
        QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);

    QAudioDeviceInfo devinfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(0);

    Client *client = new Client(this);
    AudioInput *input = new AudioInput(devinfo,this);
    AudioOutput *output = new AudioOutput(this);





    MainWindow w;
    w.show();

    return a.exec();
}
