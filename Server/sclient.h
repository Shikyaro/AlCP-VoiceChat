#ifndef SCLIENT_H
#define SCLIENT_H

#define lpsep QChar(0344)
#define mssep QChar(0345)

#define banpower int(49)
#define mutepower int(24)
#define kickpower int(24)

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QStringList>
#include <QRegularExpression>
#include "server.h"


class Server;
/*!
 * \class sClient
 * \brief Реализует обмен и обработку данных клиента.
 * Экземпляр этого класса создается для каждого
 * подключенного к серверу клиента.
 */
class sClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket  *socket;
    QTcpSocket  *voiceSock;
    Server      *server;
    quint16     blockSize;
    bool        isLoggedIn;

    QString     userName;
    bool        isMuted;

    bool        isValid(QString userName);
    void        sendOnline();
public:
    /*!
     * \fn sClient::sClient
     * Конструктор класса.\n
     * Инициализирует главный и голосовой сокеты.
     * \param descr
     * Дескриптор главного сокета.
     * Можно получить с помощью \code QTcpSocket::socketDescriptor() \endcode
     * \param serv
     * Указатель на объект класса Server
     * \param parent
     * Родитель
     */
    explicit sClient(qintptr descr, Server *serv, QObject *parent = 0);
    ~sClient();
    /*!
     * \brief Команда логина
     */
    static const quint8 c_login     = (quint8)1;
    /*!
     * \brief Команда успешного логина
     */
    static const quint8 c_SuccLogin = (quint8)201;
    /*!
     * \brief Команда неудачного логина
     */
    static const quint8 c_unSucc_L  = (quint8)202;
    /*!
     * \brief Команда голосового сообщения
     */
    static const quint8 c_voice_say = (quint8)2;
    /*!
     * \brief Команда регистрации
     */
    static const quint8 c_reg       = (quint8)3;
    /*!
     * \brief Команда успешной регистрации
     */
    static const quint8 c_Succ_Reg  = (quint8)203;
    /*!
     * \brief Команда неудачной регистрации
     */
    static const quint8 c_unSucc_R  = (quint8)204;
    /*!
     * \brief Команда текстового сообщения
     */
    static const quint8 c_message   = (quint8)4;
    /*!
     * \brief Команда блокировки аккаунта
     */
    static const quint8 c_ban       = (quint8)5;
    /*!
     * \brief Команда принудительного молчания
     */
    static const quint8 c_mute      = (quint8)6;
    /*!
     * \brief Команда изменения прав доступа
     */
    static const quint8 c_chperm    = (quint8)7;
    /*!
     * \brief Команда отмены блокировки
     */
    static const quint8 c_unban     = (quint8)9;
    /*!
     * \brief Команда отмены молчания
     */
    static const quint8 c_unmute    = (quint8)10;
    /*!
     * \brief Команда выкидывания из чата
     */
    static const quint8 c_kick      = (quint8)11;
    /*!
     * \brief Команда сообщения об ошибке
     */
    static const quint8 c_err_mess  = (quint8)12;
    /*!
     * \brief Команда подключения нового пользователя
     */
    static const quint8 c_userConn  = (quint8)13;
    /*!
     * \brief Команда отключения пользователя
     */
    static const quint8 c_userDisc  = (quint8)14;
    /*!
     * \brief Команда отправления списка пользователей онлайн
     */
    static const quint8 c_onList    = (quint8)15;

    /*!
     * \fn getName
     * Получение имени пользователя
     * \return
     * Имя пользователя типа QString
     */
    QString     getName()       {return userName;}
    /*!
     * \fn getLoggedIn
     * Получение статуса входа клиента
     * \return
     * True, если клиент вошел в учетную запись
     */
    bool        getLoggedIn()   {return isLoggedIn;}
    /*!
     * \fn getMuted
     * Получение статуса молчания
     * \return
     * True, если на пользователя наложено молчание
     */
    bool        getMuted()      {return isMuted;}
    /*!
     * \fn setMuted
     * Изменение статуса молчания
     * \param ism
     * Состояние
     */
    void        setMuted(bool ism) {isMuted = ism;}
    /*!
     * \fn sendBlock
     * Отправка блока данных на главный сокет
     * \param command
     * Команда
     * \param data
     * Блок данных
     */
    void        sendBlock(quint8 command, QByteArray data);
    /*!
     * \fn sendVoice
     * Отправка голоса на голосовой сокет
     * \param data
     * Кодированный звук
     */
    void        sendVoice(QByteArray data);
    /*!
     * \fn setVoiceSocket
     * Установка голосовго сокета
     * \param sock
     * Указатель на голосовой сокет
     */
    void        setVoiceSocket(QTcpSocket* sock);
    /*!
     * \fn kick
     * Выкинуть пользователя из чата
     */
    void        kick();
    /*!
     * \fn createError
     * Отправить пользователю сообщение об ошибке
     * \param errT
     * Текст ошибки
     */
    void        createError(QString errT);
signals:
    /*!
     * \fn userDisconnected
     * Сигнал, вызывается при отключении пользователя
     * \param client
     * Указатель на экземпляр класса sClient (отключившийся пользователь)
     */
    void        userDisconnected(sClient* client);
    /*!
     * \fn addVoiceSocket
     * Сигнал, вызывается после подключения пользователя
     * к серверу для присоединения дополнительного (голосового) сокета
     * \param cliToDel
     * Указатель на экземпляр класса sClient
     * \param username
     * Имя пользователя
     * \param sck
     * Указатель на голосовой сокет
     */
    void        addVoiceSocket(sClient* cliToDel, QString username, QTcpSocket* sck);
    /*!
     * \fn delDisc
     * Удаление отключенного клиента
     * \param cltd
     * Указатель на экземпляр класса sClient
     */
    void        delDisc(sClient* cltd);

private slots:
    void        onDisconnect();
    void        onReadyRead();
    void        onReadyVoice();
};

#endif // SCLIENT_H
