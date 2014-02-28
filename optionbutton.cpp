#include "optionbutton.h"

OptionButton::OptionButton(QPixmap *up, QPixmap *down, QGraphicsItem *parent) :
    Sprite(parent),
    m_upGraphic(up),
    m_downGraphic(down),
    m_state(true) {
    this->setPixmap(*m_upGraphic);
}

void OptionButton::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    this->m_pressed = true;
}

void OptionButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) {
    if (m_pressed) {
        if(m_state) this->setPixmap(*m_downGraphic);
        else this->setPixmap(*m_upGraphic);

        this->m_state = !m_state;
        this->m_pressed = false;
        this->clicked();
    }
}

void OptionButton::clicked() {
    if (m_clickedCallback) {
        m_clickedCallback();
    }
}

bool OptionButton::optionState() {
    return(m_state);
}

OptionButton::~OptionButton() {
    delete m_upGraphic;
    delete m_downGraphic;
}
