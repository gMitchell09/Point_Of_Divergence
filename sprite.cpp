/* Author: George Mitchell
 * Use Case: *
 */

#include "sprite.h"

Sprite::Sprite(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent) {
    this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    m_velocity = QPointF();
    m_apparentVelocity = QPointF();
    m_acceleration = QPointF();
}
