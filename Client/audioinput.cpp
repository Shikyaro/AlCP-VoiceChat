#include "audioinput.h"

AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent) : QObject(parent)
{
    QAudioFormat format;

    format.setChannelCount(1);
    format.setSampleRate(16000);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    audio = new QAudioInput(devinfo, format, this);
    //audio->setBufferSize(8192);
    device = audio->start();
    audio->setVolume(1);
    qDebug() << devinfo.supportedByteOrders();
    connect(device, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void AudioInput::readyRead()
{
    QByteArray data;

    qint64 len = audio->bytesReady();

    if (len > 0)
    {
        data.resize(len);
        device->read(data.data(), len);
        emit dataReady(data);
    }   
}

void AudioInput::setMicVol(int vol)
{
    qreal retv = vol;

    retv=retv/100;

    audio->setVolume(retv);

}
