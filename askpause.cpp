#include "askpause.h"
#include "ui_askpause.h"

askPause::askPause(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::askPause)
{
    ui->setupUi(this);
}

askPause::~askPause()
{
    delete ui;
}

void askPause::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    emit m_stop();
}

void askPause::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    emit m_paused();
}

void askPause::on_pushButton_3_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    emit cancel();
}
