#include "printflament.h"
#include "ui_printflament.h"

printFlament::printFlament(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::printFlament)
{
    ui->setupUi(this);
}

printFlament::~printFlament()
{
    delete ui;
}

void printFlament::init(QString str)
{
    int num = str.mid(0,3).toInt();
    switch (num) {
//    case 0:
//        ui->pushButton_6->setStyleSheet("QPushButton{color: rgb(255, 255, 255);\
//                                        font-size: 36px;\
//                text-align: center;\
//        font-family: Barlow;\
//        background-image: url(:/TempofButton.png);\
//border:none;\
//    };");
//    break;
    case 210:
        ui->pushButton->setStyleSheet("QPushButton{color: rgb(255, 255, 255);\
                                      font-size: 36px;\
                text-align: center;\
        font-family: Barlow;\
        background-image: url(:/TemponButton.png);\
border:none;\
};");
break;
case 220:
ui->pushButton_2->setStyleSheet("QPushButton{color: rgb(255, 255, 255);\
                                font-size: 36px;\
text-align: center;\
font-family: Barlow;\
background-image: url(:/TemponButton.png);\
border:none;\
};");
break;
case 230:
ui->pushButton_3->setStyleSheet("QPushButton{color: rgb(255, 255, 255);\
                                font-size: 36px;\
text-align: center;\
font-family: Barlow;\
background-image: url(:/TemponButton.png);\
border:none;\
};");
break;
case 240:
ui->pushButton_4->setStyleSheet("QPushButton{color: rgb(255, 255, 255);\
                                font-size: 36px;\
text-align: center;\
font-family: Barlow;\
background-image: url(:/TemponButton.png);\
border:none;\
};");
break;
case 250:
ui->pushButton_5->setStyleSheet("QPushButton{color: rgb(255, 255, 255);\
                                font-size: 36px;\
text-align: center;\
font-family: Barlow;\
background-image: url(:/TemponButton.png);\
border:none;\
};");
break;
default:
break;


}
}

void printFlament::on_pushButton_clicked()
{
    emit heatT(ui->pushButton->text());
}

void printFlament::on_pushButton_2_clicked()
{
    emit heatT(ui->pushButton_2->text());
}

void printFlament::on_pushButton_3_clicked()
{
    emit heatT(ui->pushButton_3->text());
}

void printFlament::on_pushButton_4_clicked()
{
    emit heatT(ui->pushButton_4->text());
}

void printFlament::on_pushButton_5_clicked()
{
    emit heatT(ui->pushButton_5->text());
}
