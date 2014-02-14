/* Author: George Mitchell
 * Added: Wednesday Feb 5 @ 10:53am
 * Initial Commit: Wednesday Feb 5 @ 11:23am
 * Use Case: 05: Interact With Solid Platform
 */

#include "movingplatform.h"
#include <QDebug>

MovingPlatform::MovingPlatform(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
}

void MovingPlatform::step(qint64 time, long delta) {
    AnimatedSprite::step(time, delta);

    double timeStep = delta / 1000.0;

    QPointF oldVel = m_velocity;

    m_velocity += m_acceleration * timeStep;
    this->setPos(this->pos() + (m_velocity + oldVel) * 0.5 * timeStep);
}
