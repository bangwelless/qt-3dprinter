#ifndef MYWIFIITEM_H
#define MYWIFIITEM_H

#include <QWidget>
#include <QString>
#include <QDebug>

namespace Ui {
class myWifiItem;
}

class myWifiItem : public QWidget
{
    Q_OBJECT

public:
    explicit myWifiItem(QWidget *parent = nullptr);
    ~myWifiItem();
    void addwifi(QString name,QString level);
    QString wifiname;
    QString wifilevel;
private:
    Ui::myWifiItem *ui;
signals:
    void conncetwifi(myWifiItem *);
private slots:
    void on_pushButton_clicked();
};

#endif // MYWIFIITEM_H
