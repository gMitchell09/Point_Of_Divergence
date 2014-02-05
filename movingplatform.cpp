#include "movingplatform.h"

MovingPlatform::MovingPlatform(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
}

void MovingPlatform::step(unsigned long time) {
    AnimatedSprite::step(time);

    double timeStep = time / 1000.0;

    QPointF oldVel = m_velocity;

    m_velocity += m_acceleration * timeStep;
    this->setPos(this->pos() + (m_velocity + oldVel) * 0.5 * timeStep);
}
