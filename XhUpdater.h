#ifndef XHUPDATER_H
#define XHUPDATER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QJsonDocument>

class XhUpdater : public QThread
{
    Q_OBJECT

public:
    explicit XhUpdater(QObject *parent = nullptr);
    virtual ~XhUpdater() override;

    void        startUpdate(const QString &fileName);

signals:
    void        completed();

protected:
    virtual void    run() override;

private:
    constexpr static const char* const m_k3dPrinterDir = "/usr/share/3d_printer/";
    constexpr static const char* const m_k3dPrinterTmpDir = "/usr/share/3d_printer/tmp";
    constexpr static const char* const m_kRuleFileName = "update_rule.json";

private:
    QMutex      m_mutex;
    QString     m_fileName;

private:
    QString     fileNameSafety();
    void        decompression(const QString &fileName);
    void        updateFlag();

private:
    static QJsonDocument        readRule();
    static void                 writeRule(const QJsonDocument &doc);
};

#endif // XHUPDATER_H

