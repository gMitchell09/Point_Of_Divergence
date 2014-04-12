#ifndef ENEMYBOSS_H
#define ENEMYBOSS_H

#include <QGraphicsItem>
#include <QPixmap>
#include "tilemap.h"
#include "animatedcollideablesprite.h"

class EnemyBoss : public AnimatedCollideableSprite
{
private:
    enum MovementState {Stand = 0, Squish};
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;
    long m_squishCtr;
    int m_enemy_health_count;

    MovementState m_currentState;

public:
    EnemyBoss(int width, int height, QString path, int enemyHits, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual QString className() { return "EnemyBoss"; }

    virtual Side damagesChar() { return (m_currentState == Stand) ? ((Side)(Left|Right|Top)) : (Side)0; }

protected:
    virtual bool usesStack() { return true; }
};

#endif // ENEMYBOSS_H
