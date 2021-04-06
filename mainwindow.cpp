#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "choosefile.h"
#include "parsing.h"
#include "askpause.h"
#include "selectmode.h"
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QGuiApplication>
#include <QProcess>
#include "filecheckthread.h"
#include "filamentfault.h"


#include <binders.h>
#include <QScroller>

#ifdef XH_LINUX
void tWifi_event_callback(struct Manager *wmg,int event_lavel)
{
    qDebug()<<"tWifi_event_callback";
}
#endif

#ifdef XH_LINUX
    #define localPath "/usr/gode/"
    #define UDiskPath "/mnt/exUDISK/gcode"
    #define UDiskfind "/mnt/exUDISK/gcode"
    #define serialNum "ttyS2"
    #define UpdateFile "/mnt/exUDISK/updateFile/TestPack.JUP"
#endif

#ifdef XH_WIN
    #define UDiskPath QDir::currentPath()+"/UD"
    #define UDiskfind QDir::currentPath()+"/UD"
    #define localPath QDir::currentPath()+"/3D/"
    #define UpdateFile "C:/3D/TestPack.JUP"
#endif


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    /*初始化*/
    m_filamentfault = NULL;
    m_WinFiel = NULL;
    par = NULL;
    skpWin = NULL;
    m_mode = NULL;
    m_delete = NULL;

    m_sensor = NULL;
    m_wizard = NULL;
    m_rest = NULL;

    m_port = NULL;
//    fileCheckThread *m_thread;
    mchoose = NULL;
    m_modeone = NULL;
    item = NULL;
    item1 = NULL;
    m_filamentfault = NULL;
    m_printfilament = NULL;

    m_power = NULL;
    m_setdlog =NULL;
    m_parsetdlog = NULL;
    m_dam = NULL;
    m_dup = NULL;
    chooseit = NULL;
#ifdef XH_LINUX
    aw_wifi =NULL;
    udpControl = NULL;
#endif
    m_update = NULL;
    m_fileParser = NULL;

    /*开启串口*/
    serialOpen =false;
    m_port = new XhPort(this);

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox_15->addItem(serial.portName());
            serial.close();
        }
    }

    ui->comboBox_15->addItem("COM16");
    ui->comboBox_15->addItem("COM17");
    ui->comboBox_15->addItem("COM18");
    ui->comboBox_15->addItem("COM19");

    m_deepTimer = new QTimer(this);
    QObject::connect(m_deepTimer,&QTimer::timeout,this,&MainWindow::deepTimer);


    /*绑定串口信号*/
    /*frist*/
    QObject::connect(m_port, &XhPort::serialIsOpen, this, &MainWindow::serailIsOpen);
    QObject::connect(m_port, &XhPort::firstTestResult, this,&MainWindow::winGfour);
    QObject::connect(m_port,&XhPort::firstTemperatureResult,this,&MainWindow::temperatureChange);
    /*filament*/
    QObject::connect(m_port,&XhPort::filamentHeated,this,&MainWindow::filamentOK);
    /*tool mach set*/
//    QObject::connect(m_port,&XhPort::toolTestResult,this,&MainWindow::toolSelfTest);
    QObject::connect(m_port,&XhPort::disUseFilament,this,&MainWindow::disfilament);
    QObject::connect(m_port,&XhPort::factoryReset,this,&MainWindow::mfactoryReset);

    /*tool carb*/

    QObject::connect(m_port,&XhPort::pPlatformCalibration,this,&MainWindow::jump21);
    QObject::connect(m_port,&XhPort::nNozzleCalibration,this,&MainWindow::jump23);
    QObject::connect(m_port,&XhPort::xXyCalibration,this,&MainWindow::plat);
    QObject::connect(m_port,&XhPort::finished,this,&MainWindow::finished);
    QObject::connect(m_port,&XhPort::canelk,this,&MainWindow::cancle);

    /*print*/
    QObject::connect(m_port,&XhPort::canPrint,this,&MainWindow::m_canPrintFile);
    QObject::connect(m_port,&XhPort::fileSendOver,this,&MainWindow::jumpSixteen);
    QObject::connect(m_port,&XhPort::type,this,&MainWindow::planAdd);
    QObject::connect(m_port,&XhPort::stopOk,this,&MainWindow::printstop);
    QObject::connect(m_port,&XhPort::pauseOk,this,&MainWindow::printpause);
    QObject::connect(m_port,&XhPort::goOnOk,this,&MainWindow::printagin);
    QObject::connect(m_port,&XhPort::printend,this,&MainWindow::printending);

//    QObject::connect(m_port,&XhPort::noHeating,this,&MainWindow::xhnoHeating);
//    QObject::connect(m_port,&XhPort::platCheck,this,&MainWindow::xhplatCheck);
    QObject::connect(m_port,&XhPort::xNoHeating,this,&MainWindow::cannext);
//    QObject::connect(m_port,&XhPort::xyCheck,this,&MainWindow::xhxyCheck);

    QObject::connect(m_port,&XhPort::filamentlost,this,&MainWindow::filamentlost);
    QObject::connect(m_port,&XhPort::powerlost,this,&MainWindow::powerlost);
    QObject::connect(m_port,&XhPort::error,this,&MainWindow::error);
    QObject::connect(m_port,&XhPort::selfTest1,this,&MainWindow::selftest1);
    QObject::connect(m_port,&XhPort::selfTest2,this,&MainWindow::selftest2);
    QObject::connect(m_port,&XhPort::selfTest3,this,&MainWindow::selftest3);
    QObject::connect(m_port,&XhPort::selfTest4,this,&MainWindow::selftest4);
    QObject::connect(m_port,&XhPort::selfTest5,this,&MainWindow::selftest5);
    QObject::connect(m_port,&XhPort::selfTest6,this,&MainWindow::selftest6);

    QObject::connect(m_port,&XhPort::state,this,&MainWindow::state);
    QObject::connect(m_port,&XhPort::xcanone,this,&MainWindow::canone);
    QObject::connect(m_port,&XhPort::xcantwo,this,&MainWindow::cantwo);
    QObject::connect(m_port,&XhPort::updateBeginsignl,this,&MainWindow::updatebegin);
    QObject::connect(m_port,&XhPort::xhupdateNum,this,&MainWindow::updateNumx);

    print = new QTimer(this);
    QObject::connect(print,&QTimer::timeout,this,&MainWindow::printTime);
    /*设置透明度淡入淡出*/
    ui->label->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->label_3->setAttribute(Qt::WA_TranslucentBackground,true);

    /*设置状态栏*/
    ui->m_StatusBar->setVisible(false);
    /*设置列表*/
    ui->listWidget->setViewMode(QListView::ListMode);
    ui->listWidget->setFlow(QListView::TopToBottom);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setHorizontalScrollMode(QListWidget::QAbstractItemView::ScrollPerPixel);
    QScroller::grabGesture(ui->listWidget, QScroller::LeftMouseButtonGesture);

    ui->listWidget_2->setViewMode(QListView::ListMode);
    ui->listWidget_2->setFlow(QListView::TopToBottom);
    ui->listWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->listWidget_2->setHorizontalScrollMode(QListWidget::QAbstractItemView::ScrollPerPixel);
        ui->listWidget_2->setHorizontalScrollMode(QListWidget::ScrollPerPixel);
    QScroller::grabGesture(ui->listWidget_2, QScroller::LeftMouseButtonGesture);

    ui->listWidget_3->setViewMode(QListView::ListMode);
    ui->listWidget_3->setFlow(QListView::TopToBottom);
    ui->listWidget_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_3->setHorizontalScrollMode(QListWidget::QAbstractItemView::ScrollPerPixel);
    QScroller::grabGesture(ui->listWidget_3, QScroller::LeftMouseButtonGesture);

    /*检测U盘*/
    fileListTimer = new QTimer(this);
    QObject::connect(fileListTimer,SIGNAL(timeout()),this,SLOT(fileList()));
    fileListTimer->start(500);


    ui->listWidget->clear();
    QDir *m_dir=new QDir(localPath);
    QStringList filter;
    QFileInfoList m_fileinfo = m_dir->entryInfoList();

    int i = 0;
    for(i = 0;i< m_fileinfo.count();i++)
    {
        m_addItemToList(m_fileinfo.at(i).fileName(),m_fileinfo.at(i).filePath());
//        qDebug()<<m_fileinfo.at(i).filePath();
//        qDebug()<<m_fileinfo.at(i).fileName();
    }
//    QScroller::grabGesture(ui->listWidget,QScroller::LeftMouseButtonGesture);
    lheatend = false;
    rheatend = false;
    printTimer = new QTimer(this);
    isPrintFirstAnim=true;
    isStartFristAnim=false;
    m_opaCity=0.0;
    m_effect=new QGraphicsOpacityEffect();
    m_effect->setOpacity(m_opaCity);
    ui->label->setGraphicsEffect(m_effect);
    ui->label->setPixmap(QPixmap(":/LogoPic.jpg"));


    ui->quickWidget_3->setSource(QUrl("qrc:/CircleProgressBar.qml"));
    ui->quickWidget_3->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget_3->setClearColor(QColor("#202023"));

    ui->quickWidget_5->setSource(QUrl("qrc:/CircleProgressBar.qml"));
    ui->quickWidget_5->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget_5->setClearColor(QColor("#202023"));

    ui->quickWidget_6->setSource(QUrl("qrc:/CircleProgressBar.qml"));
    ui->quickWidget_6->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget_6->setClearColor(QColor("#202023"));


    ui->quickWidget_4->setSource(QUrl("qrc:/BiCircleProgressBar.qml"));
    ui->quickWidget_4->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget_4->setClearColor(QColor("#202023"));


    item=ui->quickWidget_3->rootObject();
    QObject::connect(this,SIGNAL(sendSignalToQml(int )),item,SIGNAL(receFromWidget(int )));

    item3=ui->quickWidget_5->rootObject();
    QObject::connect(this,SIGNAL(sendSignalToQml(int )),item3,SIGNAL(receFromWidget(int )));

    item4=ui->quickWidget_6->rootObject();
    QObject::connect(this,SIGNAL(sendSignalToQml(int )),item4,SIGNAL(receFromWidget(int )));

    item1=ui->quickWidget_4->rootObject();
    QObject::connect(this,SIGNAL(sendSignalHeating(int ,int )),item1,SIGNAL(receFromWidgetT(int ,int )));

    m_printsec = new QTimer(this);
    m_time = new QTime(0,0,0);
    QObject::connect(m_printsec,&QTimer::timeout,this,&MainWindow::timeAdd);

#ifdef  XH_LINUX
    m_port->portInit(serialNum);
        m_port->serialOpen =true;
        ui->stackedWidget->setCurrentIndex(33);
        ui->quickWidget->setSource(QUrl("qrc:/pageView.qml"));
        ui->quickWidget_2->setSource(QUrl("qrc:/pageView.qml"));
        ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        ui->quickWidget_2->setResizeMode(QQuickWidget::SizeRootObjectToView);
        ui->quickWidget->show();
        ui->quickWidget_2->show();

        serialOpen =true;
        ui->m_StatusBar->setVisible(true);
        QObject::connect(printTimer,&QTimer::timeout,this,&MainWindow::askPrint);
        printTimer->start(1000);

#endif

#ifdef XH_WIN
    ui->stackedWidget->setCurrentIndex(79);

#endif

    m_port->powerlostsend();
    setWinPic(false);
    m_fileParser = new XhGcodeFileParser(this);
    QObject::connect(m_fileParser,&XhGcodeFileParser::parseByDirectMode,this,&MainWindow::parseMode,Qt::QueuedConnection);
    QObject::connect(m_fileParser,&XhGcodeFileParser::parseByDeepHeader,this,&MainWindow::parseHeader,Qt::QueuedConnection);
    QObject::connect(m_fileParser,&XhGcodeFileParser::parseDeepSucceded,this,&MainWindow::parseDeep,Qt::QueuedConnection);
    QObject::connect(m_fileParser,&XhGcodeFileParser::parseByDeepMode,this,&MainWindow::parseDeepMode,Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_thread->isRunning())
    {
        m_thread->exit(0);
        delete  m_thread;
    }
}

void MainWindow::setTemperatureLeft(int num1,int num2)
{
    char m_string[10] ="";
    sprintf(m_string,"%03d|%03d",num1,num2);
    QString str = QString(QLatin1String(m_string))+"°C";
    ui->label_125->setText(str);
}

void MainWindow::setTemperatureRight(int num1,int num2)
{
    char m_string[10] ="";
    sprintf(m_string,"%03d|%03d",num1,num2);
    QString str = QString(QLatin1String(m_string))+"°C";
    ui->label_127->setText(str);
}

void MainWindow::setBedPic(bool isVisible)
{
    ui->label_128->setVisible(isVisible);
}

void MainWindow::setBedTemperature(int num1,int num2)
{
    char m_string[10] ="";
    sprintf(m_string,"%03d|%03d",num1,num2);
    QString str = QString(QLatin1String(m_string))+"°C";
    ui->label_29->setText(str);
}

void MainWindow::setZnumber(float num)
{
    char m_string[10] ="";
    qDebug()<<num;
    num = num/1000;
    sprintf(m_string,"%05.1f",num);
    qDebug()<<m_string;
    QString str = QString(QLatin1String(m_string))+"mm";

    ui->label_129->setText(str);
}

void MainWindow::setUSBpic(bool isVisible)
{
    ui->label_132->setVisible(isVisible);
}

void MainWindow::setLightPic(bool isVisible)
{
    ui->label_131->setVisible(isVisible);
}

void MainWindow::setWinPic(bool isVisible)
{
    ui->label_130->setVisible(isVisible);
}

void MainWindow::setPicColor(QWidget *pic, QWidget *lab, bool isgray)
{
    if(isgray)
    {
        pic->setStyleSheet("QWidget{background-color:yellow;}");
    }
    else
    {

    }
}

void MainWindow::m_addItemToList(const QString &fileName, QString filePath)
{
    qDebug()<<"localADD";
    if(fileName == "."||fileName == "..")
    {
        return;
    }
    myListWidgetItem *pWidgetItem = new myListWidgetItem(this);
    pWidgetItem->m_addItem(fileName,filePath);
    QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget);
    pItem->setSizeHint(QSize(1050,127));

//    if(m_map.value(pWidgetItem) == nullptr)
//    {
        m_map.insert(pWidgetItem,pItem);
        ui->listWidget->setItemWidget(pItem,pWidgetItem);

        QObject::connect(pWidgetItem,SIGNAL(deleteItem(myListWidgetItem * )),this,SLOT(m_deleteItem(myListWidgetItem * )));
        QObject::connect(pWidgetItem,SIGNAL(chooseFile(myListWidgetItem * )),this,SLOT(m_chooseItem(myListWidgetItem * )));
        //    }
}

void MainWindow::m_addItemToList(const QString &fileName, QString filePath, QString uDisk)
{
    qDebug()<<"udiskADD";
    if(fileName == "."||fileName == "..")
    {
        return;
    }
    myListWidgetItem *pWidgetItem = new myListWidgetItem(this);
    pWidgetItem->m_addItem(fileName,filePath);
    pWidgetItem->isUDisk = uDisk;
    QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget_2);
    pItem->setSizeHint(QSize(930,127));

        m_map.insert(pWidgetItem,pItem);
        ui->listWidget_2->setItemWidget(pItem,pWidgetItem);

        QObject::connect(pWidgetItem,SIGNAL(deleteItem(myListWidgetItem * )),this,SLOT(m_deleteItem(myListWidgetItem * )));
        QObject::connect(pWidgetItem,SIGNAL(chooseFile(myListWidgetItem * )),this,SLOT(m_chooseItem(myListWidgetItem * )));
}

