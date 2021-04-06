#include "duponly.h"
#include "ui_duponly.h"

duponly::duponly(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::duponly)
{
    ui->setupUi(this);
}

duponly::~duponly()
{
    delete ui;
}

void duponly::on_pushButton_2_clicked()
{
    emit onlyprint();
}

void duponly::on_pushButton_3_clicked()
{
    emit dup();
}

void duponly::on_pushButton_clicked()
{
    emit cancle();
}
