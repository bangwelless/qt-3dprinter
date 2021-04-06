#ifndef XHCONTROLR818_H
#define XHCONTROLR818_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostInfo>
#include <QDebug>
#include <QtNetwork>
#include <QByteArray>
/*作为接受控制的模块*/
class XhControlR818 : public QWidget
{
    Q_OBJECT
public:
    explicit XhControlR818(QWidget *parent = nullptr);
    ~XhControlR818();
private:
    QUdpSocket *udpSocket;
    QUdpSocket *cmdSocket;
    /*信息*/
    QHostAddress hostAddress;
    QByteArray localAddress;
    quint16 hostPort;
    int initUdpSocket();

    int pageNumber;
    int sum;
    int fileSize;
    QFile *inputFile;
    QString inputFileName;

    void analysis(QByteArray data);
    QByteArray group(QByteArray data);

    void hostAnswer(QByteArray data);
    void hostConnect(QByteArray data);
    void hostDisconnect(QByteArray data);
//    void askCondition();
    void analysisPrintMode(QByteArray data);

    void readyWriteFile(QByteArray data);
    void getWriteFile(QByteArray data);
    void askFilePage();
    void finishWriteFile();


signals:
    void signalsAskCondition();
    void signalsStartPrint(QString fileName,int mode);
    void signalsStopPrint();
    void signalsPausePrint();
    void signalsResumePrint();
    void signalsDownloadOver(QString fileName);
public slots:
    void receive();
    void slotAskCondition(QByteArray data);
};

#endif // XHCONTROLR818_H
