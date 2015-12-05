#include "audioinput.h"

AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    audio = new QAudioInput(devinfo, format, this);
    //audio->setBufferSize(8192);

    device = audio->start();
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
