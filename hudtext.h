#ifndef HUDOBJECT_H
#define HUDOBJECT_H

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsView>

// Abstract
class HUDText : public QGraphicsSimpleTextItem
{
private:
    const QPointF m_screenPos;
    QGraphicsView *&m_view;

public:
    explicit HUDText(QGraphicsView *&view, QPointF screenPos, QGraphicsItem *parent = 0);

    virtual void step (qint64 time, long delta);
};

#endif // HUDOBJECT_H
