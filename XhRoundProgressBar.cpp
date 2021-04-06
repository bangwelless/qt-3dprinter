#include "XhRoundProgressBar.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

XhRoundProgressBar::XhRoundProgressBar(QWidget *parent)
    : QWidget(parent)
{

}

XhRoundProgressBar::~XhRoundProgressBar()
{

}

void XhRoundProgressBar::setRingWidth(qreal width)
{
    m_ringWidth = width;

    update();
}

void XhRoundProgressBar::setMin(qreal min)
{
    m_min = min;

    update();
}

void XhRoundProgressBar::setMax(qreal max)
{
    m_max = max;

    update();
}

void XhRoundProgressBar::setOutsideCircleColor(const QColor &color)
{
    m_outsideCircleColor = color;

    update();
}

void XhRoundProgressBar::setInnerCircleColor(const QColor &color)
{
    m_innerCircleColor = color;

    update();
}

void XhRoundProgressBar::setValue(int value)
{
    m_value = value;

    update();
}

void XhRoundProgressBar::paintEvent(QPaintEvent *event)
{
    event->accept();
    m_rate = m_value / ((m_max - m_min) / 100);
    m_centerCoordinate.setX(width() / 2);
    m_centerCoordinate.setY(height() / 2);
    m_outsideCircleRadius = qMin(width(), height()) / 2;
    m_innerCircleRadius = (qMin(width(), height()) / 2) - m_ringWidth;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawRing(painter);
    drawText(painter);
}

void XhRoundProgressBar::drawRing(QPainter &p)
{
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(58);

    pen.setColor("#1A1A1A");
    p.setPen(pen);
    p.drawArc(pen.width(), pen.width(), width()-(pen.width()*2), height()-(pen.width()*2), 0*16, 360*16);

    pen.setColor("#F3B67B");
    p.setPen(pen);
    p.drawArc(pen.width(), pen.width(), width()-(pen.width()*2), height()-(pen.width()*2), 90*16, -m_rate*16*3.6);
}

void XhRoundProgressBar::drawText(QPainter &p)
{
    p.setPen(Qt::white);

    QFont font;
    font.setPixelSize(64);
    p.setFont(font);

    p.drawText(QRectF(0, 0, width(), height()), Qt::AlignCenter, QString("%1%").arg(m_rate));
}


