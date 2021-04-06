#ifndef PRINTSETDLOG_H
#define PRINTSETDLOG_H

#include <QWidget>

namespace Ui {
class PrintSetDlog;
}

class PrintSetDlog : public QWidget
{
    Q_OBJECT

public:
    explicit PrintSetDlog(QWidget *parent = nullptr);
    ~PrintSetDlog();
    void initnum(QString a, QString b, QString c, QString d, QString e, QString f, QString g);
    void backup(int a );

private slots:
    void on_pushButton_clicked();

private:
    Ui::PrintSetDlog *ui;


signals:
    void closeset(int );


};

#endif // PRINTSETDLOG_H
