#ifndef ENEMY2_H
#define ENEMY2_H

#include <QGraphicsItem>
#include <QPixmap>
#include "tilemap.h"
#include "animatedcollideablesprite.h"

class Enemy2 : public AnimatedCollideableSprite
{
private:
    enum MovementState {Stand = 0, Squish};
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;
    long m_squishCtr;

    MovementState m_currentState;

public:
    Enemy2(int width, int height, QString path, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual QString className() { return "Enemy2"; }

    virtual Side damagesChar() { return (m_currentState == Stand) ? ((Side)(Left|Right|Top)) : (Side)0; }

protected:
    virtual bool usesStack() { return true; }
};

#endif // Enemy2_H