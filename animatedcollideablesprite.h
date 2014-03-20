#ifndef ANIMATEDCOLLIDEABLESPRITE_H
#define ANIMATEDCOLLIDEABLESPRITE_H

/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedsprite.h"
#include "networkmanager.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>
#include <QPainter>

#include <stack>

struct Collision {
    Sprite *firstSprite, *secondSprite;
    QPointF normalVector; // W.R.T. first!!!
    Side firstSide, secondSide;
    QPointF overlapDistance;
};

class AnimatedCollideableSprite : public AnimatedSprite
{
private:
    unsigned char checkForCollision(QList<Collision> &collisions, QPointF offset);
    void resolveCollision(Collision collision);
    bool spriteWithinWhisker(QPolygonF whisker, QList<Sprite *> &collisions);

    QPolygonF topWhiskerLeft, topWhiskerRight, rightWhiskerTop, rightWhiskerBottom, bottomWhiskerRight, bottomWhiskerLeft, leftWhiskerBottom, leftWhiskerTop;
    void updateWhiskers(QPointF offset);

    bool m_onLeftSlope, m_onRightSlope;

public:
    explicit AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent = 0);

    ///

    /// \brief setBrake
    /// \param brake determines if sprite is slowing to a stop
    ///


    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);


    bool isOnLeftSlope() { return m_onLeftSlope; }
    bool isOnRightSlope() { return m_onRightSlope; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void decodeDatagram(NetworkManager::DatagramFormat dg) {
        AnimatedSprite::decodeDatagram(dg);
    }

    virtual void pushState(qint64 time, long delta, State& state) {
        state.pos = this->pos();
        AnimatedSprite::pushState(time, delta, state);
    }

    virtual void setState(State currentState) {
        AnimatedSprite::setState(currentState);
        this->setPos(currentState.pos);
    }

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }
    virtual QString className() { return "AnimatedCollideableSprite"; }

protected:
    QPointF m_collisionPoints[4][2];
    virtual bool usesStack() { return false; }
};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
