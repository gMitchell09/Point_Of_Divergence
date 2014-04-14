#include "turret.h"
#include "animatedcollideablesprite.h"

Turret::Turret(int width, int height, bool facesLeft, int rate, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent),
    m_facesLeft(facesLeft) {
    m_fireDelay = rate;
}

void Turret::fireProjectile() {

}

void Turret::step(qint64 time, long delta) {
    m_lastFireTime += delta;
    if (m_fireDelay >= m_lastFireTime) {
        m_lastFireTime = 0;
        this->fireProjectile();
    }
}
