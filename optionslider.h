#ifndef OPTIONSLIDER_H
#define OPTIONSLIDER_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <functional>

#include "gameengine.h"
#include "sprite.h"
#include "IMenuItem.h"

class GameEngine;

class OptionSlider : public QGraphicsItemGroup, public IMenuItem
{
private:
    int m_value;

    int *m_listener;
    QGraphicsPixmapItem *m_slider, *m_bkg;
    QGraphicsSimpleTextItem *m_txt;

    std::function<void(void)> m_clickedCallback;
public:
    explicit OptionSlider(QPixmap up, QPixmap slider, QGraphicsItem *parent = 0);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);

    int optionState();

    inline void setCallback(std::function<void(void)> callback) {
        m_clickedCallback = callback;
    }

    void setListener(void* listener) {
        m_listener = (int*) listener;
    }

    void saveValue() {
        if (m_listener) *m_listener = this->m_value;
    }

    virtual ~OptionSlider();
};

#endif // OPTIONSLIDER_H
