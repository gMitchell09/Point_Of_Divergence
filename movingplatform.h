#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

/* Author: George Mitchell
 * Added: Wednesday Feb 5 @ 10:53am
 * Initial Commit: Wednesday Feb 5 @ 11:23am
 * Use Case: 05: Interact With Solid Platform
 */

#include <QGraphicsItem>

#include "animatedsprite.h"

class MovingPlatform : public AnimatedSprite
{
public:
    explicit MovingPlatform(int width, int height, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);

    virtual bool isStatic() { return false; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

protected:
    virtual bool usesStack() { return true; }

};

#endif // MOVINGPLATFORM_H
