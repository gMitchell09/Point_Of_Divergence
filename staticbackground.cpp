#include "staticbackground.h"

StaticBackground::StaticBackground(QPoint repeatDistance, QGraphicsItem* parent) :
    Sprite(parent),
    m_repeatDistance(repeatDistance) { }

bool StaticBackground::isStatic() {
    return true;
}

bool StaticBackground::isAnimated() {
    return false;
}

bool StaticBackground::isCollideable() {
    return false;
}

bool StaticBackground::isBackground() {
    return true;
}
