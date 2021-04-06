#ifndef FILECHECKTHREAD_H
#define FILECHECKTHREAD_H

#include <QObject>
#include <QByteArray>
#include <QThread>

#include <functional>

class fileCheckThread : public QThread
{
    Q_OBJECT

public:
    fileCheckThread(QObject *par);
    fileCheckThread(QObject *par,std::function<QByteArray (QString)> func, QString args);
    virtual ~fileCheckThread();
    void run();


private:
    std::function<QByteArray (QString)> m_func;
    QString filepath;

signals:
    void fileCheck(int);

};

#endif // FILECHECKTHREAD_H
