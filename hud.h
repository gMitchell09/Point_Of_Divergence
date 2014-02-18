#ifndef HUD_H
#define HUD_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>

class HUD : public QGraphicsItemGroup
{
private:
    const QPointF m_screenPos;
    QGraphicsView *&m_view;

public:
    HUD(QGraphicsView *&view, QPointF screenPos, QGraphicsItem *parent = 0);
    virtual void step (qint64 time, long delta);
};

#endif // HUD_H
