#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>

#include "sprite.h"
#include "animatedcollideablesprite.h"
#include "tilemap.h"

class Tile : public AnimatedCollideableSprite
{
public:
    explicit Tile(QGraphicsItem *parent = 0);

    virtual bool isStatic() { return true; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual QString className() { return "Tile"; }

};

#endif // TILE_H
