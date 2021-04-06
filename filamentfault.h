#ifndef FILAMENTFAULT_H
#define FILAMENTFAULT_H

#include <QWidget>

namespace Ui {
class filamentFault;
}

class filamentFault : public QWidget
{
    Q_OBJECT

public:
    explicit filamentFault(QWidget *parent = nullptr);
    ~filamentFault();

private:
    Ui::filamentFault *ui;
signals:
    void resume();
    void abort();
    void changeFilament();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
};

#endif // FILAMENTFAULT_H
