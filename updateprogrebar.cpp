#include "updateprogrebar.h"
#include "ui_updateprogrebar.h"

updateProgreBar::updateProgreBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateProgreBar)
{
    ui->setupUi(this);

    ui->quickWidget->setSource(QUrl("qrc:/CircleProgressBar.qml"));
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget->setClearColor(QColor("#202023"));


    item=ui->quickWidget->rootObject();
    QObject::connect(this,SIGNAL(sendSignalToQml(int )),item,SIGNAL(receFromWidget(int )));
}

updateProgreBar::~updateProgreBar()
{
    delete ui;
}

void updateProgreBar::change(int a )
{
    emit sendSignalToQml(a);
}
