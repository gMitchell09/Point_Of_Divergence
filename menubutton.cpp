#include "menubutton.h"

MenuButton::MenuButton(QPixmap *up, QPixmap *down, QPixmap *hover, QGraphicsItem *parent) :
    Sprite(parent),
    m_upGraphic(up),
    m_downGraphic(down),
    m_hoverGraphic(hover) {
    this->setPixmap(*m_upGraphic);
    this->setAcceptHoverEvents(true);
}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    this->m_pressed = true;
    this->setPixmap(*m_downGraphic);
}

void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) {
    if (m_hovered) {
        this->setPixmap(*m_hoverGraphic);
        this->m_pressed = false;
        this->clicked();
    }
    else if(m_pressed) {
        this->setPixmap(*m_upGraphic);
        this->m_pressed = false;
        this->clicked();
    }
}

void MenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent *ev) {
        this->m_hovered = true;
        this->setPixmap(*m_hoverGraphic);
}

void MenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev) {
    this->m_hovered = false;

    this->setPixmap(*m_upGraphic);
}

void MenuButton::clicked() {
    if (m_clickedCallback) {
        m_clickedCallback();
    }
}

MenuButton::~MenuButton() {
    delete m_upGraphic;
    delete m_downGraphic;
    delete m_hoverGraphic;
}
