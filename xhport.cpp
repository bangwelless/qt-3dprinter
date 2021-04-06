#include "xhport.h"

XhPort::XhPort(QObject *parent) : QObject(parent)
{
    m_serial = new QSerialPort(this);
    portTimer = new QTimer(this);
    m_package = new XhPage;
    serialOpen =false;
    /*打开串口*/
    QObject::connect(m_package,&XhPage::sendFileArry,this,&XhPort::sendfile);

    QObject::connect(m_package,&XhPage::firstTestResult,this,&XhPort::xhfirstTestResult);
    QObject::connect(m_package,&XhPage::firstTemperatureResult,this,&XhPort::xhfirstTemperatureResult);
    /*tool*/
    QObject::connect(m_package,&XhPage::toolTestResult,this,&XhPort::xhtoolTestResult);
    QObject::connect(m_package,&XhPage::disUseFilament,this,&XhPort::xhdisUseFilament);
    QObject::connect(m_package,&XhPage::backFactory,this,&XhPort::xhbackFactory);
    /*filament*/
    QObject::connect(m_package,&XhPage::filamentHeated,this,&XhPort::xhfilamentHeated);

    /*tool calibration*/
    QObject::connect(m_package,&XhPage::pNozzleHeating,this,&XhPort::xhpNozzleHeating);
    QObject::connect(m_package,&XhPage::pPlatformCalibration,this,&XhPort::xhpPlatformCalibration);
    QObject::connect(m_package,&XhPage::nNozzleHeating,this,&XhPort::xhnNozzleHeating);
    QObject::connect(m_package,&XhPage::nNozzleCalibration,this,&XhPort::xhnNozzleCalibration);
    QObject::connect(m_package,&XhPage::xXyHeating,this,&XhPort::xhxXyHeating);
    QObject::connect(m_package,&XhPage::xPlatformCalibration,this,&XhPort::xhxPlatformCalibration);
    QObject::connect(m_package,&XhPage::xXyCalibration,this,&XhPort::xhxXyCalibration);

    QObject::connect(m_package,&XhPage::cancle,this,&XhPort::xhcancle);
    QObject::connect(m_package,&XhPage::finished,this,&XhPort::xhcancle);


    QObject::connect(m_package,&XhPage::canPrint,this,&XhPort::printCan);
    QObject::connect(m_package,&XhPage::fileSendOver,this,&XhPort::fileSendOverSlot);
    QObject::connect(m_package,&XhPage::typeChanged,this,&XhPort::typeChang);

    QObject::connect(m_package,&XhPage::noHeating,this,&XhPort::xhnoHeating);
    QObject::connect(m_package,&XhPage::platCheck,this,&XhPort::xhplatCheck);
    QObject::connect(m_package,&XhPage::xNoHeating,this,&XhPort::xhxNoHeating);
    QObject::connect(m_package,&XhPage::xyCheck,this,&XhPort::xhxyCheck);

    QObject::connect(m_package,&XhPage::stopOk,this,&XhPort::xhstopOk);
    QObject::connect(m_package,&XhPage::pauseOk,this,&XhPort::xhpauseOk);
    QObject::connect(m_package,&XhPage::goOnOk,this,&XhPort::xhgoOnOk);
    QObject::connect(m_package,&XhPage::printend,this,&XhPort::xhprintend);

    QObject::connect(m_package,&XhPage::filamentlost,this,&XhPort::xhfilamentlost);
    QObject::connect(m_package,&XhPage::powerlost,this,&XhPort::xhpowerlost);
    QObject::connect(m_package,&XhPage::error,this,&XhPort::xherror);
    QObject::connect(m_package,&XhPage::selfTest1,this,&XhPort::xhselfTest1);
    QObject::connect(m_package,&XhPage::selfTest2,this,&XhPort::xhselfTest2);
    QObject::connect(m_package,&XhPage::selfTest3,this,&XhPort::xhselfTest3);
    QObject::connect(m_package,&XhPage::selfTest4,this,&XhPort::xhselfTest4);
    QObject::connect(m_package,&XhPage::selfTest5,this,&XhPort::xhselfTest5);
    QObject::connect(m_package,&XhPage::selfTest6,this,&XhPort::xhselfTest6);
    QObject::connect(m_package,&XhPage::state,this,&XhPort::xhstate);
    QObject::connect(m_package,&XhPage::canone,this,&XhPort::xhcanone);
    QObject::connect(m_package,&XhPage::cantwo,this,&XhPort::xhcantwo);

    QObject::connect(m_package,&XhPage::updateBeginSignls,this,&XhPort::updateBeginSlot);
    QObject::connect(m_package,&XhPage::updateCheck,this,&XhPort::updateCheck);
    QObject::connect(m_package,&XhPage::updateSend,this,&XhPort::updateSend);
    QObject::connect(m_package,&XhPage::updateOver,this,&XhPort::updateOver);
    QObject::connect(m_package,&XhPage::updateSerial,this,&XhPort::updateNum);


}

