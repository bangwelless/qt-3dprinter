#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w_ui;
    int nIndex = QFontDatabase::addApplicationFont(":/Barlow-Regular.ttf");
    int nindex = QFontDatabase::addApplicationFont(":/Barlow-Bold.ttf");
    if (nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
            QFont fontThis(strList.at(0));
            fontThis.setPointSize(9);
            a.setFont(fontThis);
        }
    }
    if (nindex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nindex));
    }
#ifdef XH_LINUX
//    w_ui.setWindowTitle(QString("3D打印机上位机测试版V2.0（for linux）"));
//    QGraphicsScene *scene = new QGraphicsScene;
//    QGraphicsProxyWidget *w = scene->addWidget(&w_ui);
//    w->setRotation(90);

//    QGraphicsView *view = new QGraphicsView(scene);
//    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    view->setWindowFlags(Qt::FramelessWindowHint);

//    view->setStyleSheet("padding:0px;border:0px");
//    view->show();
    w_ui.show();
#endif

#ifdef XH_WIN
    w_ui.show();
#endif
    return a.exec();
}
