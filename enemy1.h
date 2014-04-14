#ifndef ENEMY1_H
#define ENEMY1_H

#include <QGraphicsItem>
#include <QPixmap>
#include "tilemap.h"
#include "animatedcollideablesprite.h"

#include "Box2D.h"

class Enemy1 : public AnimatedCollideableSprite
{
private:
    enum MovementState {Stand = 0, Squish};
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;
    long m_squishCtr;

    MovementState m_currentState;

public:
    Enemy1(int width, int height, QString path, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);
    virtual void collisionsOccurred(QList<Collision> &collisions, unsigned char side);
    virtual void collisionOccurred(Sprite *other, Side side);

    virtual QString className() { return "Enemy1"; }

    virtual Side damagesChar() { return (m_currentState == Stand) ? ((Side)(Left|Right|Top)) : (Side)0; }

protected:
    virtual bool usesStack() { return true; }
};

#endif // ENEMY1_H
