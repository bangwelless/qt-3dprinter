#ifndef PRINTFLAMENT_H
#define PRINTFLAMENT_H

#include <QWidget>

namespace Ui {
class printFlament;
}

class printFlament : public QWidget
{
    Q_OBJECT

public:
    explicit printFlament(QWidget *parent = nullptr);
    ~printFlament();
    void init(QString str);
private:
    Ui::printFlament *ui;

signals:
    void heatT(QString str);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
};

#endif // PRINTFLAMENT_H
