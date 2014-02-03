#include "staticplatform.h"

StaticPlatform::StaticPlatform(QGraphicsItem *parent) :
    Sprite(parent) {
    this->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}
