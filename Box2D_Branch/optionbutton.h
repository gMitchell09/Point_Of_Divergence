#ifndef OPTIONBUTTON_H
#define OPTIONBUTTON_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <functional>

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <functional>

#include "gameengine.h"
#include "sprite.h"
#include "IMenuItem.h"

class GameEngine;

class OptionButton : public QGraphicsItemGroup, public IMenuItem
{
private:
    bool m_state;
    bool *m_listener;
    QGraphicsPixmapItem *m_slider, *m_bkg;

    std::function<void(void)> m_clickedCallback;

public:
    explicit OptionButton(QPixmap bkg, QPixmap sliderGraphic, QGraphicsItem *parent = 0);

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
