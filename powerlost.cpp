#include "powerlost.h"
#include "ui_powerlost.h"

powerLost::powerLost(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::powerLost)
{
    ui->setupUi(this);
}

powerLost::~powerLost()
{
    delete ui;
}

void powerLost::on_pushButton_2_clicked()
{
    emit cancel();
}
