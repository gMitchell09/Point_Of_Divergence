#include "tile.h"

#include <QtMath>

Tile::Tile(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent)
{
    this->setVelocity(QPointF(0, 0));
    this->setAcceleration(QPointF(0, 0));
    m_isStatic = true;
}

void Tile::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
}

void Tile::step(qint64 time, long delta) {
    if (this->blockType() == kBox) {
        AnimatedCollideableSprite::step(time, delta);
        this->getVelocity().setX(this->getVelocity().x() * 0.99);
        if (fabs(this->getVelocity().x()) < 0.1) this->setVelocity(QPointF(0, this->getVelocity().y()));
    }
}
