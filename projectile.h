#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "animatedcollideablesprite.h"
#include <Box2D.h>

class Projectile : public AnimatedCollideableSprite
{
public:
    Projectile(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);

    static b2BodyDef getBodyDef(b2Vec2 b2Pos) {
        b2BodyDef bodyDef;
        bodyDef.position.Set(b2Pos.x, b2Pos.y);
        bodyDef.type = b2_dynamicBody;
        bodyDef.bullet = true;
        return bodyDef;
    }

    static b2BodyDef getBodyDef(qreal qtx, qreal qty) {
        return Projectile::getBodyDef(b2Vec2(PX_TO_M(qtx), PX_TO_M(-qty)));
    }

    static b2PolygonShape getShape(int width, int height) {
        b2PolygonShape shape;
        shape.SetAsBox(PX_TO_M(width/2),
                       PX_TO_M(height/2),
                       b2Vec2(PX_TO_M(width/2), PX_TO_M(-height/2)),
                       0);
        return shape;
    }

    static b2FixtureDef getFixtureDef(int width, int height) {
        b2FixtureDef fixDef;
        b2PolygonShape shape = Projectile::getShape(width, height);
        fixDef.density = 1.0f;
        fixDef.friction = 0.1f;
        fixDef.isSensor = false;
        fixDef.shape = &shape;
        return fixDef;
    }

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }
    virtual QString className() { return "Projectile"; }
    virtual void collisionOccurred(Sprite *sprite, Side side);

signals:

public slots:

};

#endif // PROJECTILE_H
