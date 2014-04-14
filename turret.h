#ifndef TURRET_H
#define TURRET_H

#include "animatedcollideablesprite.h"

class Turret : public AnimatedCollideableSprite {

public:
    Turret(int width, int height, bool facesLeft, int rate, b2Body* body = 0, QGraphicsItem *parent = 0);

    void fireProjectile();

    virtual void step(qint64 time, long delta);

signals:

public slots:

private:
    qreal m_fireDelay;
    qreal m_lastFireTime;
    bool m_facesLeft;
};

#endif // TURRET_H
