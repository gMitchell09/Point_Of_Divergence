#ifndef ANIMATEDCOLLIDEABLESPRITE_H
#define ANIMATEDCOLLIDEABLESPRITE_H

/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedsprite.h"
#include "networkmanager.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>

#include "Box2D.h"

class AnimatedCollideableSprite : public AnimatedSprite
{
private:
    bool m_onLeftSlope, m_onRightSlope;

    void updatePosition();

public:
    explicit AnimatedCollideableSprite(int width, int height, b2Body* body, QGraphicsItem *parent);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(AnimatedCollideableSprite *sprite, Side side);

    bool isOnLeftSlope() { return m_onLeftSlope; }
    bool isOnRightSlope() { return m_onRightSlope; }

    virtual void fillDatagram(NetworkManager::DatagramFormat &datagram) {
        AnimatedSprite::fillDatagram(datagram);

        datagram.pos = (m_body) ? m_body->GetPosition() : b2Vec2();
        datagram.vel = (this->getVelocity());
    }

    virtual void decodeDatagram(NetworkManager::DatagramFormat dg) {
        AnimatedSprite::decodeDatagram(dg);

        this->setPosition(dg.pos);
        this->setVelocity(dg.vel, true);
    }

    virtual void pushState(qint64 time, long delta, State& state) {
        if (m_body) {
            state.pos = m_body->GetPosition();
            state.vel = m_body->GetLinearVelocity();
            state.angle = m_body->GetAngle();
        }

        AnimatedSprite::pushState(time, delta, state);
    }

    virtual void setState(State currentState) {
        AnimatedSprite::setState(currentState);
        if (m_body) {
            m_body->SetTransform(currentState.pos, currentState.angle);
            m_body->SetLinearVelocity(currentState.vel);
            this->updatePosition();
        }
    }

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }
    virtual QString className() { return "AnimatedCollideableSprite"; }
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // Position is Box2D position
    // Hides base implemenation
    static b2BodyDef getBodyDef(b2Vec2 b2Pos, bool isStatic) {
        b2BodyDef bodyDef;
        bodyDef.position.Set(b2Pos.x, b2Pos.y);
        bodyDef.type = (isStatic) ? b2_staticBody : b2_dynamicBody;
        return bodyDef;
    }

    static b2BodyDef getBodyDef(qreal qtx, qreal qty, bool isStatic) {
        return AnimatedCollideableSprite::getBodyDef(b2Vec2(PX_TO_M(qtx), PX_TO_M(-qty)), isStatic);
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
        b2PolygonShape shape = AnimatedCollideableSprite::getShape(width, height);
        fixDef.density = 1.0f;
        fixDef.friction = 0.1f;
        fixDef.isSensor = false;
        fixDef.shape = &shape;
        return fixDef;
    }

//    void setAcceleration(b2Vec2 acceleration) { m_acceleration = acceleration; }
//    b2Vec2& getAcceleration() {  }

    void setVelocity(b2Vec2 velocity, bool resetForces = true) { if (m_body) { m_body->SetLinearVelocity(velocity); if (resetForces) this->resetForces();} }
    b2Vec2 getVelocity() { if (m_body) return m_body->GetLinearVelocity(); else return b2Vec2(); }
    void setForce(b2Vec2 force) { if (m_body) m_body->ApplyForceToCenter(force, true);}
    void resetForces() { if (m_body) { m_body->SetLinearVelocity(b2Vec2(0, m_body->GetLinearVelocity().y)); m_body->SetAngularVelocity(0); } }

    void setPosition(b2Vec2 pos, float32 angle = 0) { if (m_body) m_body->SetTransform(pos, angle); }

    void setBody(b2Body* body) { m_body = body; }

    virtual ~AnimatedCollideableSprite();

protected:
    virtual bool usesStack() { return false; }

    b2Body* m_body;

private:
    virtual float getFriction() const { return 1.0f; }
    virtual float getDensity() const { return 1.0f; }
    virtual bool isSensor() const { return false; }

};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
