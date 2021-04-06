#ifndef POWERLOST_H
#define POWERLOST_H

#include <QWidget>

namespace Ui {
class powerLost;
}

class powerLost : public QWidget
{
    Q_OBJECT

public:
    explicit powerLost(QWidget *parent = nullptr);
    ~powerLost();

private slots:
    void on_pushButton_2_clicked();

signals:
    void cancel();

private:
    Ui::powerLost *ui;
};

#endif // POWERLOST_H
