#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QQuickWidget>
#include <QDialog>
#include <QString>
#include <QByteArray>
#include <QMap>
#include <QDebug>
#include <QListWidgetItem>
#include <QQuickItem>
#include <QTime>
#include <xhpage.h>
#include <xhport.h>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QDataStream>
#include <QMessageBox>
#include <QMutex>
#include <QStorageInfo>

#include "choosefile.h"
#include "parsing.h"
#include "askpause.h"
#include "selectmode.h"
#include "xhrest.h"
#include "xhsensor.h"
#include "xhwizard.h"
#include "delete.h"
#include "filecheckthread.h"
#include "choosetemp.h"
#include "unnoknfile.h"
#include "filamentfault.h"
#include "printflament.h"
#include "powerlost.h"
#include "printsetdlog.h"
#include "parsetdlog.h"
#include "dupandmirorr.h"
#include "duponly.h"
#include "mywifiitem.h"
#include "updateprogrebar.h"
#include "XhGcodeFileParser.h"
#include "mylistwidgetitem.h"
#ifdef XH_LINUX
#include "wifi_intf.h"
#include "wifi_udhcpc.h"
#include "xhcontrolr818.h"
#include <stdlib.h>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTemperatureLeft(int num1,int num2);
    void setTemperatureRight(int num,int num2);

//    void setTemperaturePicLeft(bool isBlue);
//    void setTemperaturePicRight(bool isBlue );

    void setBedPic(bool isVisible);
    void setBedTemperature(int num1,int num2);

    void setZnumber(float num);

    void setUSBpic(bool isVisible);
    void setLightPic(bool isVisible);
    void setWinPic(bool isVisible);

    void setPicColor(QWidget * pic,QWidget * lab,bool isgray);
    void m_addItemToList(const QString& fileName,QString filePath);
    void m_addItemToList(const QString& fileName,QString filePath,QString uDisk);
    void m_adTtemtowifi(const QString& wifiname,QString wifilevel);

private:
    Ui::MainWindow *ui;

    float m_opaCity;
    bool flicker;

    bool isPrintFirstAnim;

    bool isStartFristAnim;
    bool lheatend;
    bool rheatend;

    bool serialOpen;
    int printMode;
    int itemMode;
    int timeLonger;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    int printtime;
    QString lt;
    QString rt;
    QString bt;

    QString Duplicate_X_Offset;
    QString Mirror_X_offset;
    QString loaclPATH;
    QString udiskPATH;
    bool openMode;

    QByteArray offset;

    QGraphicsOpacityEffect *m_effect;
    QTimer m_timer;
    QTimer *fileListTimer;
    QTimer *printTimer;
    QTimer *print;
    QTimer *m_deepTimer;

    QMap<myListWidgetItem *,QListWidgetItem* > m_map;
    QMap<myWifiItem *,QListWidgetItem* >m_wmap;

    chooseFile * m_WinFiel;
    parsing * par;
    askPause * skpWin;
    selectMode * m_mode;
    Delete * m_delete;

    XhSensor *m_sensor;
    XhWizard *m_wizard;
    XhRest *m_rest;

    XhPort *m_port;
    fileCheckThread *m_thread;
    chooseTemp *mchoose;

    UnNoknfile *m_modeone;
    DupandMirorr *m_dam;
    duponly *m_dup;


    QQuickItem *item;
    QQuickItem *item1;
    QQuickItem *item3;
    QQuickItem *item4;
    filamentFault *m_filamentfault;
    printFlament *m_printfilament;

    powerLost *m_power;

    PrintSetDlog *m_setdlog;
    parsetdlog *m_parsetdlog;
    int32_t  offsetnum;

    myWifiItem * chooseit;
#ifdef XH_LINUX
    const aw_wifi_interface_t *aw_wifi;
    XhControlR818 *udpControl;
    QByteArray currentState;
#endif
    updateProgreBar *m_update;
    XhGcodeFileParser *m_fileParser;
    /*打印计时*/
    QTimer *m_printsec;
    QTime *m_time;


