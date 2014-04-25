#include "tile.h"

#include <QtMath>

Tile::Tile(int width, int height, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent)
{
    this->setZValue(-100);
    this->setTransformOriginPoint(this->boundingRect().width()/2, this->boundingRect().height()/2);
}

void Tile::step(qint64 time, long delta) {
    if (this->blockType() == kBox) {
        AnimatedCollideableSprite::step(time, delta);
    }
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
    qDebug() << "Tile: " << this->blockType();
    if (this->m_body) {
        Sprite* sprite = (Sprite*)m_body->GetUserData();
        qDebug() << "Body: " << sprite;
    }
}

void Tile::collisionOccurred(AnimatedCollideableSprite* other, Side side) {
    AnimatedCollideableSprite::collisionOccurred(other, side);
}
