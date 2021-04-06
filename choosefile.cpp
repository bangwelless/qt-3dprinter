#include "choosefile.h"
#include "ui_choosefile.h"

chooseFile::chooseFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chooseFile)
{
    ui->setupUi(this);
}

chooseFile::~chooseFile()
{
    delete ui;
}

void chooseFile::setFileName(QString fileName)
{
    ui->label_2->setText(fileName+"?");
    ui->label_2->setAlignment(Qt::AlignCenter);
    m_fileName = fileName;
}

void chooseFile::on_pushButton_clicked()
{
    emit choose();
}

void chooseFile::on_pushButton_2_clicked()
{
    emit cancel();
}
