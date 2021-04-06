#ifndef PARSETDLOG_H
#define PARSETDLOG_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class parsetdlog;
}

class parsetdlog : public QWidget
{
    Q_OBJECT

public:
    explicit parsetdlog(QWidget *parent = nullptr);
    ~parsetdlog();
    void seten(bool ,bool );
    void initnum(QString a, QString b, QString c, QString d, QString e, QString f, QString g);
    void backup(int );
    void backupt(int );
    QTimer *m_timer;
private slots:

    void  timeou();
    void  time2();
    void on_pushButton_12_clicked();

    void on_pushButton_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_14_clicked(bool checked);

    void on_pushButton_15_clicked(bool checked);

    void on_pushButton_13_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::parsetdlog *ui;

signals:
    void next();
    void complete(int ,int );
    void excomplete(bool ,bool ,bool );
    void active(bool );
    void up();
    void down();




};

#endif // PARSETDLOG_H
