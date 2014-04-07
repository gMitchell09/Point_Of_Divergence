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
    bool m_isStatic;
    bool m_usesStack;

    qreal m_rotation;
public:
    explicit Tile(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual bool isStatic() { return m_isStatic; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual QString className() { return "Tile"; }

    virtual ItemType blockType() { return m_kind; }
    void setBlockType(ItemType kind) {
        m_kind = kind;
        if (m_kind == kBox) {
            this->setAcceleration(QPointF(0, 2000));
            m_isStatic = false;
            m_usesStack = true;
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
