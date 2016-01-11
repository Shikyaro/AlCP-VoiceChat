/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "audiooutput.h"
#include "../Server/sclient.h"
#include <QStringList>
#include <QCryptographicHash>
#include <QMap>
#include <QRegularExpression>
/*!
 * \class comm
 * \brief Класс описывает команду чата.
 * Используется для парсинга сообщений
 */
class comm
{
public:
    /*!
     * \fn comm::comm(quint8 c_id, uint arg_c)
     * Конструктор класса.
     * \param c_id
     * Id команды
     * \param arg_c
     * Количество аргументов команды
     */
    comm(quint8 c_id, uint arg_c){comm_id = c_id; arg_count = arg_c;}
    /*!
     * \fn comm::getCount
     * Функция, позволяющая получить количество аргументов команды
     * \return
     * Возвращает число типа uint
     */
    quint8 getId(){return comm_id;}

    /*!
     * \fn comm::getId
     * Функция, позволяющая получить id команды
     * \return
     * Возвращает число типа quint8
     */
    uint getCount(){return arg_count;}

private:
    quint8 comm_id; /*!< id of command*/
    uint arg_count; /*!< количество аргументов команды */
};
/*!
 * \class Client
 * \brief Основная логика клиента.
 */
class Client : public QObject
{
    Q_OBJECT
public:
    /*!
     * \fn Client::Client(QObject *parent = 0)
     * Конструктор класса Client.
     * \param parent
     * Родитель
     */
    explicit Client(QObject *parent = 0);
    /*!
     * \fn Client::connectToSrv(QString host, quint16 port)
     * Функция подключения к серверу
     * \param host
     * IP адрес хоста
     * \param port
     * Порт
     * \return
     * Возвращет значение типа bool.
     * Показывает успех/неудачу подключения
     */
    bool    connectToSrv(QString host, quint16 port);
    /*!
     * \fn Client::getCommMap()
     * Функция, позволяющая получить QMap с командами чата
     * \return
     * Возвращает значение типа QMap<QString,comm*>.
     */
    QMap<QString,comm*> getCommMap(){return commandMap;}
signals:
    /*!
     * \fn Client::succLogin()
     * Сигнал, отправляется при успешном входе на сервер
     */
    void    succLogin();
    /*!
     * \fn Client::unSuccLogin()
     *  Сигнал, оправляется при неудачном подключении
     */
    void    unSuccLogin();
    /*!
     * \fn Client::succReg()
     * Сигнал, отправляется при успешной регистрации
     */
    void    succReg();
    /*!
     * \fn Client::unSuccReg()
     *  Сигнал, оправляется при неудачной регистрации
     */
    void    unSuccReg();
    /*!
     * \fn Client::isBanned()
     *  Сигнал, оправляется если пользователь забанен на сервере
     */
    void    isBanned();
    /*!
     * \fn Client::newMessage
     * Сигнал, отправляется при полученнии нового сообщения
     * \param uname
     * Имя отправителя
     * \param msg
     * Сообщение
     * \param clr
     * Цвет сообщения
     */
    void    newMessage(QString uname, QString msg, QString clr);
    /*!
     * \fn Client::nUser
     * Сигнал, отправляется при входе пользователя на сервер
     * \param uname
     * Имя вошедшего пользователя
     */
    void    nUser(QString uname);
    /*!
     * \fn Client::dUser
     * Сигнал, отправляется при отключении пользователя от сервера
     * \param uname
     * Имя отключившегося пользователя
     */
    void    dUser(QString uname);
    /*!
     * \fn Client::userList
     * Сигнал, отправляется при подключении пользователя к серверу\n
     * Несёт QStringList с пользователями онлайн
     * \param usrs
     * Список пользователей онлайн
     */
    void    userList(QStringList usrs);
    /*!
     * \fn Client::disc
     * Сигнал, отправляется при отключении от сервера (разрыв подключения)
     */
    void    disc();
    /*!
     * \fn Client::errMess
     * Сигнал, отправляется при получении ошибки от сервера
     * \param err
     * Текст ошибки
     */
    void    errMess(QString err);

public slots:
    /*!
     * \fn Client::login
     * Слот, принимает сигнал от MainWindow.\n
     * Используется для входа на сервер
     * \param login
     * Логин
     * \param password
     * Пароль
     * \warning Рекомендуется использовать хешированный пароль
     */
    void    login(QString login, QString password);
    /*!
     * \fn Client::reg
     * Слот, принимает сигнал от MainWindow.\n
     * Используется для регистрации на сервере
     * \param login
     * Логин
     * \param password
     * Пароль
     * \warning Рекомендуется использовать хешированный пароль
     */
    void    reg(QString login, QString password);
    /*!
     * \fn Client::voiceSay
     * Слот, принимает сигнал от AudioInput\n
     * Используется для отправки звука на сервер
     * \param data
     * Кодированный звук
     */
    void    voiceSay(QByteArray data);
    /*!
     * \fn Client::readVoice
     * Слот, вызывается voiceSock \n
     * Используется для чтения звука из voiceSock
     */
    void    readVoice();
    /*!
     * \fn Client::stringParser
     * Слот, используется для парсинга сообщений чата
     * \param str
     * Текст сообщения
     */
    void    stringParser(QString str);
    /*!
     * \fn Client::setOutVol
     * Слот, принимает сигнал из MainWindow\n
     * Используется для изменения громкости
     * устройства воспроизведения звука
     * \param vol
     * Громкость
     */
    void    setOutVol(int vol);
    /*!
     * \fn Client::setAudio
     * Слот, принимает сигналы от MainWindow\n
     * Используется для включения/выключения
     * устройства воспроизведения звука
     * \param st
     * Состояние
     */
    void    setAudio(bool st);

private slots:
    void    readyRead(); //вызывается при появлении данных на AudioInput
    void    onDisconnect(); //вызывается при отключении от сервера

private:
    template <class dataBlock>
    void    sendBlock(quint8 command, dataBlock data); //отправка блока данных через главный сокет

    void    handleCommand(QString mess); //обработчик команд чата
    void    replaceSmiles(QString mess); //парсер, заменяющий коды смайлов на пути к ним в qrc

    void    addVoiceSock(); //добавление голосового сокета

    QString hashPW(QString msg); //хеширование пароля алгоритмом SHA3-256

    QTcpSocket  *socket;
    QTcpSocket  *voiceSock;
    AudioOutput output;
    quint16     blockSize;
    bool        isLoggedIn;

    QString     cHost;
    quint16     cPort;

    QString     userName;

    QMap<QString, comm*> commandMap;
    QMap<QString, QString> smilesMap;
};

#endif // CLIENT_H
