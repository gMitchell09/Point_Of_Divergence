#include "menubutton.h"

MenuButton::MenuButton(QPixmap up, QPixmap down, QPixmap highlight, QGraphicsItem *parent) :
    Sprite(parent),
    m_upGraphic(up),
    m_downGraphic(down),
    m_highlightGraphic(highlight) {

}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    this->m_pressed = true;

    this->setPixmap(m_downGraphic);
}

void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) {
    if (m_pressed) {
        this->setPixmap(m_upGraphic);
        m_pressed = false;

        this->clicked();
    }
}

void MenuButton::mouseMoveEvent(QGraphicsSceneMouseEvent *ev) {
    if (!m_pressed) {
        this->setPixmap(m_highlightGraphic);
    }
}
