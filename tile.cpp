#include "tile.h"

Tile::Tile(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent)
{
    this->setVelocity(QPointF(0, 0));
    this->setAcceleration(QPointF(0, 0));
}

void Tile::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Hold
}

void Tile::step(qint64 time, long delta) {
    if (this->blockType() == kBox) {
        AnimatedCollideableSprite::step(time, delta);
    }
}