signals:
    void sendSignalToQml(int );
    void sendSignalHeating(int , int );


private slots:
    void firstStart();
    void jumpOne();
    void jumpTwo();
    void winGone();

    void winGtwo();
    void serailIsOpen();
    void jumpThree();

//    void winGfour();
    void winGfour(bool a,bool b,bool c,bool d,bool e);
    void temperatureChange(int a,int b,int c,int d,int e,int f,int g,QByteArray data);
    void jumpFour();

    void jumpFive();
    void jumpSix();
    void jumpSeven();

    void jumpEight();
    void jumpNine();
    void jumpTen();

    void jumpEleven();
    void shineone();
    void shinetwo();

    void jumptwelve();
    void jumpthirteen();
    void jumpFourteen();
    void jumpFifteen();

    void jumpSixteen();
    void askPrint();
    void jumpSeventeen();
    void jumpEightteen();
    void jumpnineteen();

    void jumpTwenty(bool a);
    void jump21(qint32 a,qint32 b,qint32 c,qint32 d);

    void jump22(bool a);
    void jump23(int b);

    void jump24(bool a);
    void jump25(bool a);
    void plat();

    void printstop();
    void printpause();
    void printagin();
    void printTime();

    void filamentTimeout();
    void filamentOK(bool a );
//    void toolSelfTest(bool a,bool b,bool c,bool d,bool e,bool f);
    void disfilament(bool a);
    void mfactoryReset();

    void planAdd(int ,int );
    void buprint();
    void powercancle();
    /******udp**************/
    void getCondition();
    void downloadOver(QString );
    /***/


    void m_deleteItem(myListWidgetItem * itm);
    void m_deleteTrue(myListWidgetItem * itm);
    void m_deleteFalse(myListWidgetItem * itm);
    void m_chooseItem(myListWidgetItem * itm);

    void connctwifi(myWifiItem* itm);
    void m_chooseEN();
    void sendChoose(int );
    void m_canPrintFile();
    void checkFile();
    void m_chooseUEN();
    void m_parcancel();

    void m_cancel();
    void m_backPrint();
    void m_whatThis();

    void sensorCancel();
    void sensorConfirm();

    void restCancel();
    void restConfirm();

    void wizardCancel();
    void wizardConfirm();

    void askStop();
    void askPaused();

    void fileList();

    void carilbin();
    void finished();
    void cancle();
    void nozzled();

    void xyheated();

    void printending();

    void ltemp(QString a);
    void rtemp(QString a);

    void plt(QString a);
    void prt(QString a);

    void oneprint();
    void oneparser();
    void onecancel();

    void tprint();
    void tcanle();
    void tmirro();
    void tdup();

    void dprint();
    void ddup();
    void dcancle();

    void powerlost();
    void filamentlost();

    void error(int );
    void Fresume();
    void Fabort();
    void Fchangefilament();

    void selftest1();
    void selftest2();
    void selftest3();
    void selftest4();
    void selftest5();
    void selftest6();

    void setclose(int a);

    void parsetclose(int ,int );
    void parsetexclose(bool ,bool ,bool );
    void paracktiv(bool );
    void findhotend();
    void cannext(bool );

    void state(QString ,QString ,QString ,QString ,QString ,QString ,QString );
    void canone(int a);
    void cantwo(int a);
    void up();
    void down();

    void updatebegin();
    void updateNumx(int );

    void    parseMode(QString printMode);
    void    parseDeepMode(QString printMode);
    void    parseHeader(QString left_temp, QString right_temp, QString bed_temp, QString offset);
    void    parseDeep();
    void    timeAdd();

    void    deepTimer();

    void on_noreadynext_clicked();

    void on_platnext_clicked();

    void on_readynext_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_87_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_63_clicked();

    void on_pushButton_64_clicked();

    void on_pushButton_76_clicked();

    void on_pushButton_72_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_60_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_74_clicked();

