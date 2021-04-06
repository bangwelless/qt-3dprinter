#include "dupandmirorr.h"
#include "ui_dupandmirorr.h"
#include <QDebug>

int DupandMirorr::num = 0;

DupandMirorr::DupandMirorr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DupandMirorr)
{
    ui->setupUi(this);
    num ++;
    qDebug()<<"make "<<num;

}

DupandMirorr::~DupandMirorr()
{
    qDebug()<<"remake"<<num;
    delete ui;
}

void DupandMirorr::changeMode(int n)
{
    switch (n) {
    case 1 :
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_3->setStyleSheet("QPushButton{font-weight: bold;font-family:Barlow;border-radius: 20px;\
                                        background-color: rgba(45, 44, 43, 255);\
                                        color: rgba(255, 255, 255, 255);\
                                        font-size: 36px;\
                                        text-align: center;\
                                        font-family: Barlow;\
                                        border: 2px solid rgba(45, 44, 43, 255);};");
        ui->pushButton_4->setStyleSheet("QPushButton{font-weight: bold;font-family:Barlow;border-radius: 20px;\
                                        background-color: rgba(45, 44, 43, 255);\
                                        color: rgba(255, 255, 255, 255);\
                                        font-size: 36px;\
                                        text-align: center;\
                                        font-family: Barlow;\
                                        border: 2px solid rgba(45, 44, 43, 255);};");

        break;
    case 3:
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_4->setStyleSheet("QPushButton{font-weight: bold;font-family:Barlow;border-radius: 20px;\
                                    background-color: rgba(45, 44, 43, 255);\
                                    color: rgba(255, 255, 255, 255);\
                                    font-size: 36px;\
                                    text-align: center;\
                                    font-family: Barlow;\
                                    border: 2px solid rgba(45, 44, 43, 255);};");
        break;
    default:
        break;

    }
}

void DupandMirorr::on_pushButton_2_clicked()
{
    emit print();
}

void DupandMirorr::on_pushButton_clicked()
{
    emit cancle();
}

void DupandMirorr::on_pushButton_3_clicked()
{
    emit dup();
}

void DupandMirorr::on_pushButton_4_clicked()
{
    emit mirro();
}
