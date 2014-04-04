/* Author: George Mitchell
 * Use Case: Interact With Solid Platform
 */

#include "staticplatform.h"

StaticPlatform::StaticPlatform(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
    this->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

void StaticPlatform::step(qint64 time, long delta) {
    AnimatedSprite::step(time, delta);
}