void MainWindow::m_adTtemtowifi(const QString &wifiname, QString wifilevel)
{
    myWifiItem *pWidgetItem = new myWifiItem(this);
    pWidgetItem->addwifi(wifiname,wifilevel);
    QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget_3);
    m_wmap.insert(pWidgetItem,pItem);
    ui->listWidget_3->setItemWidget(pItem,pWidgetItem);

    QObject::connect(pWidgetItem,&myWifiItem::conncetwifi,this,&MainWindow::connctwifi);
//    QObject::connect(pWidgetItem,SIGNAL(chooseFile(myListWidgetItem * )),this,SLOT(m_chooseItem(myListWidgetItem * )));
}

void MainWindow::firstStart()
{
    if(m_opaCity >= 1.0)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(firstStart()));
        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpOne()));
        m_timer.start(2000);
    }
    else
    {
        m_effect->setOpacity(m_opaCity);
        ui->label->setGraphicsEffect(m_effect);
    }
    m_opaCity+=0.1;
}

void MainWindow::jumpOne()
{
    ui->quickWidget->setSource(QUrl("qrc:/pageView.qml"));
    ui->quickWidget_2->setSource(QUrl("qrc:/pageView.qml"));


    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget_2->setResizeMode(QQuickWidget::SizeRootObjectToView);

    //ui->quickWidget->resize(1050,230);
    ui->quickWidget->show();
    ui->quickWidget_2->show();

    /*跳到welcome页，并初始化G1页参数*/
    ui->stackedWidget->setCurrentIndex(1);
    m_timer.stop();
    m_opaCity=0.0;
    m_effect->setOpacity(m_opaCity);
    ui->label_3->setGraphicsEffect(m_effect);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpOne()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpTwo()));
    m_timer.start(2000);

}

void MainWindow::jumpTwo()
{
    m_timer.stop();
    /*跳转到G1页*/
    ui->stackedWidget->setCurrentIndex(2);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpTwo()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(winGone()));
    m_timer.start(250);
}

void MainWindow::winGone()
{
    if(m_opaCity >= 1.0)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(winGone()));
        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(winGtwo()));
        m_timer.start(1000);
    }
    else
    {
        m_effect->setOpacity(m_opaCity);
        ui->label_3->setGraphicsEffect(m_effect);
    }
    m_opaCity+=0.1;
}

void MainWindow::winGtwo()
{
    if(serialOpen)
    {
        /*跳页至G2页*/
        ui->stackedWidget->setCurrentIndex(3);
        m_timer.stop();
        m_opaCity=0.0;
        m_effect->setOpacity(m_opaCity);
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(winGtwo()));
        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpThree()));
        m_timer.start(2000);
    }
}

void MainWindow::serailIsOpen()
{
    serialOpen =true;
}

void MainWindow::jumpThree()
{
    /*跳页至G3页自检*/
    ui->stackedWidget->setCurrentIndex(4);
    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpThree()));

    m_port->askStatus();


}

void MainWindow::winGfour(bool a, bool b, bool c, bool d, bool e)
{
    /*判断是跳至G4还是G4-1*/
    if(a && b && c && d && e)
    {
        /*正常启动*/
        ui->stackedWidget->setCurrentIndex(5);
        m_port->askTemperature();

        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFour()));
        m_timer.start(2000);
    }
    else
    {
        /*非正常启动*/
        ui->stackedWidget->setCurrentIndex(7);

    }
}

void MainWindow::temperatureChange(int a, int b, int c, int d, int e, int f, int g, QByteArray data)
{
    setTemperatureLeft(a,b);
    setTemperatureRight(c,d);
    setBedTemperature(e,f);
    setZnumber(g);
    if(a > 70 || b > 0)
    {
        ui->label_124->setPixmap(QPixmap(":/LHotendActive.png"));
    }else {
        ui->label_124->setPixmap(QPixmap(":/LHotendInactive.png"));
    }
    if(c>70||d>0)
    {
        ui->label_126->setPixmap(QPixmap(":/RHotendActive.png"));
    }else {
        ui->label_126->setPixmap(QPixmap(":/RHotendInactive.png"));
    }
    if(f > 0)
    {
        ui->label_128->setPixmap(QPixmap(":/BedActive.png.png"));
    }
    else {
        ui->label_128->setPixmap(QPixmap(":/rrr.png"));
    }

#ifdef XH_LINUX
    currentState.clear();
    currentState = data;
#endif
}

/*正常启动后进入*/
void MainWindow::jumpFour()
{
    /*当前界面为G5*/
    ui->stackedWidget->setCurrentIndex(6/*7*/);
    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFour()));

}

void MainWindow::jumpFive()
{
    /*G4-1-1*/
    m_timer.stop();
    ui->stackedWidget->setCurrentIndex(9);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFive()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSix()));
    m_timer.start(2000);
}

void MainWindow::jumpSix()
{
    /*G4-1-2*/
    m_timer.stop();
    ui->stackedWidget->setCurrentIndex(10);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSix()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeven()));
    m_timer.start(2000);
}

void MainWindow::jumpSeven()
{
    /*G4-1-3*/
    m_timer.stop();
    ui->stackedWidget->setCurrentIndex(11);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeven()));
}

void MainWindow::jumpEight()
{
    m_timer.stop();
    ui->stackedWidget->setCurrentIndex(13);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEight()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpNine()));
    m_timer.start(2000);

}

void MainWindow::jumpNine()
{
    m_timer.stop();
    ui->stackedWidget->setCurrentIndex(14);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpNine()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpTen()));
    m_timer.start(2000);
}

void MainWindow::jumpTen()
{
    m_timer.stop();
    ui->stackedWidget->setCurrentIndex(15);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpTen()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFour()));
    m_timer.start(2000);
}

void MainWindow::jumpEleven()
{
    /*第一个pageview页面*/
    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEleven()));
    ui->stackedWidget->setCurrentIndex(17);

}

void MainWindow::shineone()
{
#ifdef OLD
    if(flicker)
    {
        if(m_opaCity >= 1.0)
        {
           flicker =false;
        }
        else
        {
            m_effect->setOpacity(m_opaCity);
            ui->pushButton_24->setGraphicsEffect(m_effect);
        }
        m_opaCity += 0.1;
    }
    else
    {
        if(m_opaCity <= 0.0)
        {
            flicker =true;
        }
        else
        {
            m_effect ->setOpacity(m_opaCity);
            ui->pushButton_24->setGraphicsEffect(m_effect);
        }
        m_opaCity -= 0.1;
    }
    timeLonger += 1;
    if(timeLonger == 160)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(shineone()));
        m_opaCity = 1.0;
        m_effect ->setOpacity(m_opaCity);
        ui->pushButton_24->setGraphicsEffect(m_effect);
    }
#endif
}

void MainWindow::shinetwo()
{
#ifdef OLD
    if(flicker)
    {
        if(m_opaCity >= 1.0)
        {
           flicker =false;
        }
        else
        {
            m_effect->setOpacity(m_opaCity);
            ui->pushButton_41->setGraphicsEffect(m_effect);
        }
        m_opaCity += 0.1;
    }
    else
    {
        if(m_opaCity <= 0.0)
        {
            flicker =true;
        }
        else
        {
            m_effect ->setOpacity(m_opaCity);
            ui->pushButton_41->setGraphicsEffect(m_effect);
        }
        m_opaCity -= 0.1;
    }
    timeLonger += 1;
    if(timeLonger == 1600)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(shinetwo()));
        m_opaCity = 1.0;
        m_effect ->setOpacity(m_opaCity);
        ui->pushButton_41->setGraphicsEffect(m_effect);
    }
#endif
}

void MainWindow::jumptwelve()
{
#ifdef OLD
    ui->m_StatusBar->setVisible(true);
    //start 第一动画
    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumptwelve()));
    ui->stackedWidget->setCurrentIndex(21);
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpthirteen()));
    //ui->label_45->setScaledContents(true);
    timeLonger=0;
    m_x=250;
    m_y=200;
    m_w=450;
    m_h=450;
    flicker =true;
    m_timer.start(10);
#endif
}

void MainWindow::jumpthirteen()
{
    /*G10动图槽函数*/
#ifdef OLD
    if(flicker)
    {

        if(m_x>=240&&m_y>=190)
        {
            m_x-=1;
            m_y-=1;
            m_w+=2;
            m_h+=2;
            ui->label_45->setGeometry(m_x,m_y,m_w,m_h);
        }
        else
        {
            flicker =false;
        }
    }
    else
    {

        if(m_x<=260&&m_y<=210)
        {
            m_x+=1;
            m_y+=1;
            m_w-=2;
            m_h-=2;
            ui->label_45->setGeometry(m_x,m_y,m_w,m_h);
        }
        else
        {
            flicker =true;
        }
    }
    timeLonger+=1;
    if(timeLonger == 200)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpthirteen()));
        ui->stackedWidget->setCurrentIndex(22);
        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFourteen()));
        timeLonger=0;
        m_x=250;
        m_y=200;
        m_w=450;
        m_h=450;
        flicker =true;
        m_timer.start(10);
    }
#endif
}

void MainWindow::jumpFourteen()
{
#ifdef OLD
    /*G11动图槽函数*/
    if(flicker)
    {

        if(m_x>=240&&m_y>=190)
        {
            m_x-=1;
            m_y-=1;
            m_w+=2;
            m_h+=2;
            ui->label_56->setGeometry(m_x,m_y,m_w,m_h);
        }
        else
        {
            flicker =false;
        }
    }
    else
    {

        if(m_x<=260&&m_y<=210)
        {
            m_x+=1;
            m_y+=1;
            m_w-=2;
            m_h-=2;
            ui->label_56->setGeometry(m_x,m_y,m_w,m_h);
        }
        else
        {
            flicker =true;
        }
    }
    timeLonger+=1;
    if(timeLonger == 200)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFourteen()));
        ui->stackedWidget->setCurrentIndex(23);
        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFifteen()));
        m_x=250;
        m_y=200;
        m_w=450;
        m_h=450;
        timeLonger=0;
        flicker =true;
        m_timer.start(10);
    }
#endif
}

void MainWindow::jumpFifteen()
{
#ifdef OLD
    /*G12动图槽函数*/
    if(flicker)
    {

        if(m_x>=240&&m_y>=190)
        {
            m_x-=1;
            m_y-=1;
            m_w+=2;
            m_h+=2;
            ui->label_57->setGeometry(m_x,m_y,m_w,m_h);
        }
        else
        {
            flicker =false;
        }
    }
    else
    {

        if(m_x<=260&&m_y<=210)
        {
            m_x+=1;
            m_y+=1;
            m_w-=2;
            m_h-=2;
            ui->label_57->setGeometry(m_x,m_y,m_w,m_h);
        }
        else
        {
            flicker =true;
        }
    }
    timeLonger+=1;
    if(timeLonger == 200)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFifteen()));
        m_x=0;
        m_y=0;
        m_w=0;
        m_h=0;
        timeLonger=0;
        flicker =true;
    }
#endif
}

void MainWindow::jumpSixteen()
{
    QByteArray filecheck = m_port->startPrint(this->loaclPATH);
    qDebug()<<"localPath"<<this->loaclPATH;
    qDebug()<<"---------------filecheck**************"<<filecheck;
    m_port->AcsPrint(filecheck);
}
void MainWindow::askPrint()
{
    m_port->askTemperature();
}

void MainWindow::jumpSeventeen()
{
    /*加热槽函数*/
    int l1 = ui->label_125->text().left(3).toInt();
    int l2 = ui->label_125->text().mid(4,3).toInt();
    qDebug()<<"time out heating";
    int r = ui->label_127->text().left(3).toInt();
    int r2 = ui->label_127->text().mid(4,3).toInt();
    if(l2!=0 || r2 != 0)
    {
        if(l1>=(l2*0.9)&&r>=(r2*0.9))
        {
            m_port->readyprint(this->printMode,offset);
            qDebug()<<"readyprint offset "<<offset;
            qDebug()<<"readyprint mode"<<printMode;
            QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
            m_timer.stop();
        }
    }

}

void MainWindow::jumpEightteen()
{
    /*print2动图槽函数*/

}

void MainWindow::jumpnineteen()
{
    /*print3动图槽函数*/

}

void MainWindow::jumpTwenty(bool a)
{
    qDebug()<<a;
    if(a)
    {
        ui->stackedWidget->setCurrentIndex(55);
        m_port->p_platformCalibration();
    }
    else
    {
        m_port->p_nozzleHeating();
    }
}

void MainWindow::jump21(qint32 a , qint32 b, qint32 c, qint32 d)
{
//    ui->label_133->setNum((int)(c/1000));
//    ui->label_133->setAlignment(Qt::AlignCenter);
    ui->label_253->setNum((int)(a/1000));
    ui->label_253->setAlignment(Qt::AlignCenter);
    ui->label_261->setNum((int)(d/1000));
    ui->label_261->setAlignment(Qt::AlignCenter);
    ui->label_256->setNum((int)(b/1000));
    ui->label_256->setAlignment(Qt::AlignCenter);
    if(a==0&&b==0&&c==0&&d==0)
    {
        ui->stackedWidget->setCurrentIndex(55);
    }
    else {
        ui->stackedWidget->setCurrentIndex(56);
    }

//    m_timer.stop();
//    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jump21()));
}

void MainWindow::jump22(bool)
{
    ui->stackedWidget->setCurrentIndex(59);
    m_port->n_nozzleCalibration();
}

void MainWindow::jump23(int b)
{
    if(b==0)
    {
        ui->stackedWidget->setCurrentIndex(64);
    }
    else {
        ui->stackedWidget->setCurrentIndex(63);
    }

    ui->label_151->setNum(b/1000);
    ui->label_151->setAlignment(Qt::AlignCenter);
//    if(a)
//    {
//        ui->pushButton_304->setIcon(QPixmap(":/9.png"));
//        ui->pushButton_305->setIcon(QPixmap(":/11.png"));
//    }
//    else {
//        ui->pushButton_305->setIcon(QPixmap(":/9.png"));
//        ui->pushButton_304->setIcon(QPixmap(":/11.png"));
//    }
//    m_timer.stop();
//    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jump23()));
}


void MainWindow::jump24(bool a )
{
    if(a)
    {
    ui->stackedWidget->setCurrentIndex(64);
    m_port->x_platformCalibration();

    }
    else {
        m_port->x_xyHeating();
    }
}

void MainWindow::jump25(bool a)
{
//    m_timer.stop();
//    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jump25()));
    if(a)
    {
        m_port->x_xyCalibration();
    }
    else {
        m_port->x_platformCalibration();
    }
}

void MainWindow::plat()
{
    ui->stackedWidget->setCurrentIndex(68);
}

void MainWindow::printstop()
{
    if(m_filamentfault!= NULL)
    {
        if(m_filamentfault->isHidden())
        {

        }
        else
        {
            m_filamentfault->hide();
            m_filamentfault->close();
            m_filamentfault = NULL;
        }
    }
    if(skpWin!=NULL)
    {
        skpWin->hide();
        skpWin->close();
        skpWin = NULL;
    }
    ui->stackedWidget->setCurrentIndex(33);
    m_printsec->stop();
    m_time->setHMS(0,0,0);
}

void MainWindow::printpause()
{
    ui->label_272->setVisible(true);
    ui->pushButton_167->setVisible(true);
    ui->pushButton_168->setVisible(true);
    m_printsec->stop();
}

