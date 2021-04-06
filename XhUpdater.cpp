
#include "XhUpdater.h"

#include <QProcess>
#include <QJsonObject>
#include <QFile>
#include <QJsonParseError>

XhUpdater::XhUpdater(QObject *parent) :
    QThread(parent)
{

}

XhUpdater::~XhUpdater()
{

}

void XhUpdater::startUpdate(const QString &fileName)
{
    m_mutex.lock();
    m_fileName = fileName;
    m_mutex.unlock();

    start();
}

void XhUpdater::run()
{
    QString fileName = fileNameSafety();
    decompression(fileName);
    updateFlag();

    emit completed();
}

QString XhUpdater::fileNameSafety()
{
    QString ret;

    m_mutex.lock();
    ret = m_fileName;
    m_mutex.unlock();

    return ret;
}

void XhUpdater::decompression(const QString &fileName)
{
    QString command = QString("tar -xvf %1 -C %2/").arg(fileName).arg(m_k3dPrinterTmpDir);
    QProcess::execute(command);
    QProcess::execute(QString("chmod 777 %1/qt-3dprint").arg(m_k3dPrinterTmpDir));
    system(QString::asprintf("chmod 777 %s/*",m_k3dPrinterTmpDir).toStdString().c_str());
}

void XhUpdater::updateFlag()
{
    QJsonDocument doc = readRule();
    QJsonObject root = doc.object();
    root["is_update"] = true;
    doc.setObject(root);
    QFile out(QString("%1/%2").arg(m_k3dPrinterDir).arg(m_kRuleFileName));
    if (!out.open(QIODevice::ReadWrite))
        return;
    QByteArray byte_array = doc.toJson();
    out.seek(0);
    out.write(byte_array);
    out.close();
}

QJsonDocument XhUpdater::readRule()
{
    QJsonDocument ret;

    QFile in(QString("%1/%2").arg(m_k3dPrinterDir).arg(m_kRuleFileName));
    if (!in.open(QIODevice::ReadOnly))
        return ret;
    QByteArray readData = in.readAll();

    QJsonParseError json_error;

    QJsonDocument docOut = QJsonDocument::fromJson(readData,&json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
    }

    return ret;
}

void XhUpdater::writeRule(const QJsonDocument &doc)
{
    QFile out(QString("%1/%2").arg(m_k3dPrinterTmpDir).arg(m_kRuleFileName));
    if (!out.open(QIODevice::WriteOnly|QIODevice::Truncate))
        return;
    out.write(doc.toJson());
    out.close();
}

