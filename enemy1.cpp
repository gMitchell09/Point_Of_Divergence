/* Author: Jesse Hairston
 * Use Case: 07 (Destroy Enemy)
 */

#include "sfxmanager.h"
#include "enemy1.h"
#include "gameengine.h"

#define ABS(x) ((x>0)?(x):(-x))

Enemy1::Enemy1(int width, int height, QString path, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent) {

    m_leftAccel = -150;
    m_rightAccel = -m_leftAccel;
    m_maxVelX = 100;
    m_maxVelY = 2000;

    m_jumpStartVel = -500;
    m_gravity = 2000;

    m_squishCtr = 0;

    QPixmap goombaStand(path + "/GoombaStand.png");
    QPixmap goombaSquish(path + "/GoombaSquish.png");

    std::vector<QPixmap> stand;
    stand.push_back(goombaStand);

    std::vector<QPixmap> squish;
    squish.push_back(goombaSquish);

    this->addAnimation(stand, Loop);
    this->addAnimation(squish, Loop);

    this->setSolid(true);

    m_currentState = Stand;
    this->triggerAnimation(m_currentState);

    this->setVelocity(b2Vec2(this->m_maxSpeed, 0), true);
}

void Enemy1::step(qint64 time, long delta) {
    AnimatedCollideableSprite::step(time, delta);

//    if (this->getVelocity().x() > m_maxVelX) this->getVelocity().setX(m_maxVelX);
//    if (this->getVelocity().y() > m_maxVelY) this->getVelocity().setY(m_maxVelY);

    if (m_currentState == Squish) {
        m_squishCtr += delta;
        if (m_squishCtr > 250) {
            ((GameEngine*)this->scene())->removeItem(this);
        }
    }

    b2Vec2 vel = this->getVelocity();
    if (ABS(vel.x) > ABS(this->m_maxSpeed)) {
        this->setVelocity(b2Vec2(SIGN(vel.x) * this->m_maxSpeed, vel.y), false);
    }

    if (this->m_shuffleRight) {
        this->setVelocity(b2Vec2(this->m_maxSpeed, vel.y), false);
    } else {
        this->setVelocity(b2Vec2(-this->m_maxSpeed, vel.y), false);
    }
}

void Enemy1::collisionOccurred(AnimatedCollideableSprite *other, Side side) {
    AnimatedCollideableSprite::collisionOccurred(other, side);
    if (m_currentState == Squish) return;
    if (other->isSolid()) {
        if (side == Right) {
            m_shuffleRight = false;
        }
        if (side == Left) {
            m_shuffleRight = true;
        }
    }
    if (side == Top && other->className() == "MainCharacter") {
        //col.secondSprite->setVelocity(QPointF(col.secondSprite->getVelocity().x(), -400));
        m_currentState = Squish;
        this->triggerAnimation(m_currentState);

        SFXManager *inst = SFXManager::Instance();
        inst->playSound(SFXManager::SFX::Enemy_Squish);
    }
}
