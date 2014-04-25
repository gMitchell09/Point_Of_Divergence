#ifndef BOXBUTTON_H
#define BOXBUTTON_H

#include <QObject>
#include "animatedcollideablesprite.h"
#include "ITriggerable.h"
#include "sprite.h"
#include <Box2D.h>

class BoxButton : public ITriggerable, public AnimatedCollideableSprite
{
    Q_OBJECT
public:
    BoxButton(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual bool isController() { return true; }
    virtual bool isReceiver() { return false; }

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

    virtual ItemType blockType() { return kBoxButton; }

    virtual QString className() { return "BoxButton"; }

    virtual void pushState(qint64 time, long delta, State& state) {
        state.triggerableState = m_state;

        Sprite::pushState(time, delta, state);
    }

    virtual void setState(State currentState) {
        Sprite::setState(currentState);

        if (m_state == currentState.triggerableState) return;
        m_state = currentState.triggerableState;

        if (m_state) {
            this->setPixmap(m_onPixmap);
        } else {
            this->setPixmap(m_offPixmap);
        }

        emit stateChanged(m_state);
    }

    virtual void collisionOccurred(AnimatedCollideableSprite* other, Side side);

signals:

public slots:
    virtual void controllerStateChanged(bool state) { Q_UNUSED(state); }

protected:
    virtual bool usesStack() { return true; }

private:
    QPixmap m_offPixmap;
    QPixmap m_onPixmap;
    bool m_state;
};

#endif // BOXBUTTON_H
