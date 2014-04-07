/***************************************************
** Filename: switchobject.cpp
** Class: SwitchObject
** Created: 3/27/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#include "switchobject.h"

SwitchObject::SwitchObject(int width, int height, b2Body* body, QGraphicsItem* parent) :
    AnimatedCollideableSprite(width, height, body, parent) {
    this->setSolid(false);
    this->setZValue(10);
    this->setShapeMode(ShapeMode::BoundingRectShape);

    m_toggleDelay.setSingleShot(true);
}
