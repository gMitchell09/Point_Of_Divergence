/***************************************************
** Filename: door.h
** Class: Door
** Created: 3/31/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#ifndef DOOR_H
#define DOOR_H

#include <QObject>
#include <QGraphicsItem>

#include "animatedcollideablesprite.h"
#include "ITriggerable.h"

class Door : public ITriggerable, public AnimatedCollideableSprite
{
    Q_OBJECT
public:
    explicit Door(int width, int height, QGraphicsItem* parent = 0);

    /// setPixmaps
    /// Set on state and off state pixmaps
    void setPixmaps(QPixmap off, QPixmap on) {
        m_onPixmap = on;
        m_offPixmap = off;

        this->setPixmap(m_offPixmap);
        m_state = false;
    }

    virtual bool isStatic() { return true; }
    virtual bool isAnimated() { return false; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual ItemType blockType() { return kDoor; }

    virtual QString className() { return "DoorObject"; }
    
signals:
    
public slots:
    virtual void controllerStateChanged(bool state) {
        m_state = state;
        if (m_state) {
            this->setPixmap(m_onPixmap);
        } else {
            this->setPixmap(m_offPixmap);
        }
    }
};

#endif // DOOR_H
