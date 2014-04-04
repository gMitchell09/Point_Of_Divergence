#ifndef HUD_H
#define HUD_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>


class HUD : public QGraphicsItemGroup
{
private:
    const QPointF m_screenPos;
    QGraphicsView * const &m_view;

public:
    HUD(QGraphicsView * const&view, QPointF screenPos, QGraphicsItem *parent = 0);
    virtual void step (qint64 time, long delta);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
};

#endif // HUD_H
