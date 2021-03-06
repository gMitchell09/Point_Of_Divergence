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
    explicit Door(int width, int height, b2Body* body = 0, QGraphicsItem* parent = 0);

    /// setPixmaps
    /// Set on state and off state pixmaps
    void setPixmaps(QPixmap off, QPixmap on) {
        m_onPixmap = on;
        m_offPixmap = off;

        this->setPixmap(m_offPixmap);
        m_state = false;
    }

    virtual bool isController() { return false; }
    virtual bool isReceiver() { return true; }

    virtual bool isStatic() { return true; }
    virtual bool isAnimated() { return false; }
    virtual bool isCollideable() { return !m_state; }
    virtual bool isBackground() { return false; }

    virtual ItemType blockType() { return kDoor; }

    virtual QString className() { return "DoorObject"; }
    
signals:
    
public slots:
    virtual void controllerStateChanged(bool state) {
        Q_UNUSED(state);
        m_state = !m_state;
        if (m_state) {
            this->setPixmap(m_onPixmap);
            if (m_body) m_body->SetActive(false);
        } else {
            this->setPixmap(m_offPixmap);
            if (m_body) m_body->SetActive(true);
        }
    }

private:
    QPixmap m_offPixmap;
    QPixmap m_onPixmap;
    bool m_state;
};

#endif // DOOR_H