//    void on_pushButton_104_clicked();

    void on_pushButton_73_clicked();

    void on_pushButton_129_clicked();

    void on_pushButton_146_clicked();

    void on_pushButton_147_clicked();

    void on_pushButton_134_clicked();

//    void on_pushButton_135_clicked();

    void on_pushButton_169_clicked();

    void on_pushButton_174_clicked();

    void on_pushButton_175_clicked();

    void on_pushButton_176_clicked();

    void on_pushButton_177_clicked();

    void on_pushButton_182_clicked();

    void on_pushButton_183_clicked();

    void on_pushButton_188_clicked();

    void on_pushButton_189_clicked();

    void on_pushButton_194_clicked();

    void on_pushButton_195_clicked();

    void on_pushButton_200_clicked();

    void on_pushButton_201_clicked();

    void on_pushButton_206_clicked();

    void on_pushButton_207_clicked();

    void on_pushButton_212_clicked();

    void on_pushButton_213_clicked();

    void on_pushButton_218_clicked();

    void on_pushButton_219_clicked();

    void on_pushButton_224_clicked();

    void on_pushButton_225_clicked();

    void on_pushButton_230_clicked();

    void on_pushButton_235_clicked();

    void on_pushButton_236_clicked();

    void on_pushButton_237_clicked();

    void on_pushButton_242_clicked();

//    void on_pushButton_258_clicked();

    void on_pushButton_259_clicked();

    void on_pushButton_286_clicked();

    void on_pushButton_287_clicked();

    void on_pushButton_302_clicked();

    void on_pushButton_303_clicked();

    void on_pushButton_631_clicked();

    void on_pushButton_632_clicked();

    void on_pushButton_647_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_233_clicked();

    void on_pushButton_180_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_90_clicked();

    void on_pushButton_92_clicked();

    void on_pushButton_91_clicked();

    void on_pushButton_85_clicked();

    void on_pushButton_86_clicked();

    void on_pushButton_88_clicked();

    void on_pushButton_117_clicked();

    void on_pushButton_116_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_59_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_65_clicked();

    void on_pushButton_66_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_70_clicked();

    void on_pushButton_71_clicked();

    void on_pushButton_75_clicked();

    void on_pushButton_170_clicked();

    void on_pushButton_172_clicked();

    void on_pushButton_173_clicked();

    void on_pushButton_314_clicked();

    void on_pushButton_315_clicked();

    void on_pushButton_316_clicked();

    void on_pushButton_649_clicked();

    void on_pushButton_650_clicked();

    void on_pushButton_648_clicked();

    void on_pushButton_653_clicked();

    void on_pushButton_654_clicked();

    void on_pushButton_652_clicked();

    void on_pushButton_657_clicked();

    void on_pushButton_658_clicked();

    void on_pushButton_656_clicked();

    void on_pushButton_341_clicked();

    void on_pushButton_670_clicked();

    void on_pushButton_668_clicked();

    void on_pushButton_669_clicked();

    void on_pushButton_671_clicked();

    void on_pushButton_667_clicked();

    void on_pushButton_662_clicked();

    void on_pushButton_659_clicked();

    void on_pushButton_655_clicked();

    void on_pushButton_651_clicked();

    void on_pushButton_179_clicked();

    void on_pushButton_181_clicked();

    void on_pushButton_178_clicked();

    void on_pushButton_186_clicked();

    void on_pushButton_185_clicked();

    void on_pushButton_187_clicked();

    void on_pushButton_193_clicked();

    void on_pushButton_190_clicked();

    void on_pushButton_191_clicked();

    void on_pushButton_199_clicked();

    void on_pushButton_197_clicked();

    void on_pushButton_198_clicked();

    void on_pushButton_202_clicked();

    void on_pushButton_203_clicked();

    void on_pushButton_205_clicked();

    void on_pushButton_210_clicked();

    void on_pushButton_209_clicked();

    void on_pushButton_211_clicked();

    void on_pushButton_216_clicked();

    void on_pushButton_214_clicked();

    void on_pushButton_215_clicked();

    void on_pushButton_220_clicked();

    void on_pushButton_223_clicked();

    void on_pushButton_221_clicked();

    void on_pushButton_227_clicked();

    void on_pushButton_226_clicked();

    void on_pushButton_228_clicked();

    void on_pushButton_243_clicked();

    void on_pushButton_231_clicked();

    void on_pushButton_232_clicked();

    void on_pushButton_234_clicked();

    void on_pushButton_240_clicked();

    void on_pushButton_238_clicked();

    void on_pushButton_239_clicked();

    void on_pushButton_241_clicked();

    void on_pushButton_248_clicked();

    void on_pushButton_256_clicked();

    void on_pushButton_257_clicked();

    void on_pushButton_254_clicked();

    void on_pushButton_255_clicked();

    void on_pushButton_284_clicked();

    void on_pushButton_285_clicked();

    void on_pushButton_282_clicked();

    void on_pushButton_283_clicked();

    void on_pushButton_292_clicked();

