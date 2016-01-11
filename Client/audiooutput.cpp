/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#include "audiooutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{
    QAudioFormat format;

    format.setChannelCount(1);
    format.setSampleRate(16000);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    audio = new QAudioOutput(format, this);

    //audio->setBufferSize(8192);

    device = audio->start();
    audio->setVolume(1);
}

void AudioOutput::writeData(QByteArray data)
{
    device->write(data.data(), data.size());
}

void AudioOutput::setAudio(bool st)
{
    if(st){
        audio->setVolume(svol);
    enbed = true;
    }
    else
    {
        svol=audio->volume();
        audio->setVolume(0);
        enbed = false;
    }
}
