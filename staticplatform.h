#ifndef STATICPLATFORM_H
#define STATICPLATFORM_H

#include <QGraphicsItem>

#include "sprite.h"

class StaticPlatform : public Sprite
{

public:
    explicit StaticPlatform(QGraphicsItem *parent = 0);

    bool isStatic() { return true; }
    bool isAnimated() { return false; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }
    
};

#endif // STATICPLATFORM_H
