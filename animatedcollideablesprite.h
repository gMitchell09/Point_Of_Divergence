#ifndef ANIMATEDCOLLIDEABLESPRITE_H
#define ANIMATEDCOLLIDEABLESPRITE_H

/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedsprite.h"
#include <QGraphicsScene>
#include <QDebug>

enum Side { Top = 1, Right = 2, Bottom = 4, Left = 8 };

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
    bool m_solid;

    unsigned char checkForCollision(QList<Collision> &collisions, QPointF offset);
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


    virtual void step(unsigned long time);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    
signals:
    
public slots:
    
};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
