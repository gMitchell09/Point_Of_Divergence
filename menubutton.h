#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "sprite.h"

class MenuButton : public Sprite
{
private:
    bool m_pressed;
    QPixmap m_upGraphic, m_downGraphic, m_highlightGraphic;

public:
    explicit MenuButton(QPixmap up, QPixmap down, QPixmap highlight, QGraphicsItem *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);

    virtual void clicked() {}
};

#endif // MENUBUTTON_H
