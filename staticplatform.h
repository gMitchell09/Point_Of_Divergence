#ifndef STATICPLATFORM_H
#define STATICPLATFORM_H

/* Author: George Mitchell
 * Use Case: Interact With Solid Platform
 */

#include <QGraphicsItem>

#include "animatedsprite.h"

class StaticPlatform : public AnimatedSprite
{

public:
    explicit StaticPlatform(int width, int height, QGraphicsItem *parent = 0);

    virtual void step(long time);

    bool isStatic() { return true; }
    bool isAnimated() { return false; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }
    bool isSolid() { return true; }
    
};

#endif // STATICPLATFORM_H
