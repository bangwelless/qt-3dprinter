#include "XhRoundProgressBarTwo.h"


#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QPen>

XhRoundProgressBarTwo::XhRoundProgressBarTwo(QWidget *parent) : QWidget(parent)
{

}

void XhRoundProgressBarTwo::setValue(int left, int right)
{
    m_leftValue = left;
    m_rightValue = right;

    update();
}

void XhRoundProgressBarTwo::paintEvent(QPaintEvent *event)
{
    event->accept();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(58);
    painter.setPen(pen);

    int start, end;
    start = (270 - m_leftValue * 1.8) * 16;
    end = (m_leftValue + m_rightValue) * 16 * 1.8;

    pen.setColor("#1A1A1A");
    painter.setPen(pen);
    painter.drawArc(pen.width(), pen.width(), width()-(pen.width()*2), height()-(pen.width()*2), 0*16, 360*16);

    pen.setColor("#F3B67B");
    painter.setPen(pen);
    painter.drawArc(pen.width(), pen.width(), width()-(pen.width()*2), height()-(pen.width()*2), start, end);
}
