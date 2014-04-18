#include "projectile.h"
#include "gameengine.h"

Projectile::Projectile(int width, int height, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent) {
    this->setSolid();
}

void Projectile::collisionOccurred(Sprite *sprite, Side side) {
    qDebug() << "Destroying Projectile";
    if (sprite->isSolid()) {
        ((GameEngine*)this->scene())->removeItem(this);
    }
}