//    void on_pushButton_297_clicked();

    void on_pushButton_298_clicked();

    void on_pushButton_301_clicked();

    void on_pushButton_300_clicked();

    void on_pushButton_299_clicked();

    void on_pushButton_628_clicked();

    void on_pushButton_629_clicked();

    void on_pushButton_627_clicked();

    void on_pushButton_630_clicked();

    void on_pushButton_637_clicked();

//    void on_pushButton_642_clicked();

    void on_pushButton_643_clicked();

    void on_pushButton_644_clicked();

    void on_pushButton_646_clicked();

    void on_pushButton_645_clicked();

    void on_pushButton_347_clicked();

    void on_pushButton_342_clicked();

    void on_pushButton_348_clicked();

    void on_pushButton_349_clicked();

    void on_pushButton_340_clicked();

    void on_pushButton_343_clicked();

    void on_pushButton_679_clicked();

    void on_pushButton_676_clicked();

    void on_pushButton_678_clicked();

    void on_pushButton_677_clicked();

    void on_pushButton_89_clicked();

    void on_pushButton_128_clicked();

    void on_pushButton_125_clicked();

    void on_pushButton_126_clicked();

    void on_pushButton_130_clicked();

    void on_pushButton_133_clicked();

    void on_pushButton_132_clicked();

    void on_pushButton_131_clicked();

    void on_pushButton_141_clicked();

    void on_pushButton_168_clicked();

    void on_pushButton_118_clicked();

    void on_pushButton_113_clicked();

    void on_pushButton_149_clicked();

    void on_pushButton_151_clicked();

    void on_pushButton_148_clicked();

    void on_pushButton_160_clicked();

    void on_pushButton_167_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_346_clicked();

    void on_pushButton_354_clicked();

//    void on_pushButton_102_clicked();

//    void on_pushButton_103_clicked();

    void on_pushButton_355_clicked();

    void on_pushButton_253_clicked();

    void on_pushButton_365_clicked();

    void on_pushButton_367_clicked();

    void on_pushButton_375_clicked();

    void on_pushButton_308_clicked();

    void on_pushButton_109_clicked();

    void on_pushButton_110_clicked();

    void on_pushButton_358_clicked();

    void on_pushButton_111_clicked();

    void on_pushButton_112_clicked();

    void on_pushButton_114_clicked();

    void on_pushButton_115_clicked();

    void on_pushButton_105_clicked();

    void on_pushButton_107_clicked();

    void on_pushButton_108_clicked();

//    void on_pushButton_360_clicked();

    void on_pushButton_140_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_99_clicked();

    void on_pushButton_356_clicked();

    void on_pushButton_263_clicked();

    void on_pushButton_262_clicked();

    void on_pushButton_124_clicked();

//    void on_pushButton_136_clicked();s

