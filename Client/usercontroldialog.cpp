/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#include "usercontroldialog.h"

userControlDialog::userControlDialog(QString username, QMap<QString, comm *> cMap, QWidget *parent) : QWidget(parent)
{
    uname = username;

    commBox = new QComboBox();

    commLen = new QLineEdit();

    goBut = new QPushButton("Ок");
    cancelBut = new QPushButton("Отмена");

    umwlay = new QGridLayout();
    this->setLayout(umwlay);

    umwlay->addWidget(new QLabel("Имя пользователя: "),0,0);
    umwlay->addWidget(new QLabel(username),0,1);

    umwlay->addWidget(new QLabel("Выберите действие: "),1,0);
    umwlay->addWidget(commBox,1,1);

    umwlay->addWidget(new QLabel("Длина: "),2,0);
    umwlay->addWidget(commLen,2,1);

    umwlay->addWidget(cancelBut,3,0);
    umwlay->addWidget(goBut,3,1);

    QMapIterator<QString, comm*> it(cMap);
    while(it.hasNext()){
        it.next();
        commBox->addItem(it.key());
    }

    connect(goBut, SIGNAL(clicked(bool)), this, SLOT(sendMess()));
    connect(cancelBut, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void userControlDialog::sendMess()
{
    QString prepMess;

    prepMess.append(commBox->currentText()).append(" ").append(uname).append(" ").append(commLen->text());

    qDebug() << prepMess;

    emit this->sendComm(prepMess);
    this->close();
}

