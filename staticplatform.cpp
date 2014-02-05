#include "staticplatform.h"

StaticPlatform::StaticPlatform(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
    this->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

void StaticPlatform::step(unsigned long time) {
    AnimatedSprite::step(time);
}
