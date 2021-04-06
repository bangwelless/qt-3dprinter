#ifndef DUPANDMIRORR_H
#define DUPANDMIRORR_H

#include <QWidget>

namespace Ui {
class DupandMirorr;
}

class DupandMirorr : public QWidget
{
    Q_OBJECT

public:
    explicit DupandMirorr(QWidget *parent = nullptr);
    ~DupandMirorr();
    void changeMode(int n);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::DupandMirorr *ui;
    static int num;
signals:
    void print();
    void mirro();
    void dup();
    void cancle();
};

#endif // DUPANDMIRORR_H
