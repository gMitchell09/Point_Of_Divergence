#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include <QGraphicsItem>

#include "animatedsprite.h"

class MovingPlatform : public AnimatedSprite
{
public:
    explicit MovingPlatform(int width, int height, QGraphicsItem *parent = 0);

    virtual void step(unsigned long time);

    virtual bool isStatic() { return false; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

};

#endif // MOVINGPLATFORM_H
