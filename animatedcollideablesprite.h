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

struct PositionState { QPointF pos; qint64 timestamp; };

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

    unsigned char checkForCollision(QList<Collision> &collisions, QPointF offset, bool timeReversed);
    void resolveCollision(Collision collision);
    Sprite* spriteWithinWhisker(QRectF whisker, Side side = Top);

    QGraphicsRectItem *whiskers[8];
    QRectF topWhiskerLeft, topWhiskerRight, rightWhiskerTop, rightWhiskerBottom, bottomWhiskerRight, bottomWhiskerLeft, leftWhiskerBottom, leftWhiskerTop;
    void updateWhiskers(QPointF offset);

public:
    explicit AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent = 0);

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }

    ///
    /// \brief setBrake
    /// \param brake determines if sprite is slowing to a stop
    ///


    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual QString className() { return "AnimatedCollideableSprite"; }

protected:
    virtual bool usesStack() { return false; }
};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
