#include "tile.h"

Tile::Tile(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent)
{
}

void Tile::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Hold
}