void MainWindow::printagin()
{
    if(m_filamentfault!= NULL)
    {
        if(m_filamentfault->isHidden())
        {

        }
        else
        {
            m_filamentfault->hide();
            m_filamentfault->close();
            m_filamentfault = NULL;
        }
    }
    ui->stackedWidget->setCurrentIndex(36);
    //    m_timer.start(15);
    m_printsec->start(1000);
}

void MainWindow::printTime()
{
    printtime++;
}

void MainWindow::filamentTimeout()
{
    //m_port->askTemperature();
    if(ui->stackedWidget->currentIndex() == 30)
    {
        int l1 = ui->label_125->text().left(3).toInt();
        int l2 = ui->label_125->text().mid(4,3).toInt();
        int r = ui->label_127->text().left(3).toInt();
        int r2 = ui->label_127->text().mid(4,3).toInt();

        if(l2 != 0&& r2 != 0)
        {
            if(l1>(l2-5) && r>(r2-5))
            {
                m_timer.stop();
                ui->pushButton_358->setText("Complete");
                QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
                lheatend =false;
                rheatend =false;
#ifdef OLD
                ui->pushButton_111->setStyleSheet("QPushButton{outline:none;\
                                                  image: url(:/UnloadButtonActive.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_112->setStyleSheet("QPushButton{outline:none;\
                                                  image: url(:/LoadButtonActive.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_114->setStyleSheet("QPushButton{outline:none;\
                                                image: url(:/UnloadButtonActive.png);\
                                                background-color: rgb(45, 44, 43);\
                                                border:none;};");
                ui->pushButton_115->setStyleSheet("QPushButton{outline:none;\
                                                image: url(:/LoadButtonActive.png);\
                                                background-color: rgb(45, 44, 43);\
                                                border:none;};");
#endif
                ui->pushButton_111->setEnabled(true);
                ui->pushButton_112->setEnabled(true);
                ui->pushButton_114->setEnabled(true);
                ui->pushButton_115->setEnabled(true);

                ui->pushButton_105->setEnabled(true);
                ui->pushButton_106->setEnabled(true);
                ui->pushButton_107->setEnabled(true);
                ui->pushButton_108->setEnabled(true);

            }
        }
        if(l2 !=0 &&r2 == 0)
        {
            if(l1>(l2-5))
            {
                m_timer.stop();
                QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
                lheatend =false;
                rheatend =false;
                ui->pushButton_358->setText("Complete");
#ifdef OLD
                ui->pushButton_111->setStyleSheet("QPushButton{outline:none;\
                                                  image: url(:/UnloadButtonActive.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_112->setStyleSheet("QPushButton{outline:none;\
                                                  image: url(:/LoadButtonActive.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_114->setStyleSheet("QPushButton{outline:none;\
                                                    image: url(:/UnloadButton.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_115->setStyleSheet("QPushButton{outline:none;\
                                                    image: url(:/LoadButton.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
#endif
                ui->pushButton_111->setEnabled(true);
                ui->pushButton_112->setEnabled(true);
                ui->pushButton_114->setEnabled(false);
                ui->pushButton_115->setEnabled(false);

                ui->pushButton_105->setEnabled(true);
                ui->pushButton_106->setEnabled(true);
                ui->pushButton_107->setEnabled(true);
                ui->pushButton_108->setEnabled(true);

            }
        }
        if(l2 == 0&&r2 != 0)
        {
            if(r>(r2-5))
            {
                m_timer.stop();
                QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));

                lheatend =false;
                rheatend =false;
                ui->pushButton_358->setText("Complete");
#ifdef OLD
                ui->pushButton_114->setStyleSheet("QPushButton{outline:none;\
                                                  image: url(:/UnloadButtonActive.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_115->setStyleSheet("QPushButton{outline:none;\
                                                  image: url(:/LoadButtonActive.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_111->setStyleSheet("QPushButton{outline:none;\
                                                    image: url(:/UnloadButton.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
                ui->pushButton_112->setStyleSheet("QPushButton{outline:none;\
                                                    image: url(:/LoadButton.png);\
                                                  background-color: rgb(45, 44, 43);\
                                                  border:none;};");
#endif
                ui->pushButton_111->setEnabled(false);
                ui->pushButton_112->setEnabled(false);
                ui->pushButton_114->setEnabled(true);
                ui->pushButton_115->setEnabled(true);

                ui->pushButton_105->setEnabled(false);
                ui->pushButton_107->setEnabled(false);
                ui->pushButton_108->setEnabled(false);
            }
        }
        if(l2== 0&&r2==0)
        {
            lheatend =false;
            rheatend =false;
            m_timer.stop();
            QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
            ui->pushButton_358->setText("Cancel");
#ifdef OLD
            ui->pushButton_111->setStyleSheet("QPushButton{outline:none;\
                                                image: url(:/UnloadButton.png);\
                                              background-color: rgb(45, 44, 43);\
                                              border:none;};");
            ui->pushButton_112->setStyleSheet("QPushButton{outline:none;\
                                                image: url(:/LoadButton.png);\
                                              background-color: rgb(45, 44, 43);\
                                              border:none;};");
            ui->pushButton_114->setStyleSheet("QPushButton{outline:none;\
                                                image: url(:/UnloadButton.png);\
                                              background-color: rgb(45, 44, 43);\
                                              border:none;};");
            ui->pushButton_115->setStyleSheet("QPushButton{outline:none;\
                                                image: url(:/LoadButton.png);\
                                              background-color: rgb(45, 44, 43);\
                                              border:none;};");
#endif
            ui->pushButton_111->setEnabled(false);
            ui->pushButton_112->setEnabled(false);
            ui->pushButton_114->setEnabled(false);
            ui->pushButton_115->setEnabled(false);

            ui->pushButton_105->setEnabled(true);
            ui->pushButton_107->setEnabled(true);
            ui->pushButton_108->setEnabled(true);
        }
    }

}

void MainWindow::filamentOK(bool a)
{
    if(a)
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
        ui->stackedWidget->setCurrentIndex(32);
    }
}

//void MainWindow::toolSelfTest(bool a, bool b, bool c, bool d, bool e, bool f)
//{
//    m_timer.stop();
//    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(toolSelfTest()));
//    if(!a)
//    {
//        ui->label_175->setPixmap(QPixmap(":/unready.png"));
//    }
//    if(!b)
//    {
//        ui->label_176->setPixmap(QPixmap(":/unready.png"));
//    }
//    if(!c)
//    {
//        ui->label_177->setPixmap(QPixmap(":/unready.png"));
//    }
//    if(!d)
//    {
//        ui->label_178->setPixmap(QPixmap(":/unready.png"));
//    }
//    if(!e)
//    {
//        ui->label_179->setPixmap(QPixmap(":/unready.png"));
//    }
//    if(!f)
//    {
//        ui->label_180->setPixmap(QPixmap(":/unready.png"));
//    }
//    ui->stackedWidget->setCurrentIndex(75);
//}

void MainWindow::disfilament(bool a)
{
    if(a)
    {
        m_sensor->hide();
        m_sensor->close();
    }
    else
    {
        m_port->unFilament();
    }
}

void MainWindow::mfactoryReset()
{
        m_rest->hide();
        m_rest->close();
}

void MainWindow::planAdd(int a,int b)
{

    //更新加热/打印百分比和状态
    switch (ui->stackedWidget->currentIndex()) {
    case 35:
    {

        int l1 = ui->label_125->text().left(3).toInt();
        int l2 = ui->label_125->text().mid(4,3).toInt();
        int r1 = ui->label_127->text().left(3).toInt();
        int r2 = ui->label_127->text().mid(4,3).toInt();
        qDebug()<<l1<<l2<<r1<<r2;
        float l;
        float r;
        if(l2 != 0)
        {
            l = (float)l1/(float)l2;
            qDebug()<<l<<"l";
        }
        if(r2 != 0)
        {
            r = (float)r1/(float)r2;
            qDebug()<<r<<"r";
        }

        int ll = (int)(l*100);
        int rr = (int)(r*100);
        if(r2 == 0)
        {
            emit sendSignalHeating(ll,100);
        }
        else if(l2 == 0)
        {
            emit sendSignalHeating(100,rr);
        }
        else
        {
            emit sendSignalHeating(ll,rr);
        }
        qDebug()<<"sendSignalToQml"<<ll<<rr;
        break;
    }
    case 36:
    {
        emit sendSignalToQml(b);
        qDebug()<<"sendSignalToQml"<<b;
        if(a == 0)
        {
                //显示finished按钮
                ui->stackedWidget->setCurrentIndex(37);

        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::buprint()
{
    ui->stackedWidget->setCurrentIndex(33);
//    m_printsec->stop();
    m_deepTimer->stop();
    par->hide();
    par->close();
}

void MainWindow::powercancle()
{
    m_power->hide();
    m_power->close();
    m_port->powercancel();
}

void MainWindow::getCondition()
{
#ifdef XH_LINUX
    emit sendCondition(currentState);
#endif
}

void MainWindow::downloadOver(QString fileName)
{
    m_fileParser->parseByDeep("/usr/share/3d_printer/tmp/"+fileName,localPath+fileName);
#ifdef XH_LINUX
    system(QString::asprintf("rm /usr/share/3d_printer/tmp/%s",fileName.toStdString().c_str()).toStdString().c_str());
#endif
}

void MainWindow::m_deleteItem(myListWidgetItem *itm)
{
    m_delete = new Delete(this);
    m_delete->show();
    m_delete->deleteItem = itm;
    m_delete->filePath = itm->m_filePath;
    m_delete->fileName = itm->m_fileName;
    m_delete->setName();
    QObject::connect(m_delete,SIGNAL(checkDelete(myListWidgetItem * )),this,SLOT(m_deleteTrue(myListWidgetItem * )));
    QObject::connect(m_delete,SIGNAL(checkCancel(myListWidgetItem * )),this,SLOT(m_deleteFalse(myListWidgetItem * )));

}

void MainWindow::m_deleteTrue(myListWidgetItem *itm)
{
    QFile fileTemp(itm->m_filePath);
    fileTemp.remove();
    if(itm->isUDisk == "")
    {
        QListWidgetItem *v=m_map.take(itm);
        ui->listWidget->takeItem(ui->listWidget->row(v));
        delete v;
    }else
    {
        QListWidgetItem *v=m_map.take(itm);
        ui->listWidget_2->takeItem(ui->listWidget_2->row(v));
        delete v;
    }
    m_delete->hide();
    m_delete->close();

}

void MainWindow::m_deleteFalse(myListWidgetItem *itm)
{
    m_delete->hide();
    m_delete->close();
}

void MainWindow::m_chooseItem(myListWidgetItem *itm)
{
    QListWidgetItem *v =m_map.value(itm);

    m_WinFiel = new chooseFile(this);
    m_WinFiel->setFileName(itm->m_fileName);
    m_WinFiel->m_filePath=itm->m_filePath;
    QObject::connect(m_WinFiel,SIGNAL(choose()),this,SLOT(m_chooseEN()));
    QObject::connect(m_WinFiel,SIGNAL(cancel()),this,SLOT(m_chooseUEN()));
    m_WinFiel->show();

    ui->label_72->setText(itm->m_fileName);
    ui->label_73->setText(itm->m_fileName);
    ui->label_309->setText(itm->m_fileName);
    ui->label_313->setText(itm->m_fileName);

}

void MainWindow::connctwifi(myWifiItem *itm)
{
    ui->label_319->setText(itm->wifiname);
    ui->stackedWidget->setCurrentIndex(82);
    ui->stackedWidget_2->setCurrentIndex(0);
    chooseit = NULL;
    chooseit = itm;
}

void MainWindow::m_chooseEN()
{
    loaclPATH.clear();
    loaclPATH = localPath+m_WinFiel->m_fileName;
    /* 2021/3/2 cbw */
    QString a= QDir::currentPath()+"/UD";
    if(m_WinFiel->m_filePath.left(4) == "/mnt"||m_WinFiel->m_filePath.left(a.size()) == UDiskPath)
    {
        m_fileParser->parseByDirect(m_WinFiel->m_filePath,localPath+m_WinFiel->m_fileName);
        udiskPATH = m_WinFiel->m_filePath;
        openMode = false;
    }
    else
    {
        //解析文件
        int mode = 0;
        int x1=0;
        int x2=0;
        int x3=0;
        int x4=0;
        int x5=0;
        QFile m_sendFile(m_WinFiel->m_filePath);
        if(m_sendFile.open(QIODevice::ReadOnly))
        {
            QByteArray m_filedata = m_sendFile.readLine();
            QByteArray y(" ");
            x1 = m_filedata.indexOf(y);
            x2 = m_filedata.indexOf(y,x1+1);
            x3 = m_filedata.indexOf(y,x2+1);
            x4 = m_filedata.indexOf(y,x3+1);
            x5 = m_filedata.indexOf(y,x4+1);

            QString data = m_filedata;
            if(data.mid(x1+12,x2-x1-12) == "unknown"||data.mid(x1+12,x2-x1-12) =="Unknown" )
            {
                mode = 1;
                this->printMode = 0;
            }
            if(data.mid(x1+12,x2-x1-12) == "Mirror")
            {
                mode = 2;
            }
            if(data.mid(x1+12,x2-x1-12) == "Duplicate")
            {
                mode = 3;
            }
            if(data.mid(x1+12,x2-x1-12) == "Unsupport")
            {
                mode = 4;
                this->printMode = 0;
            }
            if(data.mid(x1+12,x2-x1-12) == "Mix")
            {
                mode = 5;
                this->printMode = 6;
            }
            if(data.mid(x1+12,x2-x1-12) == "Orgin-Mirror")
            {
                mode = 6;
                this->printMode = 5;
            }
            if(data.mid(x1+12,x2-x1-12) == "Orgin-Duplicate")
            {
                mode = 7;
                this->printMode = 4;
            }
            lt = data.mid(x3+11,x4-x3-11);
            rt = data.mid(x4+12,x5-x4-12);
            bt = data.mid(x5+10,data.size()-x5-10);
            QString offset = data.mid(x2+8,x3-x2-8);
            qDebug()<<"offset qstring read"<<offset;
#ifdef OLD
            if(offset.contains("-"))
            {
                offset = offset.mid(1,offset.size()-1);
                float a = offset.toFloat();
                offsetnum = 0 - (a*1000);
            }
            else {
                offsetnum = offset.toInt()*1000;
            }
#endif
            if(offset.contains("-"))
            {
                offset = offset.mid(1,offset.size()-1);
                qDebug()<<"offset"<<offset;
                float a = offset.toFloat();
                offsetnum = 0 - (a*1000);
                qDebug()<<"numoffset"<<offsetnum;
            }
            else {
                float a = offset.toFloat();
                offsetnum = a*1000;
                qDebug()<<"numoffset"<<offsetnum;
            }
        }
        m_dam = new DupandMirorr(this);
        QObject::connect(m_dam,&DupandMirorr::print,this,&MainWindow::tprint);
        QObject::connect(m_dam,&DupandMirorr::mirro,this,&MainWindow::tmirro);
        QObject::connect(m_dam,&DupandMirorr::dup,this,&MainWindow::tdup);
        QObject::connect(m_dam,&DupandMirorr::cancle,this,&MainWindow::tcanle);

        switch (mode) {
        case 1:
            m_dam->changeMode(0);
            break;
        case 2:
            m_dam->changeMode(2);
            break;
        case 3:
            m_dam->changeMode(3);
            break;
        case 4:
            m_dam->changeMode(1);
            break;
        case 5:
            m_dam->changeMode(1);
            break;
        case 6:
            m_dam->changeMode(1);
            break;
        case 7:
            m_dam->changeMode(1);
            break;
        default:
            break;
        }
        openMode =true;
        m_dam->show();
        m_WinFiel->hide();
        m_WinFiel->close();

        m_sendFile.close();
    }
    /* over */
//    QStorageInfo storage = QStorageInfo::root();
//    storage.refresh();
//    qDebug()<<storage.rootPath();
//    if(storage.isReadOnly())
//        qDebug()<<"isReadOnly:"<<storage.isReadOnly();
//    qDebug()<<"name:"<<storage.name();
//    qDebug()<<"fileSystemType:"<<storage.fileSystemType();
//    qDebug()<<"size:"<<storage.bytesTotal()/1000/1000<<"MB";
//    qDebug()<<"availableSize:"<<storage.bytesAvailable()/1000/1000<<"MB";
}

void MainWindow::sendChoose(int a )
{
//    m_port->AcsPrint(a);
}

void MainWindow::m_canPrintFile()
{
    //等待请求文件内容的指令中
    ui->stackedWidget->setCurrentIndex(36);
}

void MainWindow::checkFile()
{
//    printTimer->stop();
//    QObject::disconnect(printTimer,&QTimer::timeout,this,&MainWindow::checkFile);
//     m_port->startPrint(m_WinFiel->m_filePath);

}
void MainWindow::m_chooseUEN()
{
    m_WinFiel->hide();
    m_WinFiel->close();
}

void MainWindow::m_parcancel()
{
    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSixteen()));
    par->hide();
    par->close();
}

void MainWindow::m_cancel()
{
    skpWin->hide();
    skpWin->close();

}

void MainWindow::m_backPrint()
{
    m_mode->hide();
    m_mode->close();
    ui->stackedWidget->setCurrentIndex(34);
}

void MainWindow::m_whatThis()
{
    qDebug()<<"1";
    m_mode->hide();
    m_mode->close();
    ui->stackedWidget->setCurrentIndex(40);
    ui->m_StatusBar->setVisible(false);
}

void MainWindow::sensorCancel()
{
    m_sensor->hide();
    m_sensor->close();
}

void MainWindow::sensorConfirm()
{
    m_port->unFilament();
}

void MainWindow::restCancel()
{
    m_rest->hide();
    m_rest->close();
}

void MainWindow::restConfirm()
{
    m_port->factoryReset();
}

void MainWindow::wizardCancel()
{
    m_wizard->hide();
    m_wizard->close();
}

void MainWindow::wizardConfirm()
{
    m_wizard->hide();
    m_wizard->close();
    m_opaCity=0.0;
    m_effect=new QGraphicsOpacityEffect();
    m_effect->setOpacity(m_opaCity);
    ui->label->setGraphicsEffect(m_effect);
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(firstStart()));
    ui->stackedWidget->setCurrentIndex(0);
    m_timer.start(250);
}

void MainWindow::askStop()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEightteen()));
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFourteen()));
    }
    m_port->stopPrint();

}

void MainWindow::askPaused()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFourteen()));
    }

    m_port->pausePrint();
    skpWin->hide();
    skpWin->close();
    ui->stackedWidget->setCurrentIndex(39);
    ui->label_272->setVisible(false);
    ui->pushButton_167->setVisible(false);
    ui->pushButton_168->setVisible(false);


}

void MainWindow::fileList()
{


    QFileInfo usbfile(UDiskfind);
    if(usbfile.isDir())
    {

       ui->label_132->setVisible(true);
       ui->pushButton_101->setEnabled(true);
    }
    else
    {

        ui->label_132->setVisible(false);
        ui->pushButton_101->setEnabled(false);
        ui->listWidget_2->setVisible(false);
        ui->listWidget->setVisible(true);
    }

}

void MainWindow::carilbin()
{
    if((ui->label_125->text().left(3).toInt() >190 )&& (ui->label_127->text().left(3).toInt() >190))
    {
         m_port->p_nozzleHeating();
         QObject::disconnect(&m_timer,&QTimer::timeout,this,&MainWindow::carilbin);
         m_timer.stop();
         ui->stackedWidget->setCurrentIndex(54);
    }
}

void MainWindow::finished()
{
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::cancle()
{
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow:: nozzled()
{
    if((ui->label_125->text().left(3).toInt() >190 )&& (ui->label_127->text().left(3).toInt() >190 ))
    {
         m_port->n_nozzleHeating();
         QObject::disconnect(&m_timer,&QTimer::timeout,this,&MainWindow::nozzled);
         m_timer.stop();
         ui->stackedWidget->setCurrentIndex(62);
    }
}

void MainWindow::xyheated()
{
    if((ui->label_125->text().left(3).toInt() >190 )&& (ui->label_127->text().left(3).toInt() >190 ))
    {
         m_port->x_xyCalibration();
         QObject::disconnect(&m_timer,&QTimer::timeout,this,&MainWindow::xyheated);
         m_timer.stop();
         ui->stackedWidget->setCurrentIndex(67);
    }
}

void MainWindow::printending()
{
    ui->stackedWidget->setCurrentIndex(37);
    m_printsec->stop();
    m_time->setHMS(0,0,0);
}

void MainWindow::ltemp(QString a)
{
    if(a == "OFF")
    {
        lheatend = false;
        ui->pushButton_109->setText("000°C");
    }
    else {
        lheatend =true;
        ui->pushButton_109->setText(a);
    }
    QObject::disconnect(mchoose,&chooseTemp::heatT,this,&MainWindow::ltemp);
    mchoose->hide();
    mchoose->close();

    m_port->setFilament(ui->pushButton_109->text().mid(0,3),ui->pushButton_110->text().mid(0,3));


    if(m_timer.isActive())
    {
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
        m_timer.stop();
    }
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
    m_timer.start(1000);

}

void MainWindow::rtemp(QString a)
{
    if(a == "OFF")
    {
        rheatend = false;
        ui->pushButton_110->setText("000°C");
    }
    else {
        rheatend =true;
        ui->pushButton_110->setText(a);
    }
    QObject::disconnect(mchoose,&chooseTemp::heatT,this,&MainWindow::rtemp);
    mchoose->hide();
    mchoose->close();
    m_port->setFilament(ui->pushButton_109->text().mid(0,3),ui->pushButton_110->text().mid(0,3));

    if(m_timer.isActive())
    {
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
        m_timer.stop();
    }
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(filamentTimeout()));
    m_timer.start(1000);
}

void MainWindow::plt(QString a)
{
    QObject::disconnect(m_printfilament,&printFlament::heatT,this,&MainWindow::plt);
    m_printfilament->hide();
    m_printfilament->close();

    m_port->setFilament(ui->pushButton_263->text().mid(0,3),ui->pushButton_262->text().mid(0,3));
    ui->pushButton_263->setText(a);
}

void MainWindow::prt(QString a)
{
    QObject::disconnect(m_printfilament,&printFlament::heatT,this,&MainWindow::prt);
    m_printfilament->hide();
    m_printfilament->close();

    m_port->setFilament(ui->pushButton_263->text().mid(0,3),ui->pushButton_262->text().mid(0,3));
    ui->pushButton_262->setText(a);
}

void MainWindow::oneprint()
{
    this->printMode = 0;
    ui->label_36->setText("Direct Mode");
    ui->label_69->setText("Direct Mode");
    ui->label_308->setText("Direct Mode");
    ui->label_311->setText("Direct Mode");
    offset = QByteArray::fromHex("00000000");
    m_port->fileTemperature(lt,rt,bt);
    ui->stackedWidget->setCurrentIndex(35);
    m_modeone->hide();
    m_modeone->close();
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
    m_timer.start(100);
}

void MainWindow::oneparser()
{

}

void MainWindow::onecancel()
{
    m_modeone->hide();
    m_modeone->close();
}

void MainWindow::tprint()
{  
    ui->label_36->setText("Direct Mode");
    ui->label_69->setText("Direct Mode");
    ui->label_308->setText("Direct Mode");
    ui->label_311->setText("Direct Mode");
#ifdef OLD
    QString offsethex = QString::asprintf("%08x", offsetnum);
    qDebug()<<"string offset   "<<offsethex;
    QByteArray offsetarry= offsethex.toLatin1();
    qDebug()<<"arry offset   "<<offsetarry;
    offset.resize(0);
    offset.append(offsetarry[3]);
    offset.append(offsetarry[2]);
    offset.append(offsetarry[1]);
    offset.append(offsetarry[0]);
    qDebug()<<"offset send arry "<<this->offset;
#endif

    char offsetbuff[4]  = {0};
    offsetbuff[3] = static_cast< char>((offsetnum >> 24) & 0xFF);
    offsetbuff[2] = static_cast< char>((offsetnum >> 16) & 0xFF);
    offsetbuff[1] = static_cast< char>((offsetnum >> 8) & 0xFF);
    offsetbuff[0] = static_cast< char>((offsetnum >> 0) & 0xFF);
    this->offset.resize(0);
    this->offset.append(offsetbuff,4);
    m_port->fileTemperature(lt,rt,bt);
    ui->stackedWidget->setCurrentIndex(35);
    m_dam->hide();
    m_dam->close();
    m_dam=NULL;
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
    m_timer.start(100);
    m_printsec->start(1000);
}

void MainWindow::tcanle()
{

    if(m_dam != NULL)
    {
        m_dam->hide();
        m_dam->close();
        delete m_dam;
        m_dam = NULL;
    }
}

void MainWindow::tmirro()
{
    itemMode = 3;
    this->printMode = 3;
    if(openMode)
    {

        ui->label_36->setText("Mirror Mode");
        ui->label_69->setText("Mirror Mode");
        ui->label_308->setText("Mirror Mode");
        ui->label_311->setText("Mirror Mode");
        char offsetbuff[4]  = {0};
        offsetbuff[3] = static_cast< char>((offsetnum >> 24) & 0xFF);
        offsetbuff[2] = static_cast< char>((offsetnum >> 16) & 0xFF);
        offsetbuff[1] = static_cast< char>((offsetnum >> 8) & 0xFF);
        offsetbuff[0] = static_cast< char>((offsetnum >> 0) & 0xFF);
        this->offset.resize(0);
        this->offset.append(offsetbuff,4);
        m_port->fileTemperature(lt,lt,bt);
        ui->stackedWidget->setCurrentIndex(35);
        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
        m_timer.start(100);
        m_printsec->start(1000);
    }
    else
    {
        par =new parsing();
        if(par)
        {
            QObject::connect(par,&parsing::cancel,this,&MainWindow::buprint);
            m_deepTimer->start(1000);
            par->show();
        }
        m_fileParser->parseByDeep(udiskPATH,loaclPATH);
    }
    m_dam->hide();
    m_dam->close();
    m_dam=NULL;
}

void MainWindow::tdup()
{
    itemMode = 2;
    this->printMode = 2;
    if(openMode)
    {

        ui->label_36->setText("Duplicate Mode");
        ui->label_69->setText("Duplicate Mode");
        ui->label_308->setText("Duplicate Mode");
        ui->label_311->setText("Duplicate Mode");
        char offsetbuff[4]  = {0};
        offsetbuff[3] = static_cast< char>((offsetnum >> 24) & 0xFF);
        offsetbuff[2] = static_cast< char>((offsetnum >> 16) & 0xFF);
        offsetbuff[1] = static_cast< char>((offsetnum >> 8) & 0xFF);
        offsetbuff[0] = static_cast< char>((offsetnum >> 0) & 0xFF);
        this->offset.resize(0);
        this->offset.append(offsetbuff,4);
        m_port->fileTemperature(lt,lt,bt);
        ui->stackedWidget->setCurrentIndex(35);
        QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
        m_timer.start(100);
        m_printsec->start(1000);
    }
    else {
        par =new parsing(this);
        par->show();
        QObject::connect(par,&parsing::cancel,this,&MainWindow::buprint);
        m_deepTimer->start(1000);
        m_fileParser->parseByDeep(udiskPATH,loaclPATH);
    }
    m_dam->hide();
    m_dam->close();
    m_dam=NULL;
}

void MainWindow::dprint()
{
    printMode = 0;
    ui->label_36->setText("Direct Mode");
    ui->label_69->setText("Direct Mode");
    ui->label_308->setText("Direct Mode");
    ui->label_311->setText("Direct Mode");
    offset = QByteArray::fromHex("00000000");
    m_port->fileTemperature(lt,rt,bt);
    ui->stackedWidget->setCurrentIndex(35);
    m_dup->hide();
    m_dup->close();
    m_dup=NULL;
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
    m_timer.start(100);
}

void MainWindow::ddup()
{
#ifdef OLD
    printMode = 2;
    ui->label_36->setText("Duplicate mode");
    ui->label_69->setText("Duplicate mode");
    ui->label_308->setText("Duplicate mode");
    ui->label_311->setText("Duplicate mode");
    QString offsethex = QString::asprintf("%x", offsetnum);
    QByteArray offsetarry= QByteArray::fromHex(offsethex.toLatin1());
    offset ="";
    offset.append(offsetarry[3]);
    offset.append(offsetarry[2]);
    offset.append(offsetarry[1]);
    offset.append(offsetarry[0]);

    m_port->fileTemperature(lt,lt,bt);
    ui->stackedWidget->setCurrentIndex(35);
    m_dup->hide();
    m_dup->close();
    m_dup=NULL;
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
    m_timer.start(100);
#endif
}

void MainWindow::dcancle()
{
    m_dup->hide();
    m_dup->close();
    m_dup=NULL;
}

void MainWindow::powerlost()
{
    m_power = new powerLost(this);
    m_power->show();
    QObject::connect(m_power,&powerLost::cancel,this,&MainWindow::powercancle);
}

void MainWindow::filamentlost()
{
    m_port->selfTest();
}

void MainWindow::error(int a )
{
    switch (a) {
    case 4:
        m_filamentfault = new  filamentFault(this);
        m_filamentfault->show();
        QObject::connect(m_filamentfault,&filamentFault::abort,this,&MainWindow::Fabort);
        QObject::connect(m_filamentfault,&filamentFault::resume,this,&MainWindow::Fresume);
        QObject::connect(m_filamentfault,&filamentFault::changeFilament,this,&MainWindow::Fchangefilament);
        break;
    case 5:
        m_filamentfault = new  filamentFault(this);
        m_filamentfault->show();
        QObject::connect(m_filamentfault,&filamentFault::abort,this,&MainWindow::Fabort);
        QObject::connect(m_filamentfault,&filamentFault::resume,this,&MainWindow::Fresume);
        QObject::connect(m_filamentfault,&filamentFault::changeFilament,this,&MainWindow::Fchangefilament);
        break;
    default:
        break;
    }
}

void MainWindow::Fresume()
{
    m_port->resume();
}

void MainWindow::Fabort()
{
    m_port->abort();
}

void MainWindow::Fchangefilament()
{
    ui->stackedWidget->setCurrentIndex(80);
    m_filamentfault->hide();
    m_filamentfault->close();
}

void MainWindow::selftest1()
{
    ui->label_164->setPixmap(QPixmap(":/testok.png"));
    m_port->selftest2();
    ui->label_192->setPixmap(QPixmap(":/wait.png"));

}

void MainWindow::selftest2()
{
ui->label_192->setPixmap(QPixmap(":/testok.png"));
m_port->selftest3();
ui->label_193->setPixmap(QPixmap(":/wait.png"));

}

void MainWindow::selftest3()
{
ui->label_193->setPixmap(QPixmap(":/testok.png"));
m_port->selftest4();
ui->label_224->setPixmap(QPixmap(":/wait.png"));

}

void MainWindow::selftest4()
{
ui->label_224->setPixmap(QPixmap(":/testok.png"));
m_port->selftest5();
ui->label_225->setPixmap(QPixmap(":/wait.png"));

}

void MainWindow::selftest5()
{
    ui->label_225->setPixmap(QPixmap(":/testok.png"));
    ui->pushButton_348->setVisible(true);
    ui->pushButton_347->setVisible(true);


    ui->pushButton_676->setEnabled(true);
    ui->pushButton_677->setEnabled(true);
    ui->pushButton_678->setEnabled(true);
    ui->pushButton_679->setEnabled(true);

    ui->stackedWidget->setCurrentIndex(85);
//    m_port->selfTest();
//    ui->label_285->setPixmap(QPixmap(":/wait.png"));
}

void MainWindow::selftest6()
{
    ui->label_285->setPixmap(QPixmap(":/testok.png"));

    ui->pushButton_348->setVisible(true);
    ui->pushButton_347->setVisible(true);


    ui->pushButton_676->setEnabled(true);
    ui->pushButton_677->setEnabled(true);
    ui->pushButton_678->setEnabled(true);
    ui->pushButton_679->setEnabled(true);

}

void MainWindow::setclose(int a)
{
    m_setdlog->hide();
    m_setdlog->close();
    m_setdlog =NULL;
    m_port->printcom(a);
    m_printsec->start(1000);
}

void MainWindow::parsetclose(int a, int b)
{
    m_port->parcom(b);
    qDebug()<<"2";
    if(m_parsetdlog!=NULL)
    {
        m_parsetdlog->hide();
        m_parsetdlog->close();
        m_parsetdlog= NULL;
    }
    m_printsec->start(1000);

}

void MainWindow::parsetexclose(bool l, bool r,bool b)
{
    m_port->enHotend(l,r);
    qDebug()<<"2";
    if(m_parsetdlog!=NULL)
    {
        m_parsetdlog->hide();
        m_parsetdlog->close();
        m_parsetdlog= NULL;
    }
    m_printsec->start(1000);
}

void MainWindow::paracktiv(bool a)
{
    qDebug()<<"3";
    m_port->enbackup(a);
}

void MainWindow::findhotend()
{
    m_port->askHotend();
    m_port->backuptsend();
}

void MainWindow::cannext(bool )
{
    if(m_parsetdlog!=NULL)
    {
        m_parsetdlog->seten(true,true);
    }
}

void MainWindow::state(QString a, QString b, QString c, QString d, QString e, QString f, QString g)
{
    if(m_setdlog != NULL)
    {
        m_setdlog->initnum(a,b,c,d,e,f,g);
    }
    if(m_parsetdlog != NULL)
    {
        m_parsetdlog->initnum(a,b,c,d,e,f,g);
    }
}

void MainWindow::canone(int a)
{
    if(m_setdlog != NULL)
    {
        m_setdlog->backup(a);
    }
    if(m_parsetdlog != NULL)
    {
        m_parsetdlog->backup(a);
    }
    qDebug()<<"ma1"<<a;
}

void MainWindow::cantwo(int a)
{
    if(m_parsetdlog != NULL)
    {
        m_parsetdlog->backupt(a);
    }
    qDebug()<<"ma2"<<a;
}

void MainWindow::up()
{
    m_port->backuptsend();
}

void MainWindow::down()
{
    m_port->backupsend();
}

void MainWindow::updatebegin()
{
    /*进度条*/
    m_update = new updateProgreBar();
    QObject::connect(this,&MainWindow::updateNum,m_update,&updateProgreBar::change);
    m_update->show();

}



void MainWindow::updateNumx(int q )
{
    emit updateNum(q);
}

void MainWindow::parseMode(QString printMode)
{
    int mode = 0;
    qDebug()<<"print Mode:"<<printMode;
    if(printMode == "unknown"||printMode == "Unknown")
    {
        mode = 1;
        this->printMode = 0;
    }
    if(printMode == "Mirror")
    {
        mode = 2;
    }
    if(printMode == "Duplicate")
    {
        mode = 3;
    }
    if(printMode == "Unsupport")
    {
        mode = 4;
        this->printMode = 0;
    }
    if(printMode == "Mix")
    {
        mode = 5;
        this->printMode = 6;
    }
    if(printMode == "Orgin-Mirror")
    {
        mode = 6;
        this->printMode = 5;
    }
    if(printMode == "Orgin-Duplicate")
    {
        mode = 7;
        this->printMode = 4;
    }
    qDebug()<<"mode num"<<mode;
    if(m_dam == NULL)
    {
        m_dam = new DupandMirorr(this);
        QObject::connect(m_dam,&DupandMirorr::print,this,&MainWindow::tprint);
        QObject::connect(m_dam,&DupandMirorr::mirro,this,&MainWindow::tmirro);
        QObject::connect(m_dam,&DupandMirorr::dup,this,&MainWindow::tdup);
        QObject::connect(m_dam,&DupandMirorr::cancle,this,&MainWindow::tcanle);
        switch (mode) {
        case 1:
            m_dam->changeMode(0);
            break;
        case 2:
            m_dam->changeMode(2);
            break;
        case 3:
            m_dam->changeMode(3);
            break;
        case 4:
            m_dam->changeMode(1);
            break;
        case 5:
            m_dam->changeMode(1);
            break;
        case 6:
            m_dam->changeMode(1);
            break;
        case 7:
            m_dam->changeMode(1);
            break;
        default:
            break;
        }
        m_dam->show();
    }
    if(m_WinFiel)
    {
        m_WinFiel->hide();
        m_WinFiel->close();
    }

}

void MainWindow::parseDeepMode(QString printMode)
{
    qDebug()<<"parseDeepMode-----"<<itemMode;
    if(itemMode == 3){
        qDebug()<<"deep mode*****"<<printMode<<"3";
        if(printMode == "Mirror")
        {
            ui->label_36->setText("Mirror Mode");
            ui->label_69->setText("Mirror Mode");
            ui->label_308->setText("Mirror Mode");
            ui->label_311->setText("Mirror Mode");
            this->printMode = 3;
            m_port->fileTemperature(lt,lt,bt);
            ui->stackedWidget->setCurrentIndex(35);
            QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
            m_timer.start(100);
        }
        else
        {
            int mode = 0;
            if(printMode == "unknown"||printMode == "Unknown")
            {
                mode = 1;
                this->printMode = 0;
            }
            if(printMode == "Mirror")
            {
                mode = 2;
            }
            if(printMode == "Duplicate")
            {
                mode = 3;
            }
            if(printMode == "Unsupport")
            {
                mode = 4;
                this->printMode = 0;
            }
            if(printMode == "Mix")
            {
                mode = 5;
                this->printMode = 6;
            }
            if(printMode == "Orgin-Mirror")
            {
                mode = 6;
                this->printMode = 5;
            }
            if(printMode == "Orgin-Duplicate")
            {
                mode = 7;
                this->printMode = 4;
            }
            if(m_dam == NULL)
            {
                m_dam = new DupandMirorr(this);
                QObject::connect(m_dam,&DupandMirorr::print,this,&MainWindow::tprint);
                QObject::connect(m_dam,&DupandMirorr::mirro,this,&MainWindow::tmirro);
                QObject::connect(m_dam,&DupandMirorr::dup,this,&MainWindow::tdup);
                QObject::connect(m_dam,&DupandMirorr::cancle,this,&MainWindow::tcanle);
                switch (mode) {
                case 1:
                    m_dam->changeMode(0);
                    break;
                case 2:
                    m_dam->changeMode(2);
                    break;
                case 3:
                    m_dam->changeMode(3);
                    break;
                case 4:
                    m_dam->changeMode(1);
                    break;
                case 5:
                    m_dam->changeMode(1);
                    break;
                case 6:
                    m_dam->changeMode(1);
                    break;
                case 7:
                    m_dam->changeMode(1);
                    break;
                default:
                    break;
                }
                m_dam->show();
            }
        }
    }
    if(itemMode == 2)
    {
        qDebug()<<"deep mode*****"<<printMode<<"2";
        if(printMode == "Mirror"||printMode == "Duplicate")
        {
            this->printMode = 2;
            ui->label_36->setText("Duplicate Mode");
            ui->label_69->setText("Duplicate Mode");
            ui->label_308->setText("Duplicate Mode");
            ui->label_311->setText("Duplicate Mode");
            m_port->fileTemperature(lt,lt,bt);
            ui->stackedWidget->setCurrentIndex(35);
            QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
            m_timer.start(100);
        }
        else {
            int mode = 0;
            if(printMode == "unknown"||printMode == "Unknown")
            {
                mode = 1;
                this->printMode = 0;
            }
            if(printMode == "Mirror")
            {
                mode = 2;
            }
            if(printMode == "Duplicate")
            {
                mode = 3;
            }
            if(printMode == "Unsupport")
            {
                mode = 4;
                this->printMode = 0;
            }
            if(printMode == "Mix")
            {
                mode = 5;
                this->printMode = 6;
            }
            if(printMode == "Orgin-Mirror")
            {
                mode = 6;
                this->printMode = 5;
            }
            if(printMode == "Orgin-Duplicate")
            {
                mode = 7;
                this->printMode = 4;
            }
            if(m_dam == NULL)
            {
                m_dam = new DupandMirorr(this);
                QObject::connect(m_dam,&DupandMirorr::print,this,&MainWindow::tprint);
                QObject::connect(m_dam,&DupandMirorr::mirro,this,&MainWindow::tmirro);
                QObject::connect(m_dam,&DupandMirorr::dup,this,&MainWindow::tdup);
                QObject::connect(m_dam,&DupandMirorr::cancle,this,&MainWindow::tcanle);
                switch (mode) {
                case 1:
                    m_dam->changeMode(0);
                    break;
                case 2:
                    m_dam->changeMode(2);
                    break;
                case 3:
                    m_dam->changeMode(3);
                    break;
                case 4:
                    m_dam->changeMode(1);
                    break;
                case 5:
                    m_dam->changeMode(1);
                    break;
                case 6:
                    m_dam->changeMode(1);
                    break;
                case 7:
                    m_dam->changeMode(1);
                    break;
                default:
                    break;
                }
                m_dam->show();
            }
        }
    }

}

void MainWindow::parseHeader(QString left_temp, QString right_temp, QString bed_temp, QString offset)
{
    qDebug()<<"parseHeader";
    lt = left_temp;
    rt = right_temp;
    bt = bed_temp;
    QString offseta = offset;
    if(offseta.contains("-"))
    {
        offseta = offseta.mid(1,offseta.size()-1);
        qDebug()<<"offset"<<offseta;
        float a = offseta.toFloat();
        offsetnum = 0 - (a*1000);
        qDebug()<<"num"<<offsetnum;
    }
    else {
        float a = offseta.toFloat();
        offsetnum = a*1000;
    }
    char offsetbuff[4]  = {0};
    offsetbuff[3] = static_cast< char>((offsetnum >> 24) & 0xFF);
    offsetbuff[2] = static_cast< char>((offsetnum >> 16) & 0xFF);
    offsetbuff[1] = static_cast< char>((offsetnum >> 8) & 0xFF);
    offsetbuff[0] = static_cast< char>((offsetnum >> 0) & 0xFF);
    this->offset.resize(0);
    this->offset.append(offsetbuff,4);
    qDebug()<<"void MainWindow::parseHeader"<<offset<<offsetnum<<this->offset;
}

void MainWindow::parseDeep()
{
    m_printsec->start(1000);
}

void MainWindow::timeAdd()
{
    *m_time =  m_time->addSecs(1);
    ui->label_78->setText(QString::number(m_time->hour())+"H"+QString::number(m_time->minute())+"M"+QString::number(m_time->second())+"S");
    ui->label_307->setText(QString::number(m_time->hour())+"H"+QString::number(m_time->minute())+"M"+QString::number(m_time->second())+"S");
    ui->label_310->setText(QString::number(m_time->hour())+"H"+QString::number(m_time->minute())+"M"+QString::number(m_time->second())+"S");
    ui->label_312->setText(QString::number(m_time->hour())+"H"+QString::number(m_time->minute())+"M"+QString::number(m_time->second())+"S");
}

void MainWindow::deepTimer()
{
    int x= m_fileParser->GetParsedLine();
    qDebug()<<"m_deep timer num "<<x;
    par->setNum(x);
    if(x == 100)
    {
        m_deepTimer->stop();
        //        QObject::disconnect(m_deepTimer,&QTimer::timeout,this,&MainWindow::deepTimer);
        /*判断是否需要重新选择*/
        par->hide();
        par->close();
        par = NULL;
        m_dam->hide();
        m_dam->close();
        delete  m_dam;
        int mode = 0;
        int x1=0;
        int x2=0;
        int x3=0;
        int x4=0;
        int x5=0;
        QFile m_sendFile(this->loaclPATH);
        if(m_sendFile.open(QIODevice::ReadOnly))
        {
            QByteArray m_filedata = m_sendFile.readLine();
            QByteArray y(" ");
            x1 = m_filedata.indexOf(y);
            x2 = m_filedata.indexOf(y,x1+1);
            x3 = m_filedata.indexOf(y,x2+1);
            x4 = m_filedata.indexOf(y,x3+1);
            x5 = m_filedata.indexOf(y,x4+1);

            QString data = m_filedata;
            if(data.mid(x1+12,x2-x1-12) == "unknown"||data.mid(x1+12,x2-x1-12) =="Unknown" )
            {
                mode = 1;
                this->printMode = 0;
            }
            if(data.mid(x1+12,x2-x1-12) == "Mirror")
            {
                mode = 2;
            }
            if(data.mid(x1+12,x2-x1-12) == "Duplicate")
            {
                mode = 3;
            }
            if(data.mid(x1+12,x2-x1-12) == "Unsupport")
            {
                mode = 4;
                this->printMode = 0;
            }
            if(data.mid(x1+12,x2-x1-12) == "Mix")
            {
                mode = 5;
                this->printMode = 6;
            }
            if(data.mid(x1+12,x2-x1-12) == "Orgin-Mirror")
            {
                mode = 6;
                this->printMode = 5;
            }
            if(data.mid(x1+12,x2-x1-12) == "Orgin-Duplicate")
            {
                mode = 7;
                this->printMode = 4;
            }
            lt = data.mid(x3+11,x4-x3-11);
            rt = data.mid(x4+12,x5-x4-12);
            bt = data.mid(x5+10,data.size()-x5-10);
            QString offset = data.mid(x2+8,x3-x2-8);
#ifdef OLD
            if(offset.contains("-"))
            {
                offset = offset.mid(1,offset.size()-1);
                qDebug()<<"offset"<<offset;
                float a = offset.toFloat();
                offsetnum = 0 - (a*1000);
                qDebug()<<"num"<<offsetnum;
                //            Duplicate_X_Offset = QString::number(offsetnum,10);
            }
            else
            {
                offsetnum = offset.toInt();
            }
#endif
            if(offset.contains("-"))
            {
                offset = offset.mid(1,offset.size()-1);
                qDebug()<<"offset"<<offset;
                float a = offset.toFloat();
                offsetnum = 0 - (a*1000);
                qDebug()<<"num"<<offsetnum;
            }
            else {
                float a = offset.toFloat();
                offsetnum = a*1000;
                qDebug()<<"num"<<offsetnum;
            }
        }
#ifdef OLD
        if(0 == 4)
        {
            ui->label_36->setText("Direct Mode");
            ui->label_69->setText("Direct Mode");
            ui->label_308->setText("Direct Mode");
            ui->label_311->setText("Direct Mode");
//            this->offset = QByteArray::fromHex("00000000");
            QString offsethex = QString::asprintf("%x", offsetnum);
            QByteArray offsetarry= QByteArray::fromHex(offsethex.toLatin1());
            offset ="";
            offset.append(offsetarry[3]);
            offset.append(offsetarry[2]);
            offset.append(offsetarry[1]);
            offset.append(offsetarry[0]);
            m_port->fileTemperature(lt,rt,bt);
            ui->stackedWidget->setCurrentIndex(35);
            QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
            m_timer.start(100);
            m_printsec->start(1000);
        }
        else
        {
            m_dam = new DupandMirorr(this);
            QObject::connect(m_dam,&DupandMirorr::print,this,&MainWindow::tprint);
            QObject::connect(m_dam,&DupandMirorr::mirro,this,&MainWindow::tmirro);
            QObject::connect(m_dam,&DupandMirorr::dup,this,&MainWindow::tdup);
            QObject::connect(m_dam,&DupandMirorr::cancle,this,&MainWindow::tcanle);
            switch (mode) {
            case 1:
                m_dam->changeMode(0);
                break;
            case 2:
                m_dam->changeMode(2);
                break;
            case 3:
                m_dam->changeMode(3);
                break;
            case 4:
                m_dam->changeMode(1);
                break;
            case 5:
                m_dam->changeMode(1);
                break;
            case 6:
                m_dam->changeMode(1);
                break;
            case 7:
                m_dam->changeMode(1);
                break;
            default:
                break;
            }
            m_dam->show();
            openMode = true;
        }
#endif
    }
}

void MainWindow::on_noreadynext_clicked()
{
    /*G4-1按钮槽函数*/
    ui->stackedWidget->setCurrentIndex(8);
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFive()));
    m_timer.start(2000);
}

