/* Author: George Mitchell
 * Use Case: *
 */

#include "sprite.h"

Sprite::Sprite(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent) {
    this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}