XhPort::~XhPort()
{
    m_serial->close();
    delete m_serial;
    delete portTimer;
    delete m_package;
}

/*请求温度*/
void XhPort::askTemperature()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002E001000100");
    m_serial->write(buff);
}
/*请求状态*/
void XhPort::askStatus()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002E003000102");
    m_serial->write(buff);
}

void XhPort::isHeating()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002E005000104");
    m_serial->write(buff);
}

void XhPort::calibrationPlatform()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002E006000105");
    m_serial->write(buff);
}

void XhPort::isXYHeating()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002E007000106");
    m_serial->write(buff);
}

void XhPort::XYAdjust()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002E008000107");
    m_serial->write(buff);
}

void XhPort::powerlostsend()
{
    QByteArray s = QByteArray::fromHex("0103");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::powercancel()
{
    QByteArray s = QByteArray::fromHex("0104");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}



void XhPort::setFilament(QString l, QString r)
{
    QByteArray buff= m_package->filamentPage(l,r);
    m_serial->write(buff);
}

void XhPort::lup()
{
    QByteArray s = QByteArray::fromHex("0205006079FEFF");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::ldown()
{
    QByteArray s = QByteArray::fromHex("020500000186A0");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::rup()
{
    QByteArray s = QByteArray::fromHex("0205016079FEFF");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::rdown()
{
    QByteArray s = QByteArray::fromHex("020501000186A0");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

QByteArray XhPort::startPrint(QString m_filePath)
{
    QByteArray buff = m_package->chooseFile(m_filePath);
    return  buff;

}

void XhPort::AcsPrint(QByteArray a)
{
    m_serial->write(a);
}

void XhPort::pausePrint()
{
    QByteArray s = QByteArray::fromHex("0602");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::stopPrint()
{
    QByteArray s = QByteArray::fromHex("0601");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::regainPrint()
{
    QByteArray s = QByteArray::fromHex("060C");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::fileTemperature(QString l, QString r, QString bed)
{
    int l1 = l.indexOf(".");
    int r1 = r.indexOf(".");
    int b1 = bed.indexOf(".");

    int lnum = l.left(l1).toInt();
    int rnum = r.left(r1).toInt();
    int bnum = bed.left(b1).toInt();


    QByteArray a = "";
    a[0]='\x00';
    a[1]='\x00';
    a[2]='\x00';
    QByteArray b = "";
    b[0]='\x01';
    b[1]='\x00';
    b[2]='\x00';
    QByteArray x = "";
    x[0]='\x02';
    x[1]='\x00';
    x[2]='\x00';
    /*数据区计算*/
    while(lnum > 255)
    {
        lnum = lnum -256;
        a[2] = a[2]+'\x01';
    }
    for (;lnum > 0;lnum -- ) {
        a[1] = a[1] + '\x01';
    }
    while(rnum > 255)
    {
        rnum = rnum -256;
        b[2] = b[2]+'\x01';
    }
    for (;rnum > 0;rnum -- ) {
        b[1] = b[1] + '\x01';
    }
    while(bnum > 255)
    {
        bnum = bnum -256;
        x[2] = x[2]+'\x01';
    }
    for (;bnum > 0;bnum -- ) {
        x[1] = x[1] + '\x01';
    }

    QByteArray c ="";
    c[0]='\x02';
    c[1]='\x00';
    c.append(a);
    c.append(b);
    c.append(x);
    QByteArray d = m_package->groupPage(c);
    m_serial->write(d);
}

void XhPort::readyprint(int a, QByteArray b)
{
    switch (a) {
    case 0:
    {
        QByteArray s = QByteArray::fromHex("060D00");
        s.append(b);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
        break;
    case 2:
    {
        QByteArray s = QByteArray::fromHex("060D02");
        s.append(b);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
        break;
    case 3:
    {
        QByteArray s = QByteArray::fromHex("060D03");
        s.append(b);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
        break;
    case 4:
    {
        QByteArray s = QByteArray::fromHex("060D04");
        s.append(b);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
        break;
    case 5:
    {
        QByteArray s = QByteArray::fromHex("060D05");
        s.append(b);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
        break;
    case 6:
    {
        QByteArray s = QByteArray::fromHex("060D06");
        s.append(b);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
        break;
    default:
        break;
    }

}

void XhPort::enHotend(bool l, bool r)
{
    if(!l)
    {
        QByteArray s = QByteArray::fromHex("010701");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
    if(!r)
    {
        QByteArray s = QByteArray::fromHex("010702");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
}

void XhPort::enbackup(bool a )
{
    if(a)
    {
        QByteArray s = QByteArray::fromHex("011101");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
    else {
        QByteArray s = QByteArray::fromHex("011100");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
}

void XhPort::printcom(int a )
{
    if(a == 0)
    {
        QByteArray s = QByteArray::fromHex("010F00");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
    else {
        QByteArray s = QByteArray::fromHex("010F01");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
}

void XhPort::parcom(int a )
{
    if(a == 0)
    {
        QByteArray s = QByteArray::fromHex("010F00");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
    else {
        QByteArray s = QByteArray::fromHex("010F01");
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
}



void XhPort::askHotend()
{
    QByteArray s = QByteArray::fromHex("0106");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::askstate()
{
    QByteArray s = QByteArray::fromHex("010D");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::resume()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002001200060C");
    m_serial->write(buff);
}

void XhPort::abort()
{
    QByteArray buff = QByteArray::fromHex("4A46300200020007000601");
    m_serial->write(buff);
}

void XhPort::testr()
{
    QByteArray buff = QByteArray::fromHex("4A4630060006001D000B0210000000");
    m_serial->write(buff);
}

void XhPort::testb()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002000E000B03");
    m_serial->write(buff);
}

/*设备自检*/
void XhPort::selfTest()
{
    QByteArray s = QByteArray::fromHex("050A");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);

}

void XhPort::unFilament()
{
    QByteArray s = QByteArray::fromHex("0900");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::factoryReset()
{
    QByteArray s = QByteArray::fromHex("0901");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::p_nozzleHeating()
{
    QByteArray s = QByteArray::fromHex("030200");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::p_platformCalibration()
{
    QByteArray s = QByteArray::fromHex("030200");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::finish()
{
    QByteArray s = QByteArray::fromHex("0309");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::cancle()
{
    QByteArray s = QByteArray::fromHex("030A");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::n_nozzleHeating()
{
    QByteArray s = QByteArray::fromHex("0300");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::n_nozzleCalibration()
{
    QByteArray s = QByteArray::fromHex("0300");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::x_xyHeating()
{
    QByteArray s = QByteArray::fromHex("0405");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::x_platformCalibration()
{
    QByteArray s = QByteArray::fromHex("0309");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::x_xyCalibration()
{
    QByteArray s = QByteArray::fromHex("0305");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::l_hotendTest()
{
    QByteArray s = QByteArray::fromHex("0801");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::l_cooling()
{
    QByteArray s = QByteArray::fromHex("0802");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::r_hotendTest()
{
    QByteArray s = QByteArray::fromHex("0803");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

/*相对模式移动*/
void XhPort::relativeMove()
{

}

void XhPort::absoluteMove()
{

}

void XhPort::homeMove()
{

}

void XhPort::testdemo()
{
    QByteArray s = QByteArray::fromHex("84654875294581");
    QByteArray buff = m_package->groupPage(s);
//    qDebug()<<s.size();
    m_serial->write(buff);
}

/*打开串口*/
void XhPort::portInit(QString portname)
{
    m_serial->setPortName(portname);
    m_serial->open(QIODevice::ReadWrite);//打开串口
    if(m_serial->isOpen())
    {
        m_serial->setBaudRate(QSerialPort::Baud115200);//设置波特率为115200
        m_serial->setDataBits(QSerialPort::Data8);//设置数据位8
        m_serial->setParity(QSerialPort::NoParity);
        m_serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
        m_serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

        QObject::connect(m_serial,&QSerialPort::readyRead,this,&XhPort::readData);//连接信号槽
        QObject::connect(portTimer, &QTimer::timeout, this, &XhPort::serialTest);//连接信号槽
        portTimer->start(500);
    }
    else
    {
        qDebug()<<"serial open false!";
    }
}

void XhPort::selftest2()
{
    QByteArray s = QByteArray::fromHex("0506");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::selftest3()
{
    QByteArray s = QByteArray::fromHex("0507");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::selftest4()
{
    QByteArray s = QByteArray::fromHex("0508");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::selftest5()
{
    QByteArray s = QByteArray::fromHex("0509");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::backupsend()
{
    QByteArray s = QByteArray::fromHex("010E");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::backuptsend()
{
    QByteArray s = QByteArray::fromHex("0110");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::carbinfinished()
{
    QByteArray s = QByteArray::fromHex("0309");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::carbincancel()
{
    QByteArray s = QByteArray::fromHex("030A");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::updateBegin(QString update)
{
//    QByteArray s = QByteArray::fromHex("0500");
//    QByteArray buff = m_package->groupPage(s);
//    m_serial->write(buff);
    m_package->updateBegin(update);
}

void XhPort::selftest1()
{
    QByteArray s = QByteArray::fromHex("05050F");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

/*读串口readyread槽函数*/
void XhPort::readData()
{
    QByteArray revBuff;
    revBuff = m_serial->readAll();
    if(!revBuff.isEmpty())
    {
        /*串口是否打开*/
        if(serialOpen)
        {
            /*解析响应包*/
            m_package->analysis(revBuff);
        }
        else
        {
        /*没有打开就判断收到的消息是不是串口连接对应的回复*/
            QByteArray compareBuff = QByteArray::fromHex("4A4630030003000200010100");
            if(revBuff == compareBuff)
            {
                QObject::disconnect(portTimer, &QTimer::timeout, this, &XhPort::serialTest);//dis连接信号槽
                portTimer->stop();
                serialOpen = true;
                emit serialIsOpen();
            }
        }
    }
    else
    {
        qDebug()<<"no data read！";
    }
    revBuff.clear();
}

/*测试串口通讯*/
void XhPort::serialTest()
{
    QByteArray buff = QByteArray::fromHex("4A4630020002E002000101");
    m_serial->write(buff);
    if(serialOpen)
    {
        QObject::disconnect(portTimer, &QTimer::timeout, this, &XhPort::serialTest);//dis连接信号槽
        portTimer->stop();
    }
}

void XhPort::sendfile(QByteArray sendData)
{
    m_serial->write(sendData);
}

void XhPort::xhfirstTestResult(bool a, bool b, bool c, bool d, bool e)
{
    emit firstTestResult(a,b,c,d,e);
}

void XhPort::xhfirstTemperatureResult(int a, int b, int c, int d , int e, int f, int g, QByteArray data )
{
    emit firstTemperatureResult(a,b,c,d,e,f,g,data);
}

void XhPort::xhtoolTestResult(bool a, bool b, bool c, bool d, bool e, bool f)
{
    emit toolTestResult(a,b,c,d,e,f);
}

void XhPort::xhdisUseFilament(bool a)
{
    emit disUseFilament(a);
}

void XhPort::xhbackFactory(bool a)
{
    emit backFactory(a);
}

void XhPort::xhfilamentHeated(bool a )
{
    emit filamentHeated(a);
}

void XhPort::xhpNozzleHeating(bool a)
{
    emit pNozzleHeating(a);
}

void XhPort::xhpPlatformCalibration(qint32 a, qint32 b, qint32 c, qint32 d)
{
    emit pPlatformCalibration(a,b,c,d);
}

void XhPort::xhnNozzleHeating(bool a )
{
    emit nNozzleHeating(a);
}

void XhPort::xhnNozzleCalibration(int b)
{
    emit nNozzleCalibration(b);
}

void XhPort::xhxXyHeating(bool a)
{
    emit xXyHeating(a);
}

void XhPort::xhxPlatformCalibration(bool a )
{
    emit xPlatformCalibration(a);
}

void XhPort::xhxXyCalibration()
{
    emit xXyCalibration();
}

void XhPort::printCan()
{
    emit canPrint();
}

void XhPort::fileSendOverSlot()
{
    emit fileSendOver();
}

void XhPort::typeChang(int a, int b)
{
    emit type(a,b);
}

void XhPort::xhnoHeating(bool a)
{
    emit noHeating(a);
}

void XhPort::xhplatCheck(bool a)
{
    emit platCheck(a);
}

void XhPort::xhxNoHeating(bool a)
{
    emit xNoHeating(a);
}

void XhPort::xhxyCheck(bool a)
{
    emit xyCheck(a);
}

void XhPort::xhstopOk()
{
    emit stopOk();
}

void XhPort::xhpauseOk()
{
    emit pauseOk();
}

void XhPort::xhgoOnOk()
{
    emit goOnOk();
}

void XhPort::xhfinished()
{
    emit finished();
    setFilament("0","0");
}

void XhPort::xhcancle()
{
    emit canelk();
    setFilament("0","0");
}

void XhPort::xhprintend()
{
    emit printend();
}

void XhPort::xhpowerlost()
{
    emit powerlost();
}

void XhPort::xhfilamentlost()
{
    emit filamentlost();
}

void XhPort::xherror(int a )
{
    emit error(a);
}

void XhPort::xhselfTest1()
{
    emit selfTest1();
}

void XhPort::xhselfTest2()
{
    emit selfTest2();
}

void XhPort::xhselfTest3()
{
    emit selfTest3();
}

void XhPort::xhselfTest4()
{
    emit selfTest4();
}

void XhPort::xhselfTest5()
{
    emit selfTest5();
}

void XhPort::xhselfTest6()
{
    emit selfTest6();
}

void XhPort::xhstate(QString a,QString b,QString c,QString d,QString e,QString f,QString g)
{
    emit state(a ,b ,c ,d ,e ,f ,g );
}

void XhPort::xhcanone(int a)
{
    emit xcanone(a);
    qDebug()<<"port"<<a;
}

void XhPort::xhcantwo(int a)
{
    emit xcantwo(a);
    qDebug()<<"port"<<a;
}

void XhPort::updateBeginSlot()
{
    /*开始更新*/
    QByteArray s = QByteArray::fromHex("0500");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
    emit updateBeginsignl();
}

void XhPort::updateCheck(QByteArray a)
{
    m_serial->write(a);
}

void XhPort::updateSend(QByteArray a, int b, QByteArray c)
{
    if(b == 1)
    {
        m_serial->write(a);
    }
    if(b == 0)
    {
        m_serial->write(a);
        QByteArray s = QByteArray::fromHex("0502");
        s.append(c);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
    if(b == 2)
    {
        QByteArray s = QByteArray::fromHex("0502");
        s.append(c);
        QByteArray buff = m_package->groupPage(s);
        m_serial->write(buff);
    }
}

void XhPort::updateOver()
{
    QByteArray s = QByteArray::fromHex("050C");
    QByteArray buff = m_package->groupPage(s);
    m_serial->write(buff);
}

void XhPort::updateNum(int a)
{
    emit xhupdateNum(a);
}
