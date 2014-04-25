#ifndef TELEPORTATIONPAD_H
#define TELEPORTATIONPAD_H

#include "animatedcollideablesprite.h"
#include "ITriggerable.h"
#include "maincharacter.h"

#include <QObject>
#include <Box2D.h>

class MainCharacter;
class TeleportationPad : public ITriggerable, public AnimatedCollideableSprite
{
    Q_OBJECT
public:
    TeleportationPad(int width, int height, bool controller, b2Body* body, QGraphicsItem *parent = 0);

    virtual bool isController() { return m_sender; }
    virtual bool isReceiver() { return !m_sender; }

    virtual bool isStatic() { return true; }
    virtual bool isAnimated() { return false; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual ItemType blockType() { return kTeleStation; }

    virtual QString className() { return "TeleportationPad"; }

    virtual void collisionOccurred(AnimatedCollideableSprite* other, Side side);

    MainCharacter* mc;

public slots:
    virtual void controllerStateChanged(bool state);

private:
    bool m_sender;

};

#endif // TELEPORTATIONPAD_H