void MainWindow::on_platnext_clicked()
{
    /*G4-1-3按钮槽函数*/
    ui->stackedWidget->setCurrentIndex(12);
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEight()));
    m_timer.start(2000);

}

void MainWindow::on_readynext_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);
    ui->m_StatusBar->setVisible(true);
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEleven()));
    m_timer.start(3000);
}

void MainWindow::on_pushButton_12_clicked()
{
    /*第二个PAGEVIEW界面*/
    ui->stackedWidget->setCurrentIndex(18);
    ui->quickWidget_2->setSource(QUrl("qrc:/pageView.qml"));
    ui->quickWidget_2->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget_2->show();
}

void MainWindow::on_pushButton_23_clicked()
{
    /*G6按钮闪烁*/
    ui->stackedWidget->setCurrentIndex(19);
    if(m_timer.isActive())
    {
        m_timer.stop();
    }
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(shineone()));
    flicker =true;
    timeLonger = 0;
    m_opaCity = 0.0;
    m_effect->setOpacity(m_opaCity);
    m_timer.start(50);

}

void MainWindow::on_pushButton_24_clicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(shineone()));
    }
      ui->stackedWidget->setCurrentIndex(20);
      ui->widget_2->setVisible(false);
      ui->label_43->setVisible(false);
      ui->pushButton_42->setVisible(false);

}

