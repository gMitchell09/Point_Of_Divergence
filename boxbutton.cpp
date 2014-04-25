#include "boxbutton.h"

BoxButton::BoxButton(int width, int height, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent)
{
}

void BoxButton::collisionOccurred(AnimatedCollideableSprite *other, Side side) {
    Q_UNUSED(side);
    if (!m_state && other->blockType() == kBox) {
        m_state = true;
        this->setPixmap(m_onPixmap);
        emit stateChanged(true);
    }
}
