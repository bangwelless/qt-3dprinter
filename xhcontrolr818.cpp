#include "xhcontrolr818.h"
#define localPath "/usr/share/3d_printer/tmp/"
#define XH_LITTLE_BIT_MERGE_32(d1, d2, d3, d4)      ( (((d4) << 24) & 0xFF000000) | (((d3) << 16) & 0xFF0000) | (((d2) << 8) & 0xFF00) | ((d1) & 0xFF))
#define INT_TO_ARRAY(output, input)                             \
    do {                                                        \
        (output)[0] = (char ) ((input) & 0x000000FF);         \
        (output)[1] = (char ) (((input) >> 8) & 0x000000FF);  \
        (output)[2] = (char ) (((input) >> 16) & 0x000000FF); \
        (output)[3] = (char ) (((input) >> 24) & 0x000000FF); \
    } while(0)


XhControlR818::XhControlR818(QWidget *parent) : QWidget(parent)
{
    inputFile = new QFile;
    if(initUdpSocket()<0)
    {
        qDebug()<<"init socket false!";
        return;
    }
}

XhControlR818::~XhControlR818()
{
    delete inputFile;
}

int XhControlR818::initUdpSocket()
{
    udpSocket = new QUdpSocket;
    if(!udpSocket->bind(QHostAddress::AnyIPv4,8899))
    {
        return -1;
    }
    QObject::connect(udpSocket,&QUdpSocket::readyRead,this,&XhControlR818::receive);
    cmdSocket = new QUdpSocket;
    if(!cmdSocket->bind(QHostAddress::AnyIPv4,12365))
    {
        return -1;
    }
    QHostInfo add = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address,add.addresses())
    {
         if(address.protocol() == QAbstractSocket::IPv4Protocol)
             qDebug() << address.toString();
         QString x = address.toString()+":12365";
         localAddress = x.toLatin1();
    }
    QObject::connect(cmdSocket,&QUdpSocket::readyRead,this,&XhControlR818::receive);
    return 0;
}

void XhControlR818::analysis(QByteArray data)
{
    if(data[0]!='\x4A'||data[1]!='\x46'||data[2]!='\x31')
    {
        qDebug()<<"Not is JD V31 ";
        return;
    }
    int dataSize = XH_LITTLE_BIT_MERGE_32(data[3],data[4],data[5],data[6]);
    QByteArray comData = data.mid(11,dataSize);
    qDebug()<<comData;
    switch (comData[0]) {
    case '\x08':
    {
        if(comData[1] == '\x00')
            hostAnswer(comData);
        if(comData[1] == '\x01')
            hostConnect(comData);
        if(comData[1] == '\x02')
            hostDisconnect(comData);
    }
        break;
    case '\x01':
        emit signalsAskCondition();
        break;
    case '\x02':

        break;
    case '\x06':
        if(comData[1] == '\x00'){
            readyWriteFile(comData);
            askFilePage();
        }
        if(comData[1] == '\x01')
            getWriteFile(comData);
        if(comData[1] == '\x02')
            finishWriteFile();
        if(comData[1] == '\x03')
            analysisPrintMode(comData);
        if(comData[1] == '\x04')
            emit signalsStopPrint();
        if(comData[1] == '\x05')
            emit signalsPausePrint();
        if(comData[1] == '\x06')
            emit signalsResumePrint();
        break;
    default:
        break;

    }
}

QByteArray XhControlR818::group(QByteArray data)
{
    QByteArray sendPage = QByteArray::fromHex("4A4631");

    QByteArray pagelen;
    pagelen[0]='\x00';pagelen[1]='\x00';
    pagelen[2]='\x00';pagelen[3]='\x00';
    int len = data.size();
    INT_TO_ARRAY(pagelen,len);
    sendPage.append(pagelen);

    quint16 lencheck = pagelen[3]^pagelen[2]^pagelen[1]^pagelen[0];
    char checklen[2]={0};
    checklen[1] = static_cast< char>((lencheck >> 8) & 0xFF);
    checklen[0] = static_cast< char>((lencheck >> 0) & 0xFF);
    sendPage.append(checklen,2);

    quint16 nature = 0;
    for (int i = 0;i<data.size();i++) {
        nature = nature + (quint8)data[i];
    }
    char check[2]={0};
    check[1] = static_cast< char>((nature >> 8) & 0xFF);
    check[0] = static_cast< char>((nature >> 0) & 0xFF);
    sendPage.append(check,2);
    sendPage.append(data);
    return sendPage;
}

