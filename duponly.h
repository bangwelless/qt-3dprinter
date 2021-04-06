#ifndef DUPONLY_H
#define DUPONLY_H

#include <QWidget>

namespace Ui {
class duponly;
}

class duponly : public QWidget
{
    Q_OBJECT

public:
    explicit duponly(QWidget *parent = nullptr);
    ~duponly();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::duponly *ui;

signals:
    void dup();
    void onlyprint();
    void cancle();

};

#endif // DUPONLY_H
