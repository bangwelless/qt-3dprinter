#ifndef XHSENSOR_H
#define XHSENSOR_H

#include <QWidget>

namespace Ui {
class XhSensor;
}

class XhSensor : public QWidget
{
    Q_OBJECT

public:
    explicit XhSensor(QWidget *parent = nullptr);
    ~XhSensor();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void cancel();
    void confirm();

private:
    Ui::XhSensor *ui;
};

#endif // XHSENSOR_H
