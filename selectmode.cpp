#include "selectmode.h"
#include "ui_selectmode.h"

selectMode::selectMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectMode)
{
    ui->setupUi(this);
}

selectMode::~selectMode()
{
    delete ui;
}

void selectMode::on_pushButton_5_clicked()
{
    emit m_whatThis();
}

void selectMode::on_pushButton_6_clicked()
{
    emit m_whatThis();
}

void selectMode::on_pushButton_4_clicked()
{
    emit m_back();
}