void MainWindow::on_pushButton_41_clicked()
{
    ui->label_43->setVisible(true);
    ui->pushButton_42->setVisible(true);

    ui->label_44->setVisible(false);
    ui->pushButton_41->setVisible(false);
    ui->pushButton_44->setVisible(false);

    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(shinetwo()));
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumptwelve()));
    m_timer.start(2000);
}

void MainWindow::on_pushButton_31_clicked()
{
    ui->m_StatusBar->setVisible(false);
    ui->widget_2->setVisible(true);
    if(m_timer.isActive())
    {
        m_timer.stop();
    }
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(shinetwo()));
    flicker =true;
    timeLonger = 0;
    m_opaCity = 0.0;
    m_effect->setOpacity(m_opaCity);
    m_timer.start(50);
}

void MainWindow::on_pushButton_87_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFifteen()));
        m_x=0;
        m_y=0;
        m_w=0;
        m_h=0;
        timeLonger=0;
        flicker =true;
    }
}

void MainWindow::on_pushButton_52_clicked()
{
    ui->stackedWidget->setCurrentIndex(25);
}

void MainWindow::on_pushButton_53_clicked()
{
    ui->stackedWidget->setCurrentIndex(26);
}

void MainWindow::on_pushButton_62_clicked()
{
    ui->stackedWidget->setCurrentIndex(27);
}

