/* Author: Jesse Hairston
 * Use Case: 07 (Destroy Enemy)
 */

#include "sfxmanager.h"
#include "enemy1.h"
#include "gameengine.h"

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
    this->setVelocity(QPointF(0, 0));
    this->setAcceleration(QPointF(0, 0));

    m_currentState = Stand;
    this->triggerAnimation(m_currentState);
    this->setAcceleration(QPointF(m_rightAccel, m_gravity));
}

void Enemy1::step(qint64 time, long delta) {
    AnimatedCollideableSprite::step(time, delta);

    if (this->getVelocity().x() > m_maxVelX) this->getVelocity().setX(m_maxVelX);
    if (this->getVelocity().y() > m_maxVelY) this->getVelocity().setY(m_maxVelY);

    if (m_currentState == Squish) {
        m_squishCtr += delta;
        if (m_squishCtr > 250) {
            ((GameEngine*)this->scene())->removeItem(this);
        }
    }
}

void Enemy1::collisionsOccurred(QList<Collision> &collisions, unsigned char side) {
    AnimatedCollideableSprite::collisionsOccurred(collisions, side);
    if (m_currentState == Squish) return;
    if (side & Right) {
        this->getAcceleration().setX(m_leftAccel);
    }
    if (side & Left) {
        this->getAcceleration().setX(m_rightAccel);
    }

    for (auto itr = collisions.begin(); itr != collisions.end(); ++itr) {
        Collision col = (*itr);
        if (col.firstSide & Top && col.secondSprite->className() == "MainCharacter") {
            // Bounce!!!
            col.secondSprite->setVelocity(QPointF(col.secondSprite->getVelocity().x(), -400));
            m_currentState = Squish;
            this->triggerAnimation(m_currentState);

            SFXManager *inst = SFXManager::Instance();
            inst->playSound(SFXManager::SFX::Enemy_Squish);
        }
    }
}

void Enemy1::collisionOccurred(Sprite *other, Side side) {
    AnimatedCollideableSprite::collisionOccurred(other, side);
    if (m_currentState == Squish) return;
    if (side == Right) {
        this->getAcceleration().setX(m_leftAccel);
    }
    if (side == Left) {
        this->getAcceleration().setX(m_rightAccel);
    }
    if (side == Top && other->className() == "MainCharacter") {
        //col.secondSprite->setVelocity(QPointF(col.secondSprite->getVelocity().x(), -400));
        m_currentState = Squish;
        this->triggerAnimation(m_currentState);

        SFXManager *inst = SFXManager::Instance();
        inst->playSound(SFXManager::SFX::Enemy_Squish);
    }
}
