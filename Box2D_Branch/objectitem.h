#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QGraphicsItem>
#include <QPixmap>

#include "animatedcollideablesprite.h"
#include "gameengine.h"

#include "Box2D.h"

class ObjectItem : public AnimatedCollideableSprite
{
public:
    ObjectItem(int width, int height, QString path, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual void collisionsOccurred(QList<Collision> &collisions, unsigned char side);

    virtual bool isStatic() { return true; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual QString className() { return "ObjectItem"; }

    virtual ItemType blockType() { return ItemType::kCoin; }

    // In here we want to give the subclasses the ability to "flip" switches, "press" buttons, ...
    virtual void triggerEvent() {}


protected:
    virtual bool usesStack() { return true; }

};

#endif // OBJECTITEM_H
