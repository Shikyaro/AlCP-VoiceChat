/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QtCore>
#include <QtMultimedia>

/*!
 * \class AudioOutput
 * \brief Класс, реализующий воспроизведение звука.
 */
class AudioOutput : public QObject
{
    Q_OBJECT
public:
    /*!
     * \fn AudioOutput::AudioOutput
     * Конструктор класса
     * \param parent
     * Родитель
     */
    explicit AudioOutput(QObject *parent = 0);
    /*!
     * \fn AudioOutput::setVol
     * Функция настройки громкости вывода звука
     * \param vol
     * Громкость
     */
    void setVol(qreal vol){audio->setVolume(vol);}
    /*!
     * \fn AudioOutput::getenb
     * Функция для проверки, включён ли сейчас звук
     * \return
     * Возвращает значение типа bool - включен/выключен true/false
     */
    bool getenb(){return enbed;}

signals:

public slots:
    /*!
     * \fn AudioOutput::writeData
     * Слот, вызывается для вывода звука
     * \param data
     * Кодированный звук
     */
    void writeData(QByteArray data);
    /*!
     * \fn AudioOutput::setAudio
     * Слот, вызывается для включения/выключения звука
     * \param st
     * Состояние
     */
    void setAudio(bool st);

private:
    QAudioOutput *audio;
    QIODevice *device;
    qreal svol = 1;
    bool enbed;
};

#endif // AUDIOOUTPUT_H
