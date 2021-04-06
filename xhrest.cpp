#include "xhrest.h"
#include "ui_xhrest.h"

XhRest::XhRest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XhRest)
{
    ui->setupUi(this);
}

XhRest::~XhRest()
{
    delete ui;
}

void XhRest::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    emit confirm();
}

void XhRest::on_pushButton_2_clicked()
{
    emit cancel();
}
