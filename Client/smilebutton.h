/*************************/
/*  Автор: Романов Павел */
/*     Группа: П-304     */
/*************************/
#ifndef SMILEBUTTON_H
#define SMILEBUTTON_H

#include <QWidget>
#include <QPushButton>

class smileButton : public QPushButton
{
    Q_OBJECT
public:
    explicit smileButton(QString smI, QString smT, QWidget *parent = 0);

    QString getSmileText(){return smileText;}

signals:

    void smButClicked(QString smtext);

public slots:
private slots:
    void onSmClicked();

private:
    QString smileText;
};

#endif // SMILEBUTTON_H
