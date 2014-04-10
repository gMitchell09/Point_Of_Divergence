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

    void updatePosition();

public:
    explicit AnimatedCollideableSprite(int width, int height, b2Body* body, QGraphicsItem *parent);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    bool isOnLeftSlope() { return m_onLeftSlope; }
    bool isOnRightSlope() { return m_onRightSlope; }

    virtual void decodeDatagram(NetworkManager::DatagramFormat dg) {
        AnimatedSprite::decodeDatagram(dg);
    }

    virtual void pushState(qint64 time, long delta, State& state) {
        if (m_body) {
            state.pos = m_body->GetPosition();
            state.vel = m_body->GetLinearVelocity();
            state.angle = m_body->GetAngle();
        }

        AnimatedSprite::pushState(time, delta, state);
    }

    virtual void setState(State currentState) {
        AnimatedSprite::setState(currentState);
        if (m_body) {
            m_body->SetTransform(currentState.pos, currentState.angle);
            m_body->SetLinearVelocity(currentState.vel);
            this->updatePosition();
        }
    }

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }
    virtual QString className() { return "AnimatedCollideableSprite"; }
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    virtual bool usesStack() { return false; }

    b2Body* m_body;

};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
