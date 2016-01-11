/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtCore>
#include <QtMultimedia>
#include "client.h"
/*!
 * \class AudioInput
 * \brief Класс, реализующий считывание звука с устройства ввода.
 */
class AudioInput : public QObject
{
    Q_OBJECT
public:
    /*!
     * \fn AudioInput::AudioInput
     * Конструктор класса
     * \param devinfo
     * Данные, о выбраном для записи устройстве ввода
     * \param parent
     * Родитель
     */
    explicit AudioInput(QAudioDeviceInfo devinfo, QObject *parent = 0);
    /*!
     * \fn AudioInput::start
     * Функция для начала записи данных
     */
    void start(){audio->start();}
    /*!
     * \fn AudioInput::stop
     * Функция для окончания записи данных
     */
    void stop(){audio->stop();}

signals:
    /*!
     * \fn AudioInput::dataReady
     * Сигнал, отправляет считанные с устройства записи звука данные
     * \param data
     * Кодированный звук
     */
    void dataReady(QByteArray data);

public slots:
    /*!
     * \fn AudioInput::setMicVol
     * Слот, используется для изменения громкости звука при записи
     * \param vol
     * Громкость
     */
    void setMicVol(int vol);
    /*!
     * \fn AudioInput::setAudio
     * Слот, используется для включения/выключения записи
     * \param st
     * Состояние
     */
    void setAudio(bool st);

private slots:
    void readyRead();

private:
    QAudioInput *audio;
    QIODevice *device;
    qreal svol = 1;
};

#endif // AUDIOINPUT_H
