#include "menubutton.h"

MenuButton::MenuButton(QPixmap *up, QPixmap *down, QGraphicsItem *parent) :
    Sprite(parent),
    m_upGraphic(up),
    m_downGraphic(down) {
    this->setPixmap(*m_upGraphic);

}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    this->m_pressed = true;

    this->setPixmap(*m_downGraphic);
}

void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) {
    if (m_pressed) {
        this->setPixmap(*m_upGraphic);
        m_pressed = false;

        this->clicked();
    }
}

void MenuButton::mouseMoveEvent(QGraphicsSceneMouseEvent *ev) {
    if (!this->contains(ev->pos())) {
        m_pressed = false;
        this->setPixmap(*m_upGraphic);
    }
}

void MenuButton::clicked() {
    if (m_clickedCallback) {
        m_clickedCallback();
    }
}


MenuButton::~MenuButton() {
    delete m_upGraphic;
    delete m_downGraphic;
}
