#ifndef OPTIONBUTTON_H
#define OPTIONBUTTON_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <functional>

#include "gameengine.h"
#include "sprite.h"

class GameEngine;

class IMenuItem {
public:
    virtual ~IMenuItem() {}
    virtual void setListener(void* listener) = 0;
    virtual void saveValue() = 0;
};

class OptionButton : public Sprite, public IMenuItem
{
private:
    bool m_pressed;
    bool m_state;

    bool *m_listener;
    QPixmap *m_upGraphic, *m_downGraphic;

    std::function<void(void)> m_clickedCallback;

public:
    explicit OptionButton(QPixmap *up, QPixmap *down, QGraphicsItem *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);

    bool optionState();

    inline void setCallback(std::function<void(void)> callback) {
        m_clickedCallback = callback;
    }

    void setListener(void* listener) {
        m_listener = (bool*) listener;
    }

    void saveValue() {
        if (m_listener) *m_listener = !m_state;
    }

    virtual void clicked();
    virtual ~OptionButton();

};

#endif // OPTIONBUTTON_H
