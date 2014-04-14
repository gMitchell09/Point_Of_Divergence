#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "animatedcollideablesprite.h"
#include <Box2D.h>

class Projectile : public AnimatedCollideableSprite
{
public:
    Projectile(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);

signals:

public slots:

};

#endif // PROJECTILE_H
