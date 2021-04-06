#include "xhwizard.h"
#include "ui_xhwizard.h"

XhWizard::XhWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XhWizard)
{
    ui->setupUi(this);
}

XhWizard::~XhWizard()
{
    delete ui;
}

void XhWizard::on_pushButton_clicked()
{
    emit confirm();
}

void XhWizard::on_pushButton_2_clicked()
{
    emit cancel();
}
