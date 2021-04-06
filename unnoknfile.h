#ifndef UNNOKNFILE_H
#define UNNOKNFILE_H

#include <QWidget>

namespace Ui {
class UnNoknfile;
}

class UnNoknfile : public QWidget
{
    Q_OBJECT

public:
    explicit UnNoknfile(QWidget *parent = nullptr);
    ~UnNoknfile();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::UnNoknfile *ui;

signals:
    void cancle();
    void print();
    void parser();
};

#endif // UNNOKNFILE_H
