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

class SwitchObject : public QObject, public AnimatedCollideableSprite
{
    Q_OBJECT
private:
    bool m_state;

    QPixmap m_onPixmap;
    QPixmap m_offPixmap;

    QTimer m_toggleDelay;

public:
    SwitchObject(int width, int height, QGraphicsItem* parent = 0);

    /// setPixmaps
    /// Set on state and off state pixmaps
    void setPixmaps(QPixmap on, QPixmap off) {
        m_onPixmap = on;
        m_offPixmap = off;

        this->setPixmap(m_onPixmap);
        m_state = false;
    }


    void toggle() {
        if (!m_toggleDelay.isActive()) {
            m_toggleDelay.setSingleShot(true);
            m_toggleDelay.start(250); // Test with 1/4 second delay
            m_state = !m_state;
            if (m_state)
                this->setPixmap(m_onPixmap);
            else
                this->setPixmap(m_offPixmap);

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
    void stateChanged(bool m_state);
};

#endif // SWITCHOBJECT_H
