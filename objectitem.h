#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QGraphicsItem>
#include <QPixmap>

#include "animatedcollideablesprite.h"
#include "gameengine.h"

class ObjectItem : public AnimatedCollideableSprite
{
public:
    ObjectItem(int width, int height, QGraphicsItem *parent = 0);

    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    bool isStatic() { return true; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }

    virtual QString className() { return "ObjectItem"; }


protected:
    virtual bool usesStack() { return true; }

};

#endif // OBJECTITEM_H
