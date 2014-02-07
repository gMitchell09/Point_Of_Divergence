#ifndef HUDSPRITE_H
#define HUDSPRITE_H

/* Author: Jesse Hairston
 * Added: Friday Feb 7 @ 4:25pm
 * Initial Commit: Wednesday Feb 7 @ 4:25pm
 * Use Case: 09: Heads-Up Display
 */

#include "sprite.cpp"
#include <QGraphicsItem>

class HUDsprite : public Sprite
{
public:
    explicit HUDsprite(QGraphicsItem *parent = 0);

};

#endif // HUDSPRITE_H
