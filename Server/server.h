/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include <QDebug>
#include <QTimer>
#include "sclient.h"
#include "database.h"

class sClient;
/*!
 * \brief Класс сервера.
 * Реализует взаимодействие клиентов между собой
 * с использованием экземпляров sClient.
 */
class Server : public QTcpServer
{
    Q_OBJECT
    friend class sClient;
private:
    QList<sClient *> cliList;
    database *db;
    QTimer *muteBanTimer;

public:
    /*!
     * Конструктор класса.
     * Инициализирет сервер и БД
     * \param port
     * Порт, который будет слушать сервер
     * \param parent
     * Родитель
     */
    explicit Server(quint16 port, QObject *parent = 0);
    ~Server();
    /*!
     * Запускает сервер
     * \param port
     * Порт, который будет слушать сервер
     * \return
     * True в случае успеха
     */
    bool     startServer(quint16 port);
    /*!
     * Отправка сообщения всем клиентам.\n
     * Есть возможность отправить сообщение всем кроме отправителя
     * \param command
     * Команда
     * \param data
     * Блок данных
     * \param senderName
     * Имя отправителя
     * \param exceptSender
     * Игнорируется ли отправитель
     */
    void     sendToAll(quint8 command, QByteArray data, QString senderName, bool exceptSender);
    /*!
     * Отправка голосового сообщения всем
     * \param voice
     * Кодированный звук
     * \param senderName
     * Имя отправителя
     */
    void     sendVoiceToAll(QByteArray voice, QString senderName);
    /*!
     * Отправка списка пользователей онлайн клиенту
     * \param cli
     * Указатель на объект sClient
     * \return
     * Список пользователей, разделенный запятыми
     */
    QString  sendOnline(sClient* cli);
    /*!
     * Наложить молчание на пользователя
     * \param username
     * Имя пользователя
     * \param secs
     * Время в секундах
     */
    void     mute(QString username, long long secs);
    /*!
     * Наложить блокировку на пользователя
     * \param username
     * Имя пользователя
     * \param secs
     * Время в секундах
     */
    void     ban(QString username, long long secs);
    /*!
     * Снять молчание с пользователя
     * \param username
     * Имя пользователя
     */
    void     unmute(QString username);
    /*!
     * Снять блокировку с пользователя
     * \param username
     * Имя пользователя
     */
    void     unban(QString username);
    /*!
     * Выкинуть пользователя из чата
     * \param username
     * Имя пользователя
     */
    void     kick(QString username);
    /*!
     * Отправить серверное сообщение всем клиентам
     * \param msg
     * Текст сообщения
     */
    void     serverMessage(QString msg);

protected:
    /*!
     * Переопределенная функция QTcpServer::incomingConnection(qintptr handle).\n
     * Используется для создания экземпляра sClient каждому подключению.
     * \param handle
     * Дескриптор сокета
     */
    void    incomingConnection(qintptr handle);

signals:

public slots:
    /*!
     * Слот, вызывается при отключении клиента от сервера
     * \param client
     * Указатель на экземпляр sClient
     */
    void    onUserDisconnected(sClient *client);
    /*!
     * Слот, вызывается при добавлении голосового сокета
     * \param cliToDel
     * Указатель на экземпляр sClient
     * \param username
     * Имя пользователя
     * \param sck
     * Указатель на голосовой сокет
     */
    void    onVoiceSocket(sClient* cliToDel, QString username, QTcpSocket* sck);

private slots:
    void    checkMute();
};

#endif // SERVER_H
