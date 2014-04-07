/***************************************************
** Filename: door.cpp
** Class: Door
** Created: 3/31/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#include "door.h"

Door::Door(int width, int height, b2Body* body, QGraphicsItem* parent) :
    AnimatedCollideableSprite(width, height, body, parent) {
    this->setSolid(true);
}
