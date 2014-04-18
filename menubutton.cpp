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
    Q_UNUSED(ev);
    this->setPixmap(*m_downGraphic);
}

void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) {
    if (this->boundingRect().contains(ev->pos())) {
        this->setPixmap(*m_upGraphic);
        this->clicked();
    }
}

void MenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent *ev) {
    Q_UNUSED(ev);
    this->setPixmap(*m_hoverGraphic);
}

void MenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev) {
    Q_UNUSED(ev);
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
