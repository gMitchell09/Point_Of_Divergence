#include "turret.h"
#include "animatedcollideablesprite.h"
#include "projectile.h"
#include "gameengine.h"

static const QString spritePath = "./resources/sprites";
static const QString itemPath = spritePath + "/items";
static const QString otherSpritePath = spritePath + "/other";

Turret::Turret(int width, int height, bool facesLeft, int rate, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent),
    m_facesLeft(facesLeft) {
    m_fireDelay = rate;
}

void Turret::fireProjectile() {
    if (m_body) {
        b2BodyDef bodyDef;
        bodyDef.position.Set(PX_TO_M(this->x()), PX_TO_M(-this->y()));
        bodyDef.type = b2_dynamicBody;
        bodyDef.bullet = true;
        bodyDef.gravityScale = 0.0f;
        bodyDef.linearVelocity = b2Vec2(5, 0);
        b2Body* body = m_body->GetWorld()->CreateBody(&bodyDef);
        b2PolygonShape shape;
        shape.SetAsBox(PX_TO_M(11)/2., PX_TO_M(11)/2.,
                       b2Vec2(PX_TO_M(11.)/2, -PX_TO_M(11.)/2), 0);
        body->CreateFixture(&shape, 5.0f);

        Projectile * bullet = new Projectile(11, 11, body);

        QPixmap tileImage(otherSpritePath + "/projectile.png");
        tileImage.setMask(tileImage.createHeuristicMask(true));

        bullet->setPixmap(tileImage);
        bullet->setPos(this->pos());
        bullet->setShapeMode(Tile::BoundingRectShape);

        body->SetUserData(bullet);

        GameEngine* ge = dynamic_cast<GameEngine*>(this->scene());
        if (ge) {
            ge->addSprite(bullet);
        }
    }
}

void Turret::step(qint64 time, long delta) {
    m_lastFireTime += delta;
    if (m_lastFireTime >= m_fireDelay) {
        m_lastFireTime = 0;
//        this->fireProjectile();
    }
}
