#include "delete.h"
#include "ui_delete.h"

Delete::Delete(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Delete)
{
    ui->setupUi(this);
    }

Delete::~Delete()
{
    delete ui;
}

void Delete::setName()
{
    ui->label_2->setText(fileName);
    ui->label_2->setAlignment(Qt::AlignCenter);
}

void Delete::on_pushButton_clicked()
{
    emit checkDelete(deleteItem);
}

void Delete::on_pushButton_2_clicked()
{
    emit checkCancel(deleteItem);
}
