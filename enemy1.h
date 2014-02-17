#ifndef ENEMY1_H
#define ENEMY1_H

#include <QGraphicsItem>
#include <QPixmap>
#include "tilemap.h"
#include "animatedcollideablesprite.h"

namespace Goomba {
    enum MovementState {Stand = 0, Squish};
}

class Enemy1 : public AnimatedCollideableSprite
{
private:
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;
    long m_squishCtr = 0;
    Goomba::MovementState m_currentState;
public:
    explicit Enemy1(int width, int height, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual QString className() { return "Enemy1"; }

protected:
    virtual bool usesStack() { return true; }
};

#endif // ENEMY1_H
