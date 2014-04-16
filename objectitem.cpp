#include "objectitem.h"
#include "gameengine.h"
#include <QDebug>

ObjectItem::ObjectItem(int width, int height, QString path, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent) {

    TileMap itemTiles = TileMap(16, 16, 1, 1, path + "/Items.png");
    std::vector<QPixmap> coinAnimation;
    coinAnimation.push_back(itemTiles.copyCellAt(5, 1));
    coinAnimation.push_back(itemTiles.copyCellAt(5, 1).transformed(QTransform::fromScale(-1, 1)));
    this->addAnimation(coinAnimation, Loop);
    this->triggerAnimation(0);
}

void ObjectItem::collisionOccurred(Sprite* other, Side side) {
    AnimatedCollideableSprite::collisionOccurred(other, side);

}
