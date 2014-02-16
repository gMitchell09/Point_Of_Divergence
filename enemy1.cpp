#include "enemy1.h"

Enemy1::Enemy1(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent) {

    m_leftAccel = -250;
    m_rightAccel = -m_leftAccel;
    m_maxVelX = 300;
    m_maxVelY = 2000;

    m_jumpStartVel = -500;
    m_gravity = 2000;

    QPixmap goombaStand(":Goomba/GoombaStand.png");
    QPixmap goombaSquish(":Goomba/GoombaSquish.png");

    std::vector<QPixmap> stand;
    stand.push_back(goombaStand);

    std::vector<QPixmap> squish;
    squish.push_back(goombaSquish);

    this->addAnimation(stand, Loop);
    this->addAnimation(squish, Loop);

    this->setSolid(true);
    this->setVelocity(QPointF(0, 0));
    this->setAcceleration(QPointF(0, 0));

    this->triggerAnimation(Goomba::Stand);
    this->setAcceleration(QPointF(m_rightAccel, m_gravity));
}
void Enemy1::step(qint64 time, long delta) {
    AnimatedCollideableSprite::step(time, delta);
    if (this->getVelocity().x() > m_maxVelX) this->getVelocity().setX(m_maxVelX);
    if (this->getVelocity().y() > m_maxVelY) this->getVelocity().setY(m_maxVelY);
}
void Enemy1::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    AnimatedCollideableSprite::collisionOccurred(collisions, side);
    if (side & Right) {
        this->getAcceleration().setX(m_leftAccel);      //walk left
    }
    if (side & Left) {
        this->getAcceleration().setX(m_rightAccel);     //walk right
    }
    if (side & Top) {
        this->triggerAnimation(Goomba::Squish);         //trigger squish animation
    }
}
