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
    QPointF m_collisionPoints[4][2];
    std::stack<PositionState> m_positionStateStack;

    unsigned char checkForCollision(QList<Collision> &collisions, QPointF offset);
    void resolveCollision(Collision collision);
    bool spriteWithinWhisker(QPolygonF whisker, QList<Sprite *> &collisions);

    QPolygonF topWhiskerLeft, topWhiskerRight, rightWhiskerTop, rightWhiskerBottom, bottomWhiskerRight, bottomWhiskerLeft, leftWhiskerBottom, leftWhiskerTop;
    void updateWhiskers(QPointF offset);

    bool m_onLeftSlope, m_onRightSlope;

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

    bool isOnLeftSlope() { return m_onLeftSlope; }
    bool isOnRightSlope() { return m_onRightSlope; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


protected:
    virtual bool usesStack() { return false; }
};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
