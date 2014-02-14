#include "staticbackground.h"

StaticBackground::StaticBackground(QPoint repeatDistance, QGraphicsItem* parent) :
    Sprite(parent),
    m_repeatDistance(repeatDistance) {
    this->setZValue(-1000);
}

bool StaticBackground::isStatic() {
    return true;
}

bool StaticBackground::isAnimated() {
    return false;
}

bool StaticBackground::isCollideable() {
    return true;
}

bool StaticBackground::isBackground() {
    return true;
}

void StaticBackground::step(qint64 time, long delta) {
    // do nothing... for now ;)
}
