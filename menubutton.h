#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <functional>

#include "gameengine.h"
#include "sprite.h"

class GameEngine;
class MenuButton : public Sprite
{
private:
    QPixmap *m_upGraphic, *m_downGraphic, *m_hoverGraphic;

    std::function<void(void)> m_clickedCallback;

public:
    explicit MenuButton(QPixmap *up, QPixmap *down, QPixmap *hover, QGraphicsItem *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);

    inline void setCallback(std::function<void(void)> callback) {
        m_clickedCallback = callback;
    }

    virtual void clicked();
    virtual ~MenuButton();

};

#endif // MENUBUTTON_H
