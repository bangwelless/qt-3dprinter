#include "parsetdlog.h"
#include "ui_parsetdlog.h"
#include <qdebug.h>

parsetdlog::parsetdlog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::parsetdlog)
{
    ui->setupUi(this);
    ui->pushButton_14->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    m_timer = new QTimer(this);
    QObject::connect(m_timer,&QTimer::timeout,this,&parsetdlog::timeou);
    m_timer->start(500);
}

parsetdlog::~parsetdlog()
{
    delete ui;
}

void parsetdlog::on_pushButton_12_clicked()
{
    ui->pushButton_14->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_14->setChecked(false);
    ui->pushButton_15->setChecked(false);
    emit next();
    ui->stackedWidget->setCurrentIndex(1);

}

void parsetdlog::on_pushButton_clicked()
{
    if(ui->pushButton_11->isChecked())
    {
        emit complete(0,1);
    }
    else {
        emit complete(0,0);
    }
    emit active(ui->pushButton_17->isChecked());

}

void parsetdlog::on_pushButton_16_clicked()
{

    if(ui->pushButton_14->isChecked()&&ui->pushButton_15->isChecked())
    {
        if(ui->pushButton_11->isChecked())
        {
            emit complete(0,1);
        }
        else {
            emit complete(0,0);
        }
        emit active(ui->pushButton_17->isChecked());
    }
    else
    {
        if(!ui->pushButton_14->isChecked()&&!ui->pushButton_15->isChecked())
        {
            if(ui->pushButton_11->isChecked())
            {
                emit complete(0,1);
            }
            else {
                emit complete(0,0);
            }
            emit active(ui->pushButton_17->isChecked());
        }
        else
        {
            if(ui->pushButton_11->isChecked())
            {
                emit complete(0,1);
            }
            else {
                emit complete(0,0);
            }
            emit excomplete(ui->pushButton_14->isChecked(),ui->pushButton_15->isChecked(),ui->pushButton_17->isChecked());
            emit active(ui->pushButton_17->isChecked());
            qDebug()<<"1";
        }

    }

}

void parsetdlog::on_pushButton_14_clicked(bool checked)
{
    if(!checked)
    {
        ui->pushButton_15->setChecked(true);
    }
}

void parsetdlog::on_pushButton_15_clicked(bool checked)
{
    if(!checked)
    {
        ui->pushButton_14->setChecked(true);
    }
}

void parsetdlog::seten(bool l, bool r)
{
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_15->setEnabled(true);
        ui->pushButton_14->setChecked(l);
        ui->pushButton_15->setChecked(r);
}

void parsetdlog::initnum(QString a, QString b, QString c, QString d, QString e, QString f, QString g)
{
    ui->pushButton_4->setText("Left Temp / "+a+"°C");
    ui->pushButton_5->setText("Right Temp / "+b+"°C");

    ui->pushButton_6->setText("Left Fan Speed "+d+"%");
    ui->pushButton_7->setText("Right Fan Speed "+e+"%");

    ui->pushButton_8->setText("Bed Temp "+c+"°C");

    ui->pushButton_9->setText("Feedrate "+f+"%");
    ui->pushButton_10->setText("Platform Height "+g+"mm");

}

void parsetdlog::backup(int a )
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
    qDebug()<<"log"<<a;
}

void parsetdlog::backupt(int a )
{
    switch (a) {
    case 0:
        ui->pushButton_17->setChecked(false);
        break;
    case 1:
        ui->pushButton_17->setChecked(true);
        break;
    case 2:
        ui->pushButton_17->setEnabled(false);
        break;
    default:
        break;

    }
    qDebug()<<"log"<<a;
}

void parsetdlog::timeou()
{
    m_timer->stop();
    emit down();
    QObject::disconnect(m_timer,&QTimer::timeout,this,&parsetdlog::timeou);
    QObject::connect(m_timer,&QTimer::timeout,this,&parsetdlog::time2);
    m_timer->start(500);
}

void parsetdlog::time2()
{
    m_timer->stop();
    emit up();
    QObject::disconnect(m_timer,&QTimer::timeout,this,&parsetdlog::time2);
}

void parsetdlog::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_14->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_14->setChecked(false);
    ui->pushButton_15->setChecked(false);
}

void parsetdlog::on_pushButton_11_clicked()
{

}

void parsetdlog::on_pushButton_17_clicked()
{

}

void parsetdlog::on_pushButton_2_clicked()
{
    emit up();
}

void parsetdlog::on_pushButton_3_clicked()
{
    emit down();
}
