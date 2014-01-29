#ifndef ANIMATEDCOLLIDEABLESPRITE_H
#define ANIMATEDCOLLIDEABLESPRITE_H

#include "animatedsprite.h"
#include <QGraphicsScene>
#include <QDebug>

enum Side { Top = 1, Right = 2, Bottom = 4, Left = 8 };

struct Collision {
    Sprite *firstSprite, *secondSprite;
    QPointF normalVector; // W.R.T. first!!!
    Side firstSide, secondSide;
    QPointF overlapDistance;
};

class AnimatedCollideableSprite : public AnimatedSprite
{
private:
    QPoint m_collisionPoints[4][2];
    QPointF m_acceleration, m_velocity;
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;
    bool jumping, m_brake;
    bool m_solid;

    //////////////////////////////
    virtual void step(unsigned long time);
    unsigned char checkForCollision(QList<Collision> &collisions, QPointF offset);
    void resolveCollision(Collision collision);

public:
    explicit AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent = 0);

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }

    void setSolid(bool solid) { m_solid = solid; }
    bool isSolid() { return m_solid; }

    void setAcceleration(QPointF acceleration) { m_acceleration = acceleration; }
    QPointF getAcceleration() { return m_acceleration; }

    void setVelocity(QPointF velocity) { m_velocity = velocity; }
    QPointF getVelocity() { return m_velocity; }

    void setBrake(bool brake) { m_brake = brake; }

    void jump();
    
signals:
    
public slots:
    
};

#endif // ANIMATEDCOLLIDEABLESPRITE_H
