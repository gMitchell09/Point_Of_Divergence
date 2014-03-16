#include "tile.h"

#include <QtMath>

Tile::Tile(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent)
{
    this->setVelocity(QPointF(0, 0));
    this->setAcceleration(QPointF(0, 0));
    m_isStatic = true;
    m_usesStack = false;
    this->setZValue(-100);
    this->setTransformOriginPoint(this->boundingRect().width()/2, this->boundingRect().height()/2);
}

void Tile::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    if (this->blockType() == kBox) {
        if (this->isOnLeftSlope() || this->isOnRightSlope()) {
            for (auto itr = collisions.begin(); itr != collisions.end(); ++itr) {
                if (((Collision)(*itr)).firstSide == Bottom) {
                    Sprite* spr = dynamic_cast<Sprite*>(((Collision)(*itr)).secondSprite);
                    switch (spr->blockType()) {
                        case kSlope30Left:
                            this->setRotation(30);
                            break;
                        case kSlope30Right:
                            this->setRotation(-30);
                            break;
                        case kSlope60Left:
                            this->setRotation(30);
                            break;
                        case kSlope60Right:
                            this->setRotation(-30);
                            break;
                        case kSlope45Left:
                            this->setRotation(45);
                            break;
                        case kSlope45Right:
                            this->setRotation(-45);
                            break;
                    }
                }
            }
        } else {
            this->setRotation(0);
        }
    }
}

void Tile::step(qint64 time, long delta) {
    if (this->blockType() == kBox) {
        AnimatedCollideableSprite::step(time, delta);
        this->getVelocity().setX(this->getVelocity().x() * 0.99);
        if (fabs(this->getVelocity().x()) < 0.1) this->setVelocity(QPointF(0, this->getVelocity().y()));
    }
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "Tile: " << this->blockType();
}
