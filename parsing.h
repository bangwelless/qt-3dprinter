#ifndef PARSING_H
#define PARSING_H

#include <QWidget>
#include <QQuickItem>

namespace Ui {
class parsing;
}

class parsing : public QWidget
{
    Q_OBJECT

public:
    explicit parsing(QWidget *parent = nullptr);
    ~parsing();
    void setNum(int num);

private slots:
    void on_pushButton_clicked();

private:
    Ui::parsing *ui;
    QQuickItem *item;

signals:
    void cancel();
    void sendSignalToQml(int );
};

#endif // PARSING_H
