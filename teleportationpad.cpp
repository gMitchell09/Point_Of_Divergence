#include "teleportationpad.h"

TeleportationPad::TeleportationPad(int width, int height, bool controller, b2Body* body, QGraphicsItem *parent) :
AnimatedCollideableSprite(width, height, body, parent),
    mc(nullptr),
    m_sender(controller) {
}

void TeleportationPad::collisionOccurred(Sprite* other, Side side) {
    if (this->isController() && other->className() == "MainCharacter") {
        mc = dynamic_cast<MainCharacter*>(other);
        emit stateChanged(true);
    }
}

void TeleportationPad::controllerStateChanged(bool state) {
    if (state && this->isReceiver()) {
        TeleportationPad *other = dynamic_cast<TeleportationPad*>(QObject::sender());
        if (other->mc != nullptr) {
            other->mc->setB2DPosition(this->pos().x(), this->pos().y() - this->pixmap().height());
        }
    }
}
