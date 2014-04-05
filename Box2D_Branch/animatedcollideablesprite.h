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

#include "Box2D.h"

struct Collision {
    Sprite *firstSprite, *secondSprite;
    QPointF normalVector; // W.R.T. first!!!
    Side firstSide, secondSide;
    QPointF overlapDistance;
};

class AnimatedCollideableSprite : public AnimatedSprite
{
private:
    bool m_onLeftSlope, m_onRightSlope;

public:
    explicit AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    bool isOnLeftSlope() { return m_onLeftSlope; }
    bool isOnRightSlope() { return m_onRightSlope; }

    virtual void decodeDatagram(NetworkManager::DatagramFormat dg) {
        AnimatedSprite::decodeDatagram(dg);
    }

    virtual void pushState(qint64 time, long delta, State& state) {
        state.pos = this->pos();
        state.vel = this->getApparentVelocity();
        AnimatedSprite::pushState(time, delta, state);
    }

    virtual void setState(State currentState) {
        AnimatedSprite::setState(currentState);
        this->setPos(currentState.pos);
        this->setApparentVelocity(currentState.vel);
    }

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }
    virtual QString className() { return "AnimatedCollideableSprite"; }

protected:
    virtual bool usesStack() { return false; }
};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
