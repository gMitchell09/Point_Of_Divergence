/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedcollideablesprite.h"
#include "level.h"

AnimatedCollideableSprite::AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
    m_onLeftSlope = m_onRightSlope = false;
}

void AnimatedCollideableSprite::step(qint64 time, long delta) {
    AnimatedSprite::step(time, delta);

    bool timeReversed = false;

    if (delta < 0) {
        //time = time;
        timeReversed = true;
        this->setVelocity(QPointF(0, 0));
    }

    if (timeReversed && this->usesStack()) {
    }

    else {
    }
}

void AnimatedCollideableSprite::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Override this method in your subclasses if you want to be alerted when collisions occur.
    Q_UNUSED(collisions);
    Q_UNUSED(side);
}