void XhControlR818::hostAnswer(QByteArray data)
{
    QByteArray com = QByteArray::fromHex("088000");
//    com.append(localAddress);
    QByteArray send = group(com);
    udpSocket->writeDatagram(send,hostAddress,hostPort);
}

void XhControlR818::hostConnect(QByteArray data)
{
    QByteArray com = QByteArray::fromHex("088100");
    QByteArray send = group(com);
    udpSocket->writeDatagram(send,hostAddress,hostPort);
}

void XhControlR818::hostDisconnect(QByteArray data)
{
    QByteArray com = QByteArray::fromHex("088200");
    QByteArray send = group(com);
    udpSocket->writeDatagram(send,hostAddress,hostPort);
}

void XhControlR818::analysisPrintMode(QByteArray data)
{
    int mode = (int )data[2];
    QByteArray printFileName = data.mid(3,data.size()-3);
    QString printFile ;
    printFile.prepend(printFileName);
    emit signalsStartPrint(printFile,mode);
}

void XhControlR818::readyWriteFile(QByteArray data)
{
    pageNumber = 0;
    fileSize = 0;
    sum = 0;
    int i = 0;
    for (i =0;i<data.size();i++) {
        if(data[i] == '\0')
        {
            if(i>1)
            break;
        }
    }
    QString fileName = data.mid(2,i-2);
    qDebug()<<fileName;
    fileSize = (int ) XH_LITTLE_BIT_MERGE_32(data[i+1],data[i+2],data[i+3],data[i+4]);
    sum = (int ) XH_LITTLE_BIT_MERGE_32(data[i+5],data[i+6],data[i+7],data[i+8]);
    qDebug()<<fileSize<<sum;
    inputFile->setFileName(localPath + fileName);
    inputFileName.clear();
    inputFileName = fileName;
    if(!inputFile->open(QIODevice::WriteOnly))
    {
        qDebug()<<" disopen ";
        return;
    }
    QByteArray com = QByteArray::fromHex("068000");
    QByteArray send = group(com);
    cmdSocket->writeDatagram(send,hostAddress,hostPort);
    askFilePage();
}

void XhControlR818::getWriteFile(QByteArray data)
{
    QByteArray writedata=  data.right(data.size()-5);
    inputFile->write(writedata);
    inputFile->flush();
    askFilePage();
}

void XhControlR818::askFilePage()
{
    if(pageNumber > sum)
        return;
    QByteArray com = QByteArray::fromHex("0681");
    char checklen[2]={0};
    checklen[1] = static_cast< char>((pageNumber >> 8) & 0xFF);
    checklen[0] = static_cast< char>((pageNumber >> 0) & 0xFF);
    pageNumber++;
    com.append(checklen,2);
    QByteArray send = group(com);
    cmdSocket->writeDatagram(send,hostAddress,hostPort);
}

void XhControlR818::finishWriteFile()
{
    inputFile->close();
    QByteArray com = QByteArray::fromHex("068200");
    QByteArray send = group(com);
    cmdSocket->writeDatagram(send,hostAddress,hostPort);
    emit signalsDownloadOver(inputFileName);
}

void XhControlR818::receive()
{
    QByteArray dataBuffer;
    dataBuffer.resize((int )udpSocket->bytesAvailable());
    QByteArray dataBuffer1;
    dataBuffer1.resize((int )cmdSocket->bytesAvailable());
    if(udpSocket->readDatagram(dataBuffer.data(),dataBuffer.size(),&hostAddress,&hostPort)>0)
    analysis(dataBuffer);
    if(cmdSocket->readDatagram(dataBuffer1.data(),dataBuffer1.size(),&hostAddress,&hostPort)>0)
    analysis(dataBuffer1);
}

void XhControlR818::slotAskCondition(QByteArray data)
{
    QByteArray com = QByteArray::fromHex("0180");
    com.append(data.mid(2,data.size()-2));
    QByteArray send = group(com);
    udpSocket->writeDatagram(send,hostAddress,hostPort);
}
