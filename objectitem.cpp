#include "objectitem.h"
#include "gameengine.h"
#include <QDebug>

ObjectItem::ObjectItem(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent) {

    TileMap itemTiles = TileMap(16, 16, 1, 1, ":Items/Items.png");
    std::vector<QPixmap> coinAnimation;
    coinAnimation.push_back(itemTiles.copyCellAt(5, 1));
    coinAnimation.push_back(itemTiles.copyCellAt(5, 1).transformed(QTransform::fromScale(-1, 1)));
    this->addAnimation(coinAnimation, Loop);
    this->triggerAnimation(0);
}

void ObjectItem::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    AnimatedCollideableSprite::collisionOccurred(collisions, side);


}
