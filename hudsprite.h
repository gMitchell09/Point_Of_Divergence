#ifndef HUDSPRITE_H
#define HUDSPRITE_H

/* Author: Jesse Hairston
 * Added: Friday Feb 7 @ 4:25pm
 * Initial Commit: Wednesday Feb 7 @ 4:25pm
 * Use Case: 09: Heads-Up Display
 */

#include <QGraphicsItem>
#include <QGraphicsView>

#include "sprite.h"

class HUDsprite : public Sprite
{
private:
    const QPointF m_screenPos;
    QGraphicsView *&m_view;

public:
    explicit HUDsprite(QGraphicsView *&view, QPointF screenPos, QGraphicsItem *parent = 0);

    bool isStatic() { return true; }
    bool isAnimated() { return false; }
    bool isCollideable() { return false; }
    bool isBackground() { return false; }

    virtual void step (long time);

};

#endif // HUDSPRITE_H