void MainWindow::on_pushButton_63_clicked()
{
    ui->stackedWidget->setCurrentIndex(28);
}

void MainWindow::on_pushButton_64_clicked()
{
    ui->stackedWidget->setCurrentIndex(29);
}

void MainWindow::on_pushButton_76_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_72_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_68_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_60_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_56_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}


void MainWindow::on_pushButton_74_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_73_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_129_clicked()
{
    ui->stackedWidget->setCurrentIndex(34);
    ui->listWidget->setVisible(true);
    ui->listWidget_2->setVisible(false);

}

void MainWindow::on_pushButton_146_clicked()
{
    m_timer.stop();
    skpWin = new askPause(this);
    QObject::connect(skpWin,SIGNAL(cancel()),this,SLOT(m_cancel()));
    QObject::connect(skpWin,SIGNAL(m_stop()),this,SLOT(askStop()));
    QObject::connect(skpWin,SIGNAL(m_paused()),this,SLOT(askPaused()));
    skpWin->show();
}

void MainWindow::on_pushButton_147_clicked()
{
  m_setdlog = new PrintSetDlog(this);
  m_setdlog->show();
  QObject::connect(m_setdlog,&PrintSetDlog::closeset,this,&MainWindow::setclose);
  m_port->askstate();
  m_port->backupsend();
  m_printsec->stop();
}

void MainWindow::on_pushButton_134_clicked()
{
    ui->pushButton_134->setStyleSheet("QPushButton{border-top-left-radius: 20px;\
                                      border-bottom-left-radius: 20px;\
                                      border-left: 2px solid rgb(81, 83, 86);\
                                      border-top: 2px solid rgb(81, 83, 86);\
                                      border-bottom: 2px solid rgb(81, 83, 86);\
                                      background-color: rgba(45, 44, 43, 255);\
                                      font-weight: bold;\
                                      font-family:Barlow;outline:none;\
                                      color: rgb(255, 255, 255);\
                                      font-size: 32px;\
};");
    ui->pushButton_101->setStyleSheet("QPushButton{\
                                      background-color: rgb(32, 32, 32);\
                                      font-weight: bold;\
                                      font-family:Barlow;outline:none;\
                                      color: rgb(255, 255, 255);\
                                      font-size: 32px;\
                                      border-top-right-radius: 20px;\
                                      border-bottom-right-radius: 20px;\
                                      border-right: 2px solid rgb(81, 83, 86);\
                                      border-top: 2px solid rgb(81, 83, 86);\
                                      border-bottom: 2px solid rgb(81, 83, 86);\
};");
    ui->listWidget->setVisible(true);
    ui->listWidget_2->setVisible(false);

    ui->listWidget->clear();
    QDir *m_dir=new QDir(localPath);
    QStringList filter;
    QFileInfoList m_fileinfo = m_dir->entryInfoList();

    int i = 0;
    for(i = 0;i< m_fileinfo.count();i++)
    {
        m_addItemToList(m_fileinfo.at(i).fileName(),m_fileinfo.at(i).filePath());

    }

}
//void MainWindow::on_pushButton_135_clicked()
//{

//}

void MainWindow::on_pushButton_169_clicked()
{
    ui->stackedWidget->setCurrentIndex(34);
    ui->m_StatusBar->setVisible(true);
    m_mode = new selectMode(this);
    m_mode->show();
    QObject::connect(m_mode,SIGNAL(m_back()),this,SLOT(m_backPrint()));
    QObject::connect(m_mode,SIGNAL(m_whatThis()),this,SLOT(m_whatThis()));
}

void MainWindow::on_pushButton_174_clicked()
{
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::on_pushButton_175_clicked()
{
    ui->stackedWidget->setCurrentIndex(71);
}

void MainWindow::on_pushButton_176_clicked()
{
    ui->stackedWidget->setCurrentIndex(42);
}

void MainWindow::on_pushButton_177_clicked()
{
    ui->stackedWidget->setCurrentIndex(74);
}

void MainWindow::on_pushButton_182_clicked()
{
    ui->stackedWidget->setCurrentIndex(43);
}

void MainWindow::on_pushButton_183_clicked()
{
    ui->stackedWidget->setCurrentIndex(48);
}

void MainWindow::on_pushButton_188_clicked()
{
    ui->stackedWidget->setCurrentIndex(44);
}

void MainWindow::on_pushButton_189_clicked()
{
    ui->stackedWidget->setCurrentIndex(42);
}

void MainWindow::on_pushButton_194_clicked()
{
    ui->stackedWidget->setCurrentIndex(45);
}

void MainWindow::on_pushButton_195_clicked()
{
    ui->stackedWidget->setCurrentIndex(43);

}

void MainWindow::on_pushButton_200_clicked()
{
    ui->stackedWidget->setCurrentIndex(46);
}

void MainWindow::on_pushButton_201_clicked()
{
    ui->stackedWidget->setCurrentIndex(44);
}

void MainWindow::on_pushButton_206_clicked()
{
    ui->stackedWidget->setCurrentIndex(47);
}

void MainWindow::on_pushButton_207_clicked()
{
    ui->stackedWidget->setCurrentIndex(45);
}

void MainWindow::on_pushButton_212_clicked()
{
    ui->stackedWidget->setCurrentIndex(48);
}

void MainWindow::on_pushButton_213_clicked()
{
    ui->stackedWidget->setCurrentIndex(46);
}

void MainWindow::on_pushButton_218_clicked()
{
    ui->stackedWidget->setCurrentIndex(49);
}

void MainWindow::on_pushButton_219_clicked()
{
    ui->stackedWidget->setCurrentIndex(47);
}

void MainWindow::on_pushButton_224_clicked()
{
    ui->stackedWidget->setCurrentIndex(50);
}

void MainWindow::on_pushButton_225_clicked()
{
    ui->stackedWidget->setCurrentIndex(48);
}

void MainWindow::on_pushButton_230_clicked()
{
    ui->stackedWidget->setCurrentIndex(42);
}

void MainWindow::on_pushButton_235_clicked()
{
    ui->stackedWidget->setCurrentIndex(52);
}

void MainWindow::on_pushButton_236_clicked()
{
    ui->stackedWidget->setCurrentIndex(60);
}

void MainWindow::on_pushButton_237_clicked()
{
    ui->stackedWidget->setCurrentIndex(65);
}

void MainWindow::on_pushButton_242_clicked()
{
    m_port->setFilament("200","200");
    ui->stackedWidget->setCurrentIndex(53);
    QObject::connect(&m_timer,&QTimer::timeout,this,&MainWindow::carilbin);
    m_timer.start(1000);
}

//void MainWindow::on_pushButton_258_clicked()
//{
//    m_port->finish();
//}

void MainWindow::on_pushButton_259_clicked()
{
    ui->stackedWidget->setCurrentIndex(54);
    m_port->p_platformCalibration();
}

void MainWindow::on_pushButton_286_clicked()
{
     ui->stackedWidget->setCurrentIndex(61);
     m_port->setFilament("200","200");
     QObject::connect(&m_timer,&QTimer::timeout,this,&MainWindow::nozzled);
     m_timer.start(1000);
}

void MainWindow::on_pushButton_287_clicked()
{
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::on_pushButton_302_clicked()
{
    m_port->setFilament("0","0");
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::on_pushButton_303_clicked()
{
    ui->stackedWidget->setCurrentIndex(62);
//    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jump23()));
//    m_timer.start(2000);
    m_port->n_nozzleCalibration();
}

void MainWindow::on_pushButton_631_clicked()
{
    ui->stackedWidget->setCurrentIndex(66);
    m_port->setFilament("200","200");
    QObject::connect(&m_timer,&QTimer::timeout,this,&MainWindow::xyheated);
    m_timer.start(1000);

}

void MainWindow::on_pushButton_632_clicked()
{
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::on_pushButton_647_clicked()
{
    m_port->x_platformCalibration();
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::on_pushButton_3_clicked()
{
    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEleven()));
     ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_17_clicked()
{
     ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_233_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_180_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_5_clicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEleven()));
    }
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEleven()));
    }
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_4_clicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEleven()));
    }
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(73);
}

void MainWindow::on_pushButton_16_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_18_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_90_clicked()
{
    /*start 第一动画点击设置*/
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpthirteen()));
        ui->stackedWidget->setCurrentIndex(38);
        isPrintFirstAnim = true;
        isStartFristAnim = true;
    }
}

void MainWindow::on_pushButton_92_clicked()
{
    /*start 第二动画点击设置*/
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFourteen()));
        isPrintFirstAnim = false;
        isStartFristAnim = true;
        ui->stackedWidget->setCurrentIndex(38);
    }
}

void MainWindow::on_pushButton_91_clicked()
{
    /*start 第二动画点击暂停*/
    isStartFristAnim = true;
    if(m_timer.isActive())
    {
        m_timer.stop();
        //QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFourteen()));
        skpWin = new askPause(this);
        skpWin->show();
    }
    QObject::connect(skpWin,SIGNAL(cancel()),this,SLOT(m_cancel()));
    QObject::connect(skpWin,SIGNAL(m_stop()),this,SLOT(askStop()));
    QObject::connect(skpWin,SIGNAL(m_paused()),this,SLOT(askPaused()));
}

void MainWindow::on_pushButton_85_clicked()
{

    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_86_clicked()
{
//    if(m_timer.isActive())
//    {
//        m_timer.stop();
//        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFifteen()));
//        m_x=0;
//        m_y=0;
//        m_w=0;
//        m_h=0;
//        timeLonger=0;
//        flicker =true;
//    }
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_88_clicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFifteen()));
        m_x=0;
        m_y=0;
        m_w=0;
        m_h=0;
        timeLonger=0;
        flicker =true;
    }
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_117_clicked()
{

}

void MainWindow::on_pushButton_116_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_48_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_49_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_50_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_54_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_55_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_57_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_58_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_59_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_61_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_65_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_66_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_67_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_69_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_70_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_71_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_75_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_170_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_172_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_173_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_314_clicked()
{
    ui->stackedWidget->setCurrentIndex(67);
}

void MainWindow::on_pushButton_315_clicked()
{
    ui->stackedWidget->setCurrentIndex(68);
}

void MainWindow::on_pushButton_316_clicked()
{
    ui->stackedWidget->setCurrentIndex(68);
}

void MainWindow::on_pushButton_649_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_650_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_648_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_653_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_654_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_652_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_657_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_658_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_656_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_341_clicked()
{
    ui->stackedWidget->setCurrentIndex(72);
}

void MainWindow::on_pushButton_670_clicked()
{
    ui->stackedWidget->setCurrentIndex(34);
}

