#include "smilebutton.h"

smileButton::smileButton(QString smI, QString smT, QWidget *parent) : QPushButton(parent)
{
    smileText = smT;

    this->setIcon(QIcon(tr(":/smiles/%1").arg(smI)));

    connect(this,SIGNAL(clicked(bool)),this,SLOT(onSmClicked()));

    this->setFixedSize(25,25);

    this->setIconSize(QSize(25,25));
}



void smileButton::onSmClicked()
{
    emit this->smButClicked(this->smileText);
}
