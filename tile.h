#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "animatedcollideablesprite.h"
#include "tilemap.h"

struct TileProperties {
    bool solid;
    ItemType kind;
    bool placeholderTile;
    bool fixedRotation;
    int rate;
    int life;

    TileProperties() {
        solid = true;
        kind = ItemType::kBlock;
        placeholderTile = false;
        fixedRotation = 0;
        rate = 0;
        life = 0;
    }
};

class Tile : public AnimatedCollideableSprite
{
private:
    ItemType m_kind { kBlock };
    bool m_isStatic { true };
    bool m_usesStack { false };

public:
    explicit Tile(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual bool isStatic() { return m_isStatic; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual void collisionOccurred(Sprite* other, Side side);

    virtual QString className() { return "Tile"; }

    virtual ItemType blockType() { return m_kind; }

    void setBlockType(ItemType kind) {
        m_kind = kind;
        if (m_kind == kBox) {
            m_isStatic = false;
            m_usesStack = true;
            this->setSolid(true);
            this->setZValue(0);
        }
    }

    virtual bool usesStack() { return m_usesStack; }

    virtual void step(qint64 time, long delta);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

//    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
//        AnimatedCollideableSprite::paint(painter, option, widget);
//    }


};

#endif // TILE_H
