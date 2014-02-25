#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "sprite.h"

class MenuButton : public Sprite
{
private:
    bool m_pressed;
    QPixmap *m_upGraphic, *m_downGraphic, *m_highlightGraphic;

public:
    explicit MenuButton(QPixmap *up, QPixmap *down, QGraphicsItem *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);

    virtual void clicked() { qDebug() << "Clicked!"; }

    virtual ~MenuButton();
};

#endif // MENUBUTTON_H
