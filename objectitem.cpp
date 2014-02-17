#include "objectitem.h"
#include "gameengine.h"
#include <QDebug>

ObjectItem::ObjectItem(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent) {

    QPixmap myPix1(":Simple_Sprite/1.png");
    QPixmap myPix2(":Simple_Sprite/2.png");
    QPixmap myPix3(":Simple_Sprite/3.png");
    QPixmap myPix4(":Simple_Sprite/4.png");
    QPixmap myPix5(":Simple_Sprite/5.png");

    std::vector<QPixmap> pixmapList2;
    pixmapList2.push_back(myPix1);
    pixmapList2.push_back(myPix2);
    pixmapList2.push_back(myPix3);
    pixmapList2.push_back(myPix4);
    pixmapList2.push_back(myPix5);

//    testSprite2 = new StaticPlatform(48, 64);
//    testSprite2->addAnimation(pixmapList2, Loop);
//    testSprite2->setPos(gameEngine->sceneRect().width()/2 - 48, 1020);
//    testSprite2->setVelocity(QPointF(0, 0));
//    testSprite2->setAcceleration(QPointF(0, 0));
//    testSprite2->triggerAnimation(0);

    this->addAnimation(pixmapList2, Loop);
    this->setSolid(false);

    this->setVelocity(QPointF(0, 0));
    this->setAcceleration(QPointF(0, 0));
    this->triggerAnimation(0);
}

void ObjectItem::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    AnimatedCollideableSprite::collisionOccurred(collisions, side);
    ((GameEngine*)this->scene())->removeItem(this);
}
