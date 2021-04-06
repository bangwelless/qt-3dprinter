#include "parsing.h"
#include "ui_parsing.h"

parsing::parsing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::parsing)
{
    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/CircleProgressBar.qml"));
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget->setClearColor(QColor("#2d2c2b"));

    item=ui->quickWidget->rootObject();
    QObject::connect(this,SIGNAL(sendSignalToQml(int )),item,SIGNAL(receFromWidget(int )));

}

parsing::~parsing()
{
    delete ui;
}

void parsing::setNum(int num)
{
    emit sendSignalToQml(num);
}

void parsing::on_pushButton_clicked()
{
    emit cancel();
}
