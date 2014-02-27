#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <functional>

#include "gameengine.h"
#include "sprite.h"

class GameEngine;
class MenuButton : public Sprite
{
private:
    bool m_pressed;
    QPixmap *m_upGraphic, *m_downGraphic, *m_highlightGraphic;

    GameEngine* m_clickedCallback;

public:
    explicit MenuButton(QPixmap *up, QPixmap *down, QGraphicsItem *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);

    inline void setCallback(GameEngine* callback) {
        m_clickedCallback = callback;
    }

    virtual void clicked();
    virtual ~MenuButton();
};

#endif // MENUBUTTON_H
