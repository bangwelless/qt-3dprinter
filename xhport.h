#ifndef XHPORT_H
#define XHPORT_H
/*3D打印机屏幕实例化这个类，并且绑定这个类的信号和屏幕的槽，
 * 这个类构造会打开串口，然后调用接口函数向主控发送串口指令，
 * 这个类会解析回复的信息，然后发送对应信号，操控屏幕*/
#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "xhpage.h"

#define PORTNAME "COM9"

class XhPort : public QObject
{
    Q_OBJECT
public:
    explicit XhPort(QObject *parent = nullptr);
    ~XhPort();

    /*屏幕主动请求接口函数(*表示指令发出后需要等待主控回复)*/
    /**********Frist Start**********/
    void askTemperature();//开机请求各项温度*
    void askStatus();//开机请求设备状态*
    void isHeating();//正在加热？*
    void calibrationPlatform();//校准平台*
    void isXYHeating();//是否在加热？*
    void XYAdjust();//校准XY轴

    void powerlostsend();
    void powercancel();

    /**********FILAMENT**********/
    void setFilament(QString ,QString );
    void lup();
    void ldown();
    void rup();
    void rdown();

    /**********PRINT IDER**********/
    QByteArray startPrint(QString );
    void AcsPrint(QByteArray);
    void pausePrint();
    void stopPrint();
    void regainPrint();
    void fileTemperature(QString , QString ,QString );
    void readyprint(int ,QByteArray );

    void enHotend(bool ,bool );
    void enbackup(bool );
    void printcom(int );
    void parcom(int );
    void askHotend();
    void askstate();
    /*********ERROR*************/
    void resume();
    void abort();
    void testr();
    void testb();

    /**********TOOL**********/

    void setColorRGB();//设置颜色的RGB值
    void selfTest();//设备自检*
    void unFilament();//禁止filament
    void factoryReset();//恢复出厂设置
    /*platfrom*/
    void p_nozzleHeating();
    void p_platformCalibration();

    void finish();
    void cancle();
    /*nozzle*/
    void n_nozzleHeating();
    void n_nozzleCalibration();
    /*xy*/
    void x_xyHeating();
    void x_platformCalibration();
    void x_xyCalibration();
    /*l hotend*/
    void l_hotendTest();
    void l_cooling();
    /*r hotend*/
    void r_hotendTest();
    //暂时不做
    void relativeMove();//相对模式移动
    void absoluteMove();//绝对模式移动
    void homeMove();//移动到起点

    void testdemo();

    void portInit(QString );//初始化串口
    bool serialOpen;//串口是否打开

    void selftest1();
    void selftest2();
    void selftest3();
    void selftest4();
    void selftest5();
//    void selftest6();
    void backupsend();
    void backuptsend();

    void carbinfinished();
    void carbincancel();

    void updateBegin(QString );

private:
    QSerialPort *m_serial;
    XhPage *m_package;


    QTimer *portTimer;//专门用于测试通讯串口的计时器

    void readData();//读取并解析串口响应



signals:

    /*first start*/
    void serialIsOpen();
    void firstTestResult(bool ,bool ,bool ,bool ,bool );
    void firstTemperatureResult(int ,int ,int ,int ,int ,int ,int ,QByteArray );
    void noHeating(bool );
    void platCheck(bool );
    void xNoHeating(bool );
    void xyCheck(bool );
    /*tool*/
    void toolTestResult(bool ,bool ,bool ,bool ,bool ,bool );
    void disUseFilament(bool );
    void backFactory(bool );
    /*filament*/
    void filamentHeated(bool );

    /*tool calibration*/
    void pNozzleHeating(bool );
    void pPlatformCalibration(qint32 ,qint32 ,qint32 ,qint32 );
    void nNozzleHeating(bool );
    void nNozzleCalibration( int );
    void xXyHeating(bool );
    void xPlatformCalibration(bool );
    void xXyCalibration( );
    void finished();
    void canelk();



    void canPrint();
    void fileSendOver();
    void type(int ,int);
    void stopOk();
    void pauseOk();
    void goOnOk();
    void printend();

    void powerlost();
    void filamentlost();
    void error(int );


    void selfTest1();
    void selfTest2();
    void selfTest3();
    void selfTest4();
    void selfTest5();
    void selfTest6();
    void state(QString ,QString ,QString ,QString ,QString ,QString ,QString );
    void xcanone(int );
    void xcantwo(int );
    void updateBeginsignl();
    void xhupdateNum(int );
public slots:

private slots:
    void serialTest();//frsit start 开机测试串口是否打通

    void sendfile(QByteArray );

    void xhfirstTestResult(bool ,bool ,bool ,bool ,bool );
    void xhfirstTemperatureResult(int ,int ,int ,int ,int ,int ,int ,QByteArray );
    /*tool*/
    void xhtoolTestResult(bool ,bool ,bool ,bool ,bool ,bool );
    void xhdisUseFilament(bool );
    void xhbackFactory(bool );
    /*filament*/
    void xhfilamentHeated(bool );

    /*tool calibration*/
    void xhpNozzleHeating(bool );
    void xhpPlatformCalibration(qint32 ,qint32 ,qint32 ,qint32 );
    void xhnNozzleHeating(bool );
    void xhnNozzleCalibration( int );
    void xhxXyHeating(bool );
    void xhxPlatformCalibration(bool );
    void xhxXyCalibration( );

    void printCan();
    void fileSendOverSlot();
    void typeChang(int ,int);
    void xhnoHeating(bool );
    void xhplatCheck(bool );
    void xhxNoHeating(bool );
    void xhxyCheck(bool );
    void xhstopOk();
    void xhpauseOk();
    void xhgoOnOk();

    void xhfinished();
    void xhcancle();
    void xhprintend();

    void xhpowerlost();
    void xhfilamentlost();
    void xherror(int );

    void xhselfTest1();
    void xhselfTest2();
    void xhselfTest3();
    void xhselfTest4();
    void xhselfTest5();
    void xhselfTest6();

    void xhstate(QString ,QString ,QString ,QString ,QString ,QString ,QString );
    void xhcanone(int a);
    void xhcantwo(int a);

    void updateBeginSlot();
    void updateCheck(QByteArray );
    void updateSend(QByteArray ,int ,QByteArray );
    void updateOver();
    void updateNum(int );
};

#endif // XHPORT_H