//    void on_pushButton_144_clicked();

    void on_pushButton_100_clicked();

    void on_pushButton_101_clicked();



    void on_pushButton_371_clicked();

    void on_pushButton_373_clicked();

    void on_pushButton_164_clicked();

    void on_pushButton_393_clicked();

    void on_pushButton_313_clicked();

    void on_pushButton_387_clicked();

    void on_pushButton_418_clicked();

    void on_pushButton_138_clicked();

    void on_pushButton_139_clicked();

    void on_pushButton_142_clicked();

    void on_pushButton_143_clicked();

    void on_pushButton_144_clicked();

    void on_pushButton_145_clicked();

    void on_pushButton_150_clicked();

    void on_pushButton_152_clicked();

    void on_pushButton_153_clicked();

    void on_pushButton_154_clicked();

    void on_pushButton_155_clicked();

    void on_pushButton_156_clicked();

    void on_pushButton_157_clicked();

    void on_pushButton_158_clicked();

    void on_pushButton_159_clicked();

    void on_pushButton_163_clicked();

    void on_pushButton_165_clicked();

    void on_pushButton_166_clicked();

    void on_pushButton_258_clicked();

    void on_pushButton_265_clicked();

    void on_pushButton_267_clicked();

    void on_pushButton_297_clicked();

    void on_pushButton_311_clicked();

    void on_pushButton_385_clicked();

    void on_pushButton_382_clicked();

    void on_pushButton_398_clicked();

    void on_pushButton_381_clicked();

    void on_pushButton_397_clicked();

    void on_pushButton_389_clicked();

    void on_pushButton_392_clicked();

    void on_pushButton_407_clicked();

    void on_pushButton_399_clicked();

    void on_pushButton_384_clicked();

    void on_pushButton_383_clicked();

    void on_pushButton_404_clicked();

    void on_pushButton_388_clicked();

    void on_pushButton_390_clicked();

    void on_pushButton_395_clicked();

    void on_pushButton_406_clicked();

    void on_pushButton_403_clicked();

    void on_pushButton_386_clicked();

    void on_pushButton_394_clicked();

    void on_pushButton_405_clicked();

    void on_pushButton_360_clicked();

    void on_pushButton_400_clicked();

    void on_pushButton_396_clicked();

    void on_pushButton_379_clicked();

    void on_pushButton_345_clicked();

    void on_pushButton_380_clicked();

    void on_pushButton_416_clicked();

    void on_pushButton_413_clicked();

    void on_pushButton_429_clicked();

    void on_pushButton_412_clicked();

    void on_pushButton_428_clicked();

    void on_pushButton_420_clicked();

    void on_pushButton_423_clicked();

    void on_pushButton_438_clicked();

    void on_pushButton_430_clicked();

    void on_pushButton_415_clicked();

    void on_pushButton_414_clicked();

    void on_pushButton_435_clicked();

    void on_pushButton_419_clicked();

    void on_pushButton_421_clicked();

    void on_pushButton_426_clicked();

    void on_pushButton_437_clicked();

    void on_pushButton_434_clicked();

    void on_pushButton_417_clicked();

    void on_pushButton_425_clicked();

    void on_pushButton_424_clicked();

    void on_pushButton_436_clicked();

    void on_pushButton_409_clicked();

    void on_pushButton_431_clicked();

    void on_pushButton_427_clicked();

    void on_pushButton_410_clicked();

    void on_pushButton_408_clicked();

    void on_pushButton_411_clicked();

    void on_pushButton_689_clicked();

    void on_pushButton_691_clicked();

    void on_pushButton_690_clicked();

    void on_pushButton_642_clicked();

    void on_pushButton_312_clicked();

    void on_pushButton_401_clicked();

    void on_pushButton_432_clicked();

    void on_pushButton_433_clicked();

    void on_pushButton_402_clicked();

    void on_pushButton_688_clicked();

    void on_pushButton_447_clicked();

    void on_pushButton_692_clicked();

    void on_pushButton_453_clicked(bool checked);

    void on_pushButton_454_clicked(bool checked);

    void on_pushButton_455_clicked();

signals:
    void detection(bool ready);
    void updateNum(int );
    void sendCondition(QByteArray data);


};

#endif // MAINWINDOW_H
