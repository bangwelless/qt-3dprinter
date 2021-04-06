#ifndef XHREST_H
#define XHREST_H

#include <QWidget>

namespace Ui {
class XhRest;
}

class XhRest : public QWidget
{
    Q_OBJECT

public:
    explicit XhRest(QWidget *parent = nullptr);
    ~XhRest();


signals:
    void cancel();
    void confirm();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::XhRest *ui;
};

#endif // XHREST_H
