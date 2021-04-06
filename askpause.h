#ifndef ASKPAUSE_H
#define ASKPAUSE_H

#include <QWidget>

namespace Ui {
class askPause;
}

class askPause : public QWidget
{
    Q_OBJECT

public:
    explicit askPause(QWidget *parent = nullptr);
    ~askPause();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


signals:
    void cancel();
    void m_stop();
    void m_paused();

private:
    Ui::askPause *ui;
};

#endif // ASKPAUSE_H
