#include "teleportationpad.h"

TeleportationPad::TeleportationPad(int width, int height, b2Body* body, QGraphicsItem *parent) :
AnimatedCollideableSprite(width, height, body, parent) {
}
