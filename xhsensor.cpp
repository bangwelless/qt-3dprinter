#include "xhsensor.h"
#include "ui_xhsensor.h"

XhSensor::XhSensor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XhSensor)
{
    ui->setupUi(this);
}

XhSensor::~XhSensor()
{
    delete ui;
}

void XhSensor::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    emit confirm();
}

void XhSensor::on_pushButton_2_clicked()
{
    emit cancel();
}
