#ifndef TELEPORTATIONPAD_H
#define TELEPORTATIONPAD_H

#include "animatedcollideablesprite.h"
#include "ITriggerable.h"

#include <QObject>
#include <Box2D.h>

class TeleportationPad : public ITriggerable, public AnimatedCollideableSprite
{
    Q_OBJECT
public:
    TeleportationPad(int width, int height, b2Body* body, QGraphicsItem *parent = 0);

    virtual bool isController() { return false; }
    virtual bool isReceiver() { return false; }
};

#endif // TELEPORTATIONPAD_H
