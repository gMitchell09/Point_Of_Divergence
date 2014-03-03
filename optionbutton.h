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
class OptionButton : public Sprite
{
private:
    bool m_pressed;
    bool m_state;
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

    virtual void clicked();
    virtual ~OptionButton();

};

#endif // OPTIONBUTTON_H
