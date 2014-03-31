/***************************************************
** Filename: switchobject.cpp
** Class: SwitchObject
** Created: 3/27/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#include "switchobject.h"

SwitchObject::SwitchObject(int width, int height, QGraphicsItem* parent) :
    AnimatedCollideableSprite(width, height, parent) {
    this->setSolid(false);
    this->setZValue(10);
    this->setShapeMode(ShapeMode::BoundingRectShape);

    m_toggleDelay.setSingleShot(true);
}
