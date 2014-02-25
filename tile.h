#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QGraphicsScene>

#include "sprite.h"
#include "animatedcollideablesprite.h"
#include "tilemap.h"

struct TileProperties {
    bool solid;
    ItemType kind;
    bool placeholderTile;

    TileProperties() {
        solid = true;
        kind = ItemType::kBlock;
        placeholderTile = false;
    }
};

class Tile : public AnimatedCollideableSprite
{
private:
    ItemType m_kind;
public:
    explicit Tile(int width, int height, QGraphicsItem *parent);

    virtual bool isStatic() { return true; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual QString className() { return "Tile"; }

    virtual ItemType blockType() { return m_kind; }
    void setBlockType(ItemType kind) { m_kind = kind; }

};

#endif // TILE_H
