#include "unnoknfile.h"
#include "ui_unnoknfile.h"

UnNoknfile::UnNoknfile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnNoknfile)
{
    ui->setupUi(this);
}

UnNoknfile::~UnNoknfile()
{
    delete ui;
}

void UnNoknfile::on_pushButton_2_clicked()
{
    emit print();
}

void UnNoknfile::on_pushButton_3_clicked()
{
    emit parser();
}

void UnNoknfile::on_pushButton_clicked()
{
    emit cancle();
}
