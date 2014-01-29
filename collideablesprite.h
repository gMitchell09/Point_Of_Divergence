#ifndef COLLIDEABLESPRITE_H
#define COLLIDEABLESPRITE_H

#include <QGraphicsItem>

#include "sprite.h"
#include "animatedsprite.h"

class CollideableSprite : public Sprite
{
public:
    explicit CollideableSprite(QGraphicsItem *parent = 0);
    
signals:
    
public slots:
    
};

#endif // COLLIDEABLESPRITE_H
