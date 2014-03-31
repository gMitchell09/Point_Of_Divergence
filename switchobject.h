/***************************************************
** Filename: switchobject.h
** Class: SwitchObject
** Created: 3/27/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#ifndef SWITCHOBJECT_H
#define SWITCHOBJECT_H

#include <QTimer>
#include <QObject>

#include "animatedcollideablesprite.h"
#include "ITriggerable.h"

class SwitchObject : public ITriggerable, public AnimatedCollideableSprite
{
    Q_OBJECT
private:
    QTimer m_toggleDelay;

public:
    SwitchObject(int width, int height, QGraphicsItem* parent = 0);

    /// setPixmaps
    /// Set on state and off state pixmaps
    void setPixmaps(QPixmap off, QPixmap on) {
        m_onPixmap = on;
        m_offPixmap = off;

        this->setPixmap(m_offPixmap);
        m_state = false;
    }

    void toggle() {
        if (!m_toggleDelay.isActive()) {
            m_toggleDelay.start(250); // Test with 1/4 second delay
            m_state = !m_state;
            if (m_state) {
                this->setPixmap(m_onPixmap);
            } else {
                this->setPixmap(m_offPixmap);
            }
            emit stateChanged(m_state);
        }
    }

//    virtual bool getState() { return m_state; }

    virtual bool isStatic() { return true; }
    virtual bool isAnimated() { return false; }
    virtual bool isCollideable() { return true; }
    virtual bool isBackground() { return false; }

    virtual ItemType blockType() { return kLever; }

    virtual QString className() { return "SwitchObject"; }

protected:
    virtual bool usesStack() { return false; }

signals:

public slots:
    virtual void controllerStateChanged(bool state) { Q_UNUSED(state); }


};

#endif // SWITCHOBJECT_H
