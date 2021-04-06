#ifndef SELECTMODE_H
#define SELECTMODE_H

#include <QWidget>

namespace Ui {
class selectMode;
}

class selectMode : public QWidget
{
    Q_OBJECT

public:
    explicit selectMode(QWidget *parent = nullptr);
    ~selectMode();

signals:
    void m_whatThis();
    void m_back();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::selectMode *ui;
};

#endif // SELECTMODE_H
