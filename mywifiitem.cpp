#include "mywifiitem.h"
#include "ui_mywifiitem.h"

myWifiItem::myWifiItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myWifiItem)
{
    ui->setupUi(this);
}

myWifiItem::~myWifiItem()
{
    delete ui;
}

void myWifiItem::addwifi(QString name, QString level)
{
    wifiname = name ;
    wifilevel = level;
    ui->pushButton->setText(name);
}

void myWifiItem::on_pushButton_clicked()
{
    emit conncetwifi(this);
}
