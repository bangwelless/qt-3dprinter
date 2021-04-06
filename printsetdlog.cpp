#include "printsetdlog.h"
#include "ui_printsetdlog.h"

PrintSetDlog::PrintSetDlog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintSetDlog)
{
    ui->setupUi(this);


}

PrintSetDlog::~PrintSetDlog()
{
    delete ui;
}

void PrintSetDlog::initnum(QString a, QString b, QString c, QString d, QString e, QString f, QString g)
{

        ui->pushButton_4->setText("Left Temp / "+a+"°C");
        ui->pushButton_5->setText("Right Temp / "+b+"°C");

        ui->pushButton_6->setText("Left Fan Speed "+d+"%");
        ui->pushButton_7->setText("Right Fan Speed "+e+"%");

        ui->pushButton_8->setText("Bed Temp "+c+"°C");

        ui->pushButton_9->setText("Feedrate "+f+"%");
        ui->pushButton_10->setText("Platform Height "+g+"mm");


}

void PrintSetDlog::backup(int a )
{
    switch (a) {
    case 0:
        ui->pushButton_11->setChecked(false);
        break;
    case 1:
        ui->pushButton_11->setChecked(true);
        break;
    case 2:
        ui->pushButton_11->setEnabled(false);
        break;
    default:
        break;

    }
}
void PrintSetDlog::on_pushButton_clicked()
{
    if(ui->pushButton_11->isChecked())
    {
        emit closeset(1);
    }
    else {
        emit closeset(0);
    }
}