void MainWindow::on_pushButton_668_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_669_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_671_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_667_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_662_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_659_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_655_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_651_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_179_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_181_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_178_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_186_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_185_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_187_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_193_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_190_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_191_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_199_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_197_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_198_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_202_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_203_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_205_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_210_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_209_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_211_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_216_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_214_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_215_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_220_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_223_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_221_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_227_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_226_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_228_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_243_clicked()
{
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::on_pushButton_231_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_232_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_234_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_240_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_238_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_239_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_241_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_248_clicked()
{
    m_port->setFilament("0","0");
    ui->stackedWidget->setCurrentIndex(51);
}

void MainWindow::on_pushButton_256_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_257_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_254_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_255_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_284_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_285_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_282_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_283_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_292_clicked()
{
    m_port->setFilament("0","0");
    ui->stackedWidget->setCurrentIndex(51);
}

//void MainWindow::on_pushButton_297_clicked()
//{
//    m_port->setFilament("0","0");
//    ui->stackedWidget->setCurrentIndex(51);
//}

void MainWindow::on_pushButton_298_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_301_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_300_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_299_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_628_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_629_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_627_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_630_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_637_clicked()
{
    m_timer.stop();
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jump24()));
    ui->stackedWidget->setCurrentIndex(51);
}

//void MainWindow::on_pushButton_642_clicked()
//{
//    m_timer.stop();
//    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jump25()));
//    ui->stackedWidget->setCurrentIndex(51);
//}

void MainWindow::on_pushButton_643_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_644_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_646_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_645_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_347_clicked()
{
    ui->label_164->clear();
    ui->label_192->clear();
    ui->label_193->clear();
    ui->label_224->clear();
    ui->label_225->clear();
    ui->label_285->clear();

    ui->pushButton_348->setVisible(false);
    ui->pushButton_347->setVisible(false);


    ui->pushButton_676->setEnabled(false);
    ui->pushButton_677->setEnabled(false);
    ui->pushButton_678->setEnabled(false);
    ui->pushButton_679->setEnabled(false);
    m_port->selftest1();
    ui->label_164->setPixmap(QPixmap(":/wait.png"));

}

void MainWindow::on_pushButton_342_clicked()
{
    ui->stackedWidget->setCurrentIndex(76);
}

void MainWindow::on_pushButton_348_clicked()
{
    ui->stackedWidget->setCurrentIndex(74);

    ui->label_164->clear();
    ui->label_192->clear();
    ui->label_193->clear();
    ui->label_224->clear();
    ui->label_225->clear();
    ui->label_285->clear();
}

void MainWindow::on_pushButton_349_clicked()
{
#ifdef XH_LINUX
    ui->listWidget_3->clear();
    char buff[2048]="";
    int i = sizeof(buff);
    if(aw_wifi->get_scan_results(buff,&i) == 0)
    {
        qDebug()<<buff;
        QString A = QString(buff);
        QStringList C;
        QStringList a = A.split("\n");
        for (int i = 0;i<a.size() ;i++) {
            QStringList b = a[i].split("\t");
            C<<b.at(b.size()-1);
        }
        for (int j =1;j<C.size();j++)
        {
            m_adTtemtowifi(C[j],"1");
        }
    }
    else
    {
        qDebug()<<"scan bad";
    }
#endif
}

void MainWindow::on_pushButton_340_clicked()
{
#ifdef XH_LINUX
    /*wifi初始化进入*/
    if(aw_wifi == NULL)
    {
        ui->listWidget_3->clear();
        ui->stackedWidget->setCurrentIndex(81);
        aw_wifi = aw_wifi_on(tWifi_event_callback,0);
        if(aw_wifi == NULL)
        {
            qDebug()<<"wifi open failed";
            return;
        }
        char buff[2048]="";
        int i = sizeof(buff);
        if(aw_wifi->get_scan_results(buff,&i) == 0)
        {
            qDebug()<<buff;
            QString A = QString(buff);
            QStringList C;
            QStringList a = A.split("\n");
            for (int i = 0;i<a.size() ;i++) {
                QStringList b = a[i].split("\t");
                C<<b.at(b.size()-1);
            }
            for (int j =1;j<C.size();j++)
            {
                m_adTtemtowifi(C[j],"1");
            }
        }
        else
        {
            qDebug()<<"scan bad";
        }
    }
    else
    {
//        delete  aw_wifi;
//        aw_wifi = NULL;
        ui->stackedWidget->setCurrentIndex(81);
//        ui->listWidget_3->clear();
//        aw_wifi = aw_wifi_on(tWifi_event_callback,0);
//        if(aw_wifi == NULL)
//        {
//            qDebug()<<"wifi open failed";
//            return;
//        }
//        char buff[2048]="";
//        int i = sizeof(buff);
//        if(aw_wifi->get_scan_results(buff,&i) == 0)
//        {
//            qDebug()<<buff;
//            QString A = QString(buff);
//            QStringList C;
//            QStringList a = A.split("\n");
//            for (int i = 0;i<a.size() ;i++) {
//                QStringList b = a[i].split("\t");
//                C<<b.at(b.size()-1);
//            }
//            for (int j =1;j<C.size();j++)
//            {
//                m_adTtemtowifi(C[j],"1");
//            }
//        }
//        else
//        {
//            qDebug()<<"scan bad";
//        }
    }
#endif
}

void MainWindow::on_pushButton_343_clicked()
{
    ui->lineEdit->insert(" ");
}

void MainWindow::on_pushButton_679_clicked()
{
    ui->label_164->clear();
    ui->label_192->clear();
    ui->label_193->clear();
    ui->label_224->clear();
    ui->label_225->clear();
    ui->label_285->clear();
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_676_clicked()
{
    ui->label_164->clear();
    ui->label_192->clear();
    ui->label_193->clear();
    ui->label_224->clear();
    ui->label_225->clear();
    ui->label_285->clear();
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_678_clicked()
{
    ui->label_164->clear();
    ui->label_192->clear();
    ui->label_193->clear();
    ui->label_224->clear();
    ui->label_225->clear();
    ui->label_285->clear();
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_677_clicked()
{
    ui->label_164->clear();
    ui->label_192->clear();
    ui->label_193->clear();
    ui->label_224->clear();
    ui->label_225->clear();
    ui->label_285->clear();
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_89_clicked()
{
    //
}

void MainWindow::on_pushButton_128_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_125_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_126_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_130_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}

void MainWindow::on_pushButton_133_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_132_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_131_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_141_clicked()
{
    /*print 第一个设置*/
    if(m_timer.isActive())
    {
        m_timer.stop();
    }
    //QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEightteen()));
    //ui->stackedWidget->setCurrentIndex(38);
}

void MainWindow::on_pushButton_168_clicked()
{
    m_parsetdlog =new parsetdlog(this);
    m_parsetdlog->show();
    QObject::connect(m_parsetdlog,&parsetdlog::next,this,&MainWindow::findhotend);
    QObject::connect(m_parsetdlog,&parsetdlog::complete,this,&MainWindow::parsetclose);
    QObject::connect(m_parsetdlog,&parsetdlog::excomplete,this,&MainWindow::parsetexclose);
    QObject::connect(m_parsetdlog,&parsetdlog::active,this,&MainWindow::paracktiv);
    QObject::connect(m_parsetdlog,&parsetdlog::up,this,&MainWindow::up);
    QObject::connect(m_parsetdlog,&parsetdlog::down,this,&MainWindow::down);
    m_port->askstate();
    m_printsec->stop();
//    m_port->backupsend();


}

void MainWindow::on_pushButton_118_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_113_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

void MainWindow::on_pushButton_149_clicked()
{
    ui->lineEdit->insert(ui->pushButton_149->text());
}

void MainWindow::on_pushButton_151_clicked()
{
    ui->lineEdit->insert(ui->pushButton_151->text());
}

void MainWindow::on_pushButton_148_clicked()
{
    ui->lineEdit->insert(ui->pushButton_148->text());
}

void MainWindow::on_pushButton_160_clicked()
{

    /*设置返回函数*/
//    if(isPrintFirstAnim)
//    {
//        if(isStartFristAnim)
//        {
//            m_timer.stop();
//            isStartFristAnim = false;
//            QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpthirteen()));
//            QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpSeventeen()));
//            ui->stackedWidget->setCurrentIndex(35);
//            m_timer.start(15);
//        }
//        else
//        {
//            ui->stackedWidget->setCurrentIndex(35);
//            m_timer.start(15);
//        }
//        ui->stackedWidget->setCurrentIndex(35);
//        m_timer.start(15);
//    }
//    else
//    {
//        if(isStartFristAnim)
//        {
//            m_timer.stop();
//            isStartFristAnim = false;
//            QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpFourteen()));
//            QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(jumpEightteen()));
//            ui->stackedWidget->setCurrentIndex(36);
//            m_timer.start(15);
//        }
//        else
//        {
//            ui->stackedWidget->setCurrentIndex(36);
//            m_timer.start(15);
//        }
//        ui->stackedWidget->setCurrentIndex(36);
//        m_timer.start(15);
//    }
}

void MainWindow::on_pushButton_167_clicked()
{
    m_port->regainPrint();

}

void MainWindow::on_pushButton_44_clicked()
{
    ui->widget_2->setVisible(false);
    ui->m_StatusBar->setVisible(true);
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(shinetwo()));
    m_timer.stop();
}

void MainWindow::on_pushButton_42_clicked()
{
    m_timer.stop();
    ui->m_StatusBar->setVisible(true);
    ui->widget_2->setVisible(false);
    //QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(shinetwo()));
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(jumptwelve()));

}

void MainWindow::on_pushButton_346_clicked()
{
    ui->stackedWidget->setCurrentIndex(71);
}

void MainWindow::on_pushButton_354_clicked()
{
    m_port->portInit(ui->comboBox_15->currentText());

        m_port->serialOpen =true;
        ui->stackedWidget->setCurrentIndex(33);
        m_port->powerlostsend();
        serialOpen =true;
        ui->m_StatusBar->setVisible(true);
        QObject::connect(printTimer,&QTimer::timeout,this,&MainWindow::askPrint);
        printTimer->start(1000);

}



void MainWindow::on_pushButton_355_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
    m_port->setFilament("0","0");
}

void MainWindow::on_pushButton_253_clicked()
{
    m_port->cancle();
}

void MainWindow::on_pushButton_365_clicked()
{
    m_port->setFilament("0","0");
    ui->stackedWidget->setCurrentIndex(51);
    m_port->carbinfinished();
}

void MainWindow::on_pushButton_367_clicked()
{
    ui->stackedWidget->setCurrentIndex(57);
}

void MainWindow::on_pushButton_375_clicked()
{
    ui->stackedWidget->setCurrentIndex(58);
}

void MainWindow::on_pushButton_308_clicked()
{
    ui->stackedWidget->setCurrentIndex(51);
    m_port->setFilament("0","0");

}

void MainWindow::on_pushButton_109_clicked()
{
    mchoose = new chooseTemp(this);
    mchoose->init(ui->pushButton_109->text());
    ui->pushButton_358->setText("Cancel");
#ifdef OLD
    ui->pushButton_111->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/UnloadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
    ui->pushButton_112->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/LoadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
    ui->pushButton_114->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/UnloadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
    ui->pushButton_115->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/LoadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
#endif
    ui->pushButton_111->setEnabled(false);
    ui->pushButton_112->setEnabled(false);
    ui->pushButton_114->setEnabled(false);
    ui->pushButton_115->setEnabled(false);

    ui->pushButton_105->setEnabled(false);
    ui->pushButton_106->setEnabled(false);
    ui->pushButton_107->setEnabled(false);
    ui->pushButton_108->setEnabled(false);
    QObject::connect(mchoose,&chooseTemp::heatT,this,&MainWindow::ltemp);
    mchoose->show();
}

void MainWindow::on_pushButton_110_clicked()
{
        mchoose = new chooseTemp(this);
        mchoose->init(ui->pushButton_110->text());
        ui->pushButton_358->setText("Cancel");
#ifdef OLD
        ui->pushButton_111->setStyleSheet("QPushButton{outline:none;\
                                            image: url(:/UnloadButton.png);\
                                          background-color: rgb(45, 44, 43);\
                                          border:none;};");
        ui->pushButton_112->setStyleSheet("QPushButton{outline:none;\
                                            image: url(:/LoadButton.png);\
                                          background-color: rgb(45, 44, 43);\
                                          border:none;};");
        ui->pushButton_114->setStyleSheet("QPushButton{outline:none;\
                                            image: url(:/UnloadButton.png);\
                                          background-color: rgb(45, 44, 43);\
                                          border:none;};");
        ui->pushButton_115->setStyleSheet("QPushButton{outline:none;\
                                            image: url(:/LoadButton.png);\
                                          background-color: rgb(45, 44, 43);\
                                          border:none;};");
#endif
        ui->pushButton_111->setEnabled(false);
        ui->pushButton_112->setEnabled(false);
        ui->pushButton_114->setEnabled(false);
        ui->pushButton_115->setEnabled(false);

        ui->pushButton_105->setEnabled(false);
        ui->pushButton_107->setEnabled(false);
        ui->pushButton_108->setEnabled(false);
        mchoose->show();
        QObject::connect(mchoose,&chooseTemp::heatT,this,&MainWindow::rtemp);
}

void MainWindow::on_pushButton_358_clicked()
{
    ui->pushButton_109->setText("000°C");
    ui->pushButton_110->setText("000°C");
    m_port->setFilament("0","0");
#ifdef OLD
    ui->pushButton_111->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/UnloadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
    ui->pushButton_112->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/LoadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
    ui->pushButton_114->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/UnloadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
    ui->pushButton_115->setStyleSheet("QPushButton{outline:none;\
                                        image: url(:/LoadButton.png);\
                                      background-color: rgb(45, 44, 43);\
                                      border:none;};");
#endif
    ui->pushButton_111->setEnabled(false);
    ui->pushButton_112->setEnabled(false);
    ui->pushButton_114->setEnabled(false);
    ui->pushButton_115->setEnabled(false);

    ui->pushButton_105->setEnabled(true);
    ui->pushButton_107->setEnabled(true);
    ui->pushButton_108->setEnabled(true);
}

void MainWindow::on_pushButton_111_clicked()
{
    m_port->lup();
}

void MainWindow::on_pushButton_112_clicked()
{
    m_port->ldown();
}

void MainWindow::on_pushButton_114_clicked()
{
    m_port->rup();
}

void MainWindow::on_pushButton_115_clicked()
{
    m_port->rdown();
}

void MainWindow::on_pushButton_105_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_107_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_pushButton_108_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(false);
#endif
}

//void MainWindow::on_pushButton_360_clicked()
//{

//}

void MainWindow::on_pushButton_140_clicked()
{

}

void MainWindow::on_pushButton_98_clicked()
{
//    m_port->testb();
}

void MainWindow::on_pushButton_99_clicked()
{
//    m_port->testr();
}

void MainWindow::on_pushButton_356_clicked()
{
    m_filamentfault = new  filamentFault(this);
    m_filamentfault->show();
    QObject::connect(m_filamentfault,&filamentFault::abort,this,&MainWindow::Fabort);
    QObject::connect(m_filamentfault,&filamentFault::resume,this,&MainWindow::Fresume);
    QObject::connect(m_filamentfault,&filamentFault::changeFilament,this,&MainWindow::Fchangefilament);
    ui->stackedWidget->setCurrentIndex(36);
}

void MainWindow::on_pushButton_263_clicked()
{
    m_printfilament = new printFlament(this);
    m_printfilament->init(ui->pushButton_263->text());
    m_printfilament->show();
    QObject::connect(m_printfilament,&printFlament::heatT,this,&MainWindow::plt);
}

void MainWindow::on_pushButton_262_clicked()
{
    m_printfilament = new printFlament(this);
    m_printfilament->init(ui->pushButton_262->text());
    m_printfilament->show();
    QObject::connect(m_printfilament,&printFlament::heatT,this,&MainWindow::prt);
}

void MainWindow::on_pushButton_124_clicked()
{

}





