#include "filecheckthread.h"

fileCheckThread::fileCheckThread(QObject *par): QThread (par)
{

}

fileCheckThread::fileCheckThread(QObject *par, std::function<QByteArray (QString)> func, QString args):
    QThread (par)
{
    m_func = func;
    filepath =args;
}

fileCheckThread::~fileCheckThread()
{

}

void fileCheckThread::run()
{
    QByteArray buff =  m_func(filepath);
    emit fileCheck(0);
    this->exec();
}
