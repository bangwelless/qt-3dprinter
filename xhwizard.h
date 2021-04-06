#ifndef XHWIZARD_H
#define XHWIZARD_H

#include <QWidget>

namespace Ui {
class XhWizard;
}

class XhWizard : public QWidget
{
    Q_OBJECT

public:
    explicit XhWizard(QWidget *parent = nullptr);
    ~XhWizard();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void cancel();
    void confirm();

private:
    Ui::XhWizard *ui;
};

#endif // XHWIZARD_H