void MainWindow::on_pushButton_100_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void MainWindow::on_pushButton_101_clicked()
{
    ui->pushButton_134->setStyleSheet("QPushButton{border-top-left-radius: 20px;\
                                      border-bottom-left-radius: 20px;\
                                      border-left: 2px solid rgb(81, 83, 86);\
                                      border-top: 2px solid rgb(81, 83, 86);\
                                      border-bottom: 2px solid rgb(81, 83, 86);\
                                      background-color: rgb(32, 32, 32);\
                                      font-weight: bold;\
                                      font-family:Barlow;outline:none;\
                                      color: rgb(255, 255, 255);\
                                      font-size: 32px;\
};");
    ui->pushButton_101->setStyleSheet("QPushButton{\
                                      background-color: rgba(45, 44, 43, 255);\
                                      font-weight: bold;\
                                      font-family:Barlow;outline:none;\
                                      color: rgb(255, 255, 255);\
                                      font-size: 32px;\
                                      border-top-right-radius: 20px;\
                                      border-bottom-right-radius: 20px;\
                                      border-right: 2px solid rgb(81, 83, 86);\
                                      border-top: 2px solid rgb(81, 83, 86);\
                                      border-bottom: 2px solid rgb(81, 83, 86);\
};");

        ui->listWidget->setVisible(false);
        ui->listWidget_2->setVisible(true);

        ui->listWidget_2->clear();
        QDir *m_dir=new QDir(UDiskPath);
        QStringList filter;
        QFileInfoList m_fileinfo = m_dir->entryInfoList();
        int i = 0;
        for(i = 0;i< m_fileinfo.count();i++)
        {
            m_addItemToList(m_fileinfo.at(i).fileName(),m_fileinfo.at(i).filePath(),"udisk");
                    qDebug()<<m_fileinfo.at(i).filePath();
                    qDebug()<<m_fileinfo.at(i).fileName();
        }

}



void MainWindow::on_pushButton_371_clicked()
{
    m_port->setFilament("0","0");
    ui->stackedWidget->setCurrentIndex(51);
    m_port->carbincancel();
}

void MainWindow::on_pushButton_373_clicked()
{
    m_port->setFilament("0","0");
    ui->stackedWidget->setCurrentIndex(51);
    m_port->carbincancel();
}

void MainWindow::on_pushButton_164_clicked()
{
    /*切换小写*/
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_pushButton_393_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_313_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainWindow::on_pushButton_387_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainWindow::on_pushButton_418_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_138_clicked()
{
    ui->lineEdit->insert(ui->pushButton_138->text());
}

void MainWindow::on_pushButton_139_clicked()
{
    ui->lineEdit->insert(ui->pushButton_139->text());
}

void MainWindow::on_pushButton_142_clicked()
{
    ui->lineEdit->insert(ui->pushButton_142->text());
}

void MainWindow::on_pushButton_143_clicked()
{
    ui->lineEdit->insert(ui->pushButton_143->text());
}

void MainWindow::on_pushButton_144_clicked()
{
    ui->lineEdit->insert(ui->pushButton_144->text());
}

void MainWindow::on_pushButton_145_clicked()
{
    ui->lineEdit->insert(ui->pushButton_145->text());
}

void MainWindow::on_pushButton_150_clicked()
{
    ui->lineEdit->insert(ui->pushButton_150->text());
}

void MainWindow::on_pushButton_152_clicked()
{
    ui->lineEdit->insert(ui->pushButton_152->text());
}

void MainWindow::on_pushButton_153_clicked()
{
    ui->lineEdit->insert(ui->pushButton_153->text());
}

void MainWindow::on_pushButton_154_clicked()
{
    ui->lineEdit->insert(ui->pushButton_154->text());
}

void MainWindow::on_pushButton_155_clicked()
{
    ui->lineEdit->insert(ui->pushButton_155->text());
}

void MainWindow::on_pushButton_156_clicked()
{
    ui->lineEdit->insert(ui->pushButton_156->text());
}

void MainWindow::on_pushButton_157_clicked()
{
    ui->lineEdit->insert(ui->pushButton_157->text());
}

void MainWindow::on_pushButton_158_clicked()
{
    ui->lineEdit->insert(ui->pushButton_158->text());
}

void MainWindow::on_pushButton_159_clicked()
{
    ui->lineEdit->insert(ui->pushButton_159->text());
}

void MainWindow::on_pushButton_163_clicked()
{
    ui->lineEdit->insert(ui->pushButton_163->text());
}

void MainWindow::on_pushButton_165_clicked()
{
    ui->lineEdit->insert(ui->pushButton_165->text());
}

void MainWindow::on_pushButton_166_clicked()
{
    ui->lineEdit->insert(ui->pushButton_166->text());
}

void MainWindow::on_pushButton_258_clicked()
{
    ui->lineEdit->insert(ui->pushButton_258->text());
}

void MainWindow::on_pushButton_265_clicked()
{
    ui->lineEdit->insert(ui->pushButton_265->text());
}

void MainWindow::on_pushButton_267_clicked()
{
    ui->lineEdit->insert(ui->pushButton_267->text());
}

void MainWindow::on_pushButton_297_clicked()
{
    ui->lineEdit->insert(ui->pushButton_297->text());
}

void MainWindow::on_pushButton_311_clicked()
{
    ui->lineEdit->insert(ui->pushButton_311->text());
}

void MainWindow::on_pushButton_385_clicked()
{
    ui->lineEdit->insert(ui->pushButton_385->text());
}

void MainWindow::on_pushButton_382_clicked()
{
    ui->lineEdit->insert(ui->pushButton_382->text());
}

void MainWindow::on_pushButton_398_clicked()
{
    ui->lineEdit->insert(ui->pushButton_398->text());
}

void MainWindow::on_pushButton_381_clicked()
{
    ui->lineEdit->insert(ui->pushButton_381->text());
}

void MainWindow::on_pushButton_397_clicked()
{
    ui->lineEdit->insert(ui->pushButton_397->text());
}

void MainWindow::on_pushButton_389_clicked()
{
    ui->lineEdit->insert(ui->pushButton_389->text());
}

void MainWindow::on_pushButton_392_clicked()
{
    ui->lineEdit->insert(ui->pushButton_392->text());
}

void MainWindow::on_pushButton_407_clicked()
{
    ui->lineEdit->insert(ui->pushButton_407->text());
}

void MainWindow::on_pushButton_399_clicked()
{
    ui->lineEdit->insert(ui->pushButton_399->text());
}

void MainWindow::on_pushButton_384_clicked()
{
    ui->lineEdit->insert(ui->pushButton_384->text());
}

void MainWindow::on_pushButton_383_clicked()
{
    ui->lineEdit->insert(ui->pushButton_383->text());
}

void MainWindow::on_pushButton_404_clicked()
{
    ui->lineEdit->insert(ui->pushButton_404->text());
}

void MainWindow::on_pushButton_388_clicked()
{
    ui->lineEdit->insert(ui->pushButton_388->text());
}

void MainWindow::on_pushButton_390_clicked()
{
    ui->lineEdit->insert(ui->pushButton_390->text());
}

void MainWindow::on_pushButton_395_clicked()
{
    ui->lineEdit->insert(ui->pushButton_395->text());
}

void MainWindow::on_pushButton_406_clicked()
{
    ui->lineEdit->insert(ui->pushButton_406->text());
}

void MainWindow::on_pushButton_403_clicked()
{
    ui->lineEdit->insert(ui->pushButton_403->text());
}

void MainWindow::on_pushButton_386_clicked()
{
    ui->lineEdit->insert(ui->pushButton_386->text());
}

void MainWindow::on_pushButton_394_clicked()
{
    ui->lineEdit->insert(ui->pushButton_394->text());
}

void MainWindow::on_pushButton_405_clicked()
{
    ui->lineEdit->insert(ui->pushButton_405->text());
}

void MainWindow::on_pushButton_360_clicked()
{
    ui->lineEdit->insert(ui->pushButton_360->text());
}

void MainWindow::on_pushButton_400_clicked()
{
    ui->lineEdit->insert(ui->pushButton_400->text());
}

void MainWindow::on_pushButton_396_clicked()
{
    ui->lineEdit->insert(ui->pushButton_396->text());
}

void MainWindow::on_pushButton_379_clicked()
{
    ui->lineEdit->insert(ui->pushButton_379->text());
}

void MainWindow::on_pushButton_345_clicked()
{
    ui->lineEdit->insert(ui->pushButton_345->text());
}

void MainWindow::on_pushButton_380_clicked()
{
    ui->lineEdit->insert(ui->pushButton_380->text());
}

void MainWindow::on_pushButton_416_clicked()
{
    ui->lineEdit->insert(ui->pushButton_416->text());
}

void MainWindow::on_pushButton_413_clicked()
{
    ui->lineEdit->insert(ui->pushButton_413->text());
}

void MainWindow::on_pushButton_429_clicked()
{
    ui->lineEdit->insert(ui->pushButton_429->text());
}

void MainWindow::on_pushButton_412_clicked()
{
    ui->lineEdit->insert(ui->pushButton_412->text());
}

void MainWindow::on_pushButton_428_clicked()
{
    ui->lineEdit->insert(ui->pushButton_428->text());
}

void MainWindow::on_pushButton_420_clicked()
{
    ui->lineEdit->insert(ui->pushButton_420->text());
}

void MainWindow::on_pushButton_423_clicked()
{
    ui->lineEdit->insert(ui->pushButton_423->text());
}

void MainWindow::on_pushButton_438_clicked()
{
    ui->lineEdit->insert(ui->pushButton_438->text());
}

void MainWindow::on_pushButton_430_clicked()
{
    ui->lineEdit->insert(ui->pushButton_430->text());
}

void MainWindow::on_pushButton_415_clicked()
{
    ui->lineEdit->insert(ui->pushButton_415->text());
}

void MainWindow::on_pushButton_414_clicked()
{
    ui->lineEdit->insert(ui->pushButton_414->text());
}

void MainWindow::on_pushButton_435_clicked()
{
    ui->lineEdit->insert(ui->pushButton_435->text());
}

void MainWindow::on_pushButton_419_clicked()
{
    ui->lineEdit->insert(ui->pushButton_419->text());
}

void MainWindow::on_pushButton_421_clicked()
{
    ui->lineEdit->insert(ui->pushButton_421->text());
}

void MainWindow::on_pushButton_426_clicked()
{
    ui->lineEdit->insert(ui->pushButton_426->text());
}

void MainWindow::on_pushButton_437_clicked()
{
    ui->lineEdit->insert(ui->pushButton_437->text());
}

void MainWindow::on_pushButton_434_clicked()
{
    ui->lineEdit->insert(ui->pushButton_434->text());
}

void MainWindow::on_pushButton_417_clicked()
{
    ui->lineEdit->insert(ui->pushButton_417->text());
}

void MainWindow::on_pushButton_425_clicked()
{
    ui->lineEdit->insert(ui->pushButton_425->text());
}

void MainWindow::on_pushButton_424_clicked()
{
    ui->lineEdit->insert(ui->pushButton_424->text());
}

void MainWindow::on_pushButton_436_clicked()
{
    ui->lineEdit->insert(ui->pushButton_436->text());
}

void MainWindow::on_pushButton_409_clicked()
{
    ui->lineEdit->insert(ui->pushButton_409->text());
}

void MainWindow::on_pushButton_431_clicked()
{
    ui->lineEdit->insert(ui->pushButton_431->text());
}

void MainWindow::on_pushButton_427_clicked()
{
    ui->lineEdit->insert(ui->pushButton_427->text());
}

void MainWindow::on_pushButton_410_clicked()
{
    ui->lineEdit->insert(ui->pushButton_410->text());
}

void MainWindow::on_pushButton_408_clicked()
{
    ui->lineEdit->insert(ui->pushButton_408->text());
}

void MainWindow::on_pushButton_411_clicked()
{
    ui->lineEdit->insert(ui->pushButton_411->text());
}

void MainWindow::on_pushButton_689_clicked()
{
#ifdef XH_LINUX
    std::string str = chooseit->wifiname.toStdString();
    const char * a= str.c_str();

    std::string str1 = ui->lineEdit->text().toStdString();
    const char * b= str1.c_str();

    qDebug()<<"a"<<a;
    qDebug()<<"b"<<b;
    if(aw_wifi->connect_ap(a,b,1)>=0)
    {
        qDebug()<<"connect success!";
        start_udhcpc();
        ui->stackedWidget->setCurrentIndex(84);
        setWinPic(true);
        udpControl = new XhControlR818(this);
        QObject::connect(udpControl,&XhControlR818::signalsAskCondition,this,&MainWindow::getCondition);
        QObject::connect(this,&MainWindow::sendCondition,udpControl,&XhControlR818::slotAskCondition);
        QObject::connect(udpControl,&XhControlR818::signalsDownloadOver,this,&MainWindow::downloadOver);
    }
    else
    {
        qDebug()<<"connect bad";
        ui->stackedWidget->setCurrentIndex(83);
        setWinPic(false);
    }
#endif
}

void MainWindow::on_pushButton_691_clicked()
{
    ui->stackedWidget->setCurrentIndex(81);
    ui->label_316->setText(chooseit->wifiname);
    ui->label_316->setStyleSheet("QLabel{font-weight: bold;\
            font-family:Barlow;border-radius: 20px;\
            background-color: rgb(255, 99, 11);\
            color: rgba(255, 255, 255, 255);\
            font-size: 36px;font-weight:bold;\
            border: 2px solid rgba(255, 99, 11, 255);\
};");
}

void MainWindow::on_pushButton_690_clicked()
{
    ui->stackedWidget->setCurrentIndex(81);
    ui->label_316->setText("none");
    ui->label_316->setStyleSheet("QLabel{background-color: rgba(255, 255, 255, 0);\
                                 color: rgb(255, 255, 255);\
                                 font-family:Barlow;\
                                 font-weight:bold;\
                                 font-size:36px;\
};");
}

void MainWindow::on_pushButton_642_clicked()
{
    ui->stackedWidget->setCurrentIndex(74);
}

void MainWindow::on_pushButton_312_clicked()
{
    ui->lineEdit->backspace();
}

void MainWindow::on_pushButton_401_clicked()
{
    ui->lineEdit->backspace();
}

void MainWindow::on_pushButton_432_clicked()
{
    ui->lineEdit->backspace();
}

void MainWindow::on_pushButton_433_clicked()
{
    ui->lineEdit->insert(" ");
}

void MainWindow::on_pushButton_402_clicked()
{
    ui->lineEdit->insert(" ");
}

void MainWindow::on_pushButton_688_clicked()
{
    ui->stackedWidget->setCurrentIndex(74);
}

void MainWindow::on_pushButton_447_clicked()
{
    m_port->updateBegin(UpdateFile);
    printTimer->stop();
    /*不知道是否弹窗*/
}

void MainWindow::on_pushButton_692_clicked()
{
    ui->label_164->clear();
    ui->label_192->clear();
    ui->label_193->clear();
    ui->label_224->clear();
    ui->label_225->clear();
    ui->label_285->clear();
    ui->stackedWidget->setCurrentIndex(76);
}

void MainWindow::on_pushButton_453_clicked(bool checked)
{
    if(!checked)
    {
        if(!ui->pushButton_454->isChecked())
        {
            ui->pushButton_454->setChecked(true);
        }
    }
}

void MainWindow::on_pushButton_454_clicked(bool checked)
{
    if(!checked)
    {
        if(!ui->pushButton_453->isChecked())
        {
            ui->pushButton_453->setChecked(true);
        }
    }
}

void MainWindow::on_pushButton_455_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
#ifdef XH_VIS
    ui->m_StatusBar->setVisible(true);
#endif
}
