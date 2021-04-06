#include "filamentfault.h"
#include "ui_filamentfault.h"

filamentFault::filamentFault(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::filamentFault)
{
    ui->setupUi(this);
}

filamentFault::~filamentFault()
{
    delete ui;
}

void filamentFault::on_pushButton_clicked()
{
    emit resume();
}

void filamentFault::on_pushButton_3_clicked()
{
    emit changeFilament();
}

void filamentFault::on_pushButton_2_clicked()
{
    emit abort();
}
