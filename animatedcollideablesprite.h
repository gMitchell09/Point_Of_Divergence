#ifndef ANIMATEDCOLLIDEABLESPRITE_H
#define ANIMATEDCOLLIDEABLESPRITE_H

/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedsprite.h"
#include <QGraphicsScene>
#include <QDebug>
#include <stack>

enum Side { Top = 1, Right = 2, Bottom = 4, Left = 8 };

struct PositionState { QPointF pos; };

struct Collision {
    Sprite *firstSprite, *secondSprite;
    QPointF normalVector; // W.R.T. first!!!
    Side firstSide, secondSide;
    QPointF overlapDistance;
};

class AnimatedCollideableSprite : public AnimatedSprite
{
private:
    QPoint m_collisionPoints[4][2];
    std::stack<PositionState> m_positionStateStack;
    bool m_solid;

    unsigned char checkForCollision(QList<Collision> &collisions, QPointF offset, bool timeReversed);
    void resolveCollision(Collision collision);

public:
    explicit AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent = 0);

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }

    ///
    /// \brief setSolid
    /// \param solid is true if this object shouldn't be able to penetrate other solid objects
    ///
    void setSolid(bool solid) { m_solid = solid; }
    bool isSolid() { return m_solid; }

    ///
    /// \brief setBrake
    /// \param brake determines if sprite is slowing to a stop
    ///


    virtual void step(long time);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

protected:
    virtual bool usesStack() { return false; }
};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
