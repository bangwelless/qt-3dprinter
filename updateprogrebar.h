#ifndef UPDATEPROGREBAR_H
#define UPDATEPROGREBAR_H

#include <QWidget>
#include <QQuickItem>

namespace Ui {
class updateProgreBar;
}

class updateProgreBar : public QWidget
{
    Q_OBJECT

public:
    explicit updateProgreBar(QWidget *parent = nullptr);
    ~updateProgreBar();


private:
    Ui::updateProgreBar *ui;
    QQuickItem *item;

public slots:
    void change(int );
signals:
    void sendSignalToQml(int );

};

#endif // UPDATEPROGREBAR_H
