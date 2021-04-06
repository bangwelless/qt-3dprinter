#ifndef XHROUNDPROGRESSBAR_H
#define XHROUNDPROGRESSBAR_H

#include <QWidget>
#include <QPointF>
#include <QColor>

class QPainter;

class XhRoundProgressBar : public QWidget
{
    Q_OBJECT

public:
    XhRoundProgressBar(QWidget *parent = Q_NULLPTR);
    ~XhRoundProgressBar();

    void                    setRingWidth(qreal width);
    void                    setMin(qreal min);
    void                    setMax(qreal max);
    void                    setOutsideCircleColor(const QColor &color);
    void                    setInnerCircleColor(const QColor &color);

public slots:
    void                    setValue(int value);

protected:
    virtual void            paintEvent(QPaintEvent *event) override;

private:
    qreal       m_min{ 0 }, m_value{ 0 }, m_max{ 100 };
    qreal       m_ringWidth{ 1 };

    int         m_rate{ 0 };
    QPointF     m_centerCoordinate;

    QColor      m_outsideCircleColor;
    qreal       m_outsideCircleRadius{ 0 };

    QColor      m_innerCircleColor;
    qreal       m_innerCircleRadius{ 0 };

private:
    void                    drawRing(QPainter &p);
    void                    drawText(QPainter &p);
};

#endif // XHROUNDPROGRESSBAR_H
