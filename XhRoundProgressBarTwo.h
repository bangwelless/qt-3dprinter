#ifndef XHROUNDPROGRESSBARTWO_H
#define XHROUNDPROGRESSBARTWO_H

#include <QWidget>

class XhRoundProgressBarTwo : public QWidget
{
    Q_OBJECT
public:
    explicit XhRoundProgressBarTwo(QWidget *parent = nullptr);

public slots:
    void            setValue(int left, int right);

signals:

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    int     m_leftValue{ 0 }, m_rightValue{ 0 };
};

#endif // XHROUNDPROGRESSBARTWO_H
