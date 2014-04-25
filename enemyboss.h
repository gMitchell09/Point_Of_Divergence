#ifndef ENEMYBOSS_H
#define ENEMYBOSS_H

#include <QGraphicsItem>
#include <QPixmap>
#include "tilemap.h"
#include "animatedcollideablesprite.h"

#include "Box2D.h"

class EnemyBoss : public AnimatedCollideableSprite
{
private:
    enum MovementState {Stand = 0, Squish};
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;
    long m_squishCtr;
    int m_enemy_health_count { 3 };


    const float32 m_maxSpeed { 4 };
    bool m_shuffleRight { true };

    MovementState m_currentState;

public:
    EnemyBoss(int width, int height, QString path, int life, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(AnimatedCollideableSprite *other, Side side);

    virtual QString className() { return "EnemyBoss"; }

    virtual Side damagesChar() { return (m_currentState == Stand) ? ((Side)(Left|Right|Top)) : (Side)0; }

protected:
    virtual bool usesStack() { return true; }
};

#endif // ENEMY1_H
