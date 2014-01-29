#ifndef STATICBACKGROUND_H
#define STATICBACKGROUND_H

#include "sprite.h"

class StaticBackground : public Sprite
{
private:
    QPoint m_repeatDistance;
public:
    explicit StaticBackground(QPoint repeatDistance, QGraphicsItem* parent = 0);

    // Overrides
    virtual bool isStatic();
    virtual bool isAnimated();
    virtual bool isCollideable();
    virtual bool isBackground();
};

#endif // STATICBACKGROUND_H
