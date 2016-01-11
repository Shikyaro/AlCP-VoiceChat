/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
/*!
 * \class database
 * \brief Интерфейс взаимодействия с БД
 * Класс реализует интерфейс взаимодействия с
 * базой данных MySQL.\n
 * Хранит SQL запросы к БД.
 * \warning класс Server является дружественным данному классу
 */
class database : public QObject
{
    friend class Server;
    Q_OBJECT
public:
    /*!
     * \fn database::database
     * Конструктор класса
     * \param parent
     * Родитель
     */
    explicit database(QObject *parent = 0);
    /*!
     * \fn database::initialize
     * Подключение к удаленной базе данных
     * \param hostname
     * IP адрес хоста
     * \param dbname
     * Имя базы данных
     * \param username
     * Имя пользователя
     * \param password
     * Пароль
     * \return
     * Успех или неудачу подключения
     */
    bool initialize(QString hostname, QString dbname, QString username, QString password);
    /*!
     * \fn database::getPower
     * Получение параметра "силы" пользователя
     * \param userName
     * Имя пользователя
     * \return
     * Значение "силы" пользователя типа uint.
     */
    uint getPower(QString userName);
    /*!
     * \fn database::getPower
     * Получение параметра минимальной "силы" команды
     * требуемой для использования
     * \param comName
     * Название команды
     * \return
     * Значение минимальной "силы" команды типа uint.
     */
    uint getComPower(QString comName);
    /*!
     * \fn database::authorize
     * Проверяет соответствие пары логина/пароля
     * в ьазе данных
     * \param userName
     * Имя пользователя
     * \param password
     * Пароль
     * \return
     * True, если такое сочетание есть, false, если нет.
     */
    bool authorize(QString userName, QString password);
    /*!
     * \fn database::newUser
     * Регистрирует нового пользователя в БД
     * \param userName
     * Имя пользователя
     * \param password
     * Пароль
     * \return
     * True, если пользователь был зарегистрирован, false, если нет.
     */
    bool newUser(QString userName, QString password);
    /*!
     * \fn database::setPerm
     * Изменяет группу прав доступа пользователя
     * \param username
     * Имя пользователя
     * \param perms
     * Новая группа прав
     * \return
     * True в случае успеха.
     */
    bool setPerm(QString username, uint perms);
    /*!
     * \fn database::ban
     * Накладывает временную блокировку на аккаунт пользователя
     * \param username
     * Имя пользователя
     * \param bantime
     * Дата и время окончания бана
     * \return
     * True в случае успеха.
     */
    bool ban(QString username, QDateTime bantime);
    /*!
     * \fn database::mute
     * Накладывает временное молчание на аккаунт пользователя
     * \param username
     * Время пользователя
     * \param mutetime
     * Дата и время окончания молчания
     * \return
     * True в случае успеха.
     */
    bool mute(QString username, QDateTime mutetime);
    /*!
     * \fn database::isBanned
     * Проверяет, забанен ли пользователь
     * \param username
     * Имя пользователя
     * \return
     * True, если забанен, false, если нет.
     */
    bool isBanned(QString username);
    /*!
     * \fn database::isMuted
     * Проверяет, наложен ли mute на пользователя
     * \param username
     * Имя пользователя
     * \return
     * True, если наложен, false, если нет.
     */
    bool isMuted(QString username);
    /*!
     * \fn database::unMute
     * Снимает mute с пользователя
     * \param username
     * Имя пользователя
     * \return
     * True в случае успеха.
     */
    bool unMute(QString username);
    /*!
     * \fn database::unBan
     * Снимает ban с пользователя
     * \param username
     * Имя пользователя
     * \return
     * True в случае успеха.
     */
    bool unBan(QString username);
    /*!
     * \fn database::getBanTime
     * Получает дату и время окончания бана пользователя
     * \param username
     * Имя пользователя
     * \return
     * Дату и время окончания бана пользователя
     */
    QDateTime getBanTime(QString username);
    /*!
     * \fn database::getMuteTime
     * Получает дату и время окончания mute пользователя
     * \param username
     * Имя пользователя
     * \return
     * Дату и время окончания mute пользователя
     */
    QDateTime getMuteTime(QString username);
    /*!
     * \fn database::getColor
     * Получает цвет, которым выделяются сообщения пользователя
     * \param userName
     * Имя пользователя
     * \return
     * Цвет, которым выделяются сообщения пользователя
     */
    QString getColor(QString userName);
    ~database();

signals:

public slots:

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
