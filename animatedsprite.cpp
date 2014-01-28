#include "animatedsprite.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsTransform>
#include <QGraphicsView>

#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

AnimatedSprite::AnimatedSprite(int width, int height, QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent),
    m_width(width), m_height(height) {

    m_leftAccel = -100;
    m_maxVelX = 500;
    m_maxVelY = 5000;

    m_jumpStartVel = -1000;
    m_gravity = 2000;

    m_brake = false;

    jumping = false;


    // Top
    m_collisionPoints[0][0] = QPoint(width/3, 0);
    m_collisionPoints[0][1] = QPoint((2*width)/3, 0);
    // Right
    m_collisionPoints[1][0] = QPoint(width, height/3);
    m_collisionPoints[1][1] = QPoint(width, (2*height)/3);
    // Bottom
    m_collisionPoints[2][0] = QPoint(width/3, height);
    m_collisionPoints[2][1] = QPoint((2*width)/3, height);
    // Left
    m_collisionPoints[3][0] = QPoint(0, height/3);
    m_collisionPoints[3][1] = QPoint(0, (2*height)/3);

    m_acceleration.setY(m_gravity);
}

void AnimatedSprite::jump() {
    m_velocity.setY(m_jumpStartVel);
}

void AnimatedSprite::addAnimation(std::vector<QPixmap> pixmapList, QPainterPath animationPath, kAnimationType animationType) {
    m_animationList.push_back(pixmapList);
    m_animationPathList.push_back(animationPath);
    m_animationType.push_back(animationType);
}

void AnimatedSprite::addAnimation(std::vector<QPixmap> pixmapList, kAnimationType animationType) {
    m_animationList.push_back(pixmapList);
    m_animationType.push_back(animationType);
}

void AnimatedSprite::triggerAnimation(unsigned int anim) {
    m_nCurrentAnimation = anim;
    m_msPerFrame = 200;

    switch (m_animationType.at(anim)) {
        case Forward:
        case Forward_Reverse:
        case Forward_Reverse_Loop:
        case Loop:
            m_countUp = true;
            m_nCurrentFrame = 0;
            break;
        case Reverse_Forward:
        case Reverse_Forward_Loop:
            m_countUp = false;
            m_nCurrentFrame = m_animationList.at(anim).size()-1;
            break;
    }

    this->setPixmap(m_animationList.at(m_nCurrentAnimation).at(m_nCurrentFrame));
}

void AnimatedSprite::step(unsigned long time) {
    m_msCounter += time;
    if (m_msCounter >= m_msPerFrame) {
        switch(m_animationType.at(m_nCurrentAnimation)) {
            case Forward:
                // DO NOT REPLACE +1 WITH ++.  IF YOU DO I _WILL_ FIND YOU.  :)
                m_nCurrentFrame = MIN(m_nCurrentFrame+1, m_animationList.at(m_nCurrentAnimation).size() - 1);
                break;

            case Loop:
                m_nCurrentFrame = ++m_nCurrentFrame % m_animationList.at(m_nCurrentAnimation).size();
                break;

            case Forward_Reverse:
                if (m_nCurrentFrame == 0 && !m_countUp) break;
                if (m_nCurrentFrame == m_animationList.at(m_nCurrentAnimation).size() - 1)
                    m_countUp = !m_countUp;

                if (m_countUp)
                    m_nCurrentFrame++;
                else
                    m_nCurrentFrame--;
                break;

            case Reverse_Forward:
                if (m_nCurrentFrame == m_animationList.at(m_nCurrentAnimation).size() - 1 && m_countUp) break;
                if (m_nCurrentFrame == 0)
                    m_countUp = !m_countUp;

                if (m_countUp)
                    m_nCurrentFrame++;
                else
                    m_nCurrentFrame--;
                break;

            case Forward_Reverse_Loop:
            case Reverse_Forward_Loop:
                if ((m_nCurrentFrame == m_animationList.at(m_nCurrentAnimation).size() - 1 && m_countUp)||
                        (m_nCurrentFrame == 0 && !m_countUp))
                    m_countUp = !m_countUp;

                if (m_countUp)
                    m_nCurrentFrame++;
                else
                    m_nCurrentFrame--;
                break;

            default:
                break;
        }
        m_msCounter = 0;
    }

    double timeStep = time / 1000.0;
    QList<Collision> collisions;

    QPointF oldVel = m_velocity;
    float newYVel, newXVel;

    m_velocity += m_acceleration * timeStep;
    newXVel = m_velocity.x();
    newYVel = m_velocity.y();

    unsigned char side = this->checkForCollision(collisions, (m_velocity + oldVel) * 0.5 * timeStep);
    if (((side & Top) && m_velocity.y() < 0) || ((side & Bottom) && m_velocity.y() > 0)) newYVel = 0;
    if (((side & Left) && m_velocity.x() < 0) || ((side & Right) && m_velocity.x() > 0)) newXVel = 0;

    m_velocity = QPointF(newXVel, newYVel);

    if (m_brake) {
        if (SIGN(m_velocity.x()) == SIGN(m_acceleration.x())) {
            m_velocity.setX(0);
            m_acceleration.setX(0);
            m_brake = false;
        }
    }

    if (m_velocity.x() > 0)
        m_velocity.setX(MIN(m_velocity.x(), m_maxVelX));
    else
        m_velocity.setX(MAX(m_velocity.x(), -m_maxVelX));

    if (m_velocity.y() > 0)
        m_velocity.setY(MIN(m_velocity.y(), m_maxVelY));
    else
        m_velocity.setY(MAX(m_velocity.y(), -m_maxVelY));


    QPointF oldPos = this->pos();

    this->setPos(this->pos() + (m_velocity + oldVel) * 0.5 * timeStep);

    if (side & Top) {
        // If m_velocity.y is positive then set it to zero
        if (m_velocity.y() < 0) this->m_velocity.setY(0);
        //if (m_acceleration.y() < 0) this->m_acceleration.setY(0);
        this->pos().setY(oldPos.y());
    }

//    if (side & Right) {
//        if (m_velocity.x() > 0) this->m_velocity.setX(0);
//        this->pos().setX(oldPos.x());
//    }

//    if (side & Bottom) {
//        if (m_velocity.y() > 0) this->m_velocity.setY(0);
//        //if (m_acceleration.y() > 0) this->m_acceleration.setY(0);
//        this->pos().setY(oldPos.y());
//    }

//    if (side & Left) {
//        if (m_velocity.x() < 0) this->m_velocity.setX(0);
//        this->pos().setX(oldPos.x());
//    }

    this->setPixmap(m_animationList.at(m_nCurrentAnimation).at(m_nCurrentFrame));
}



unsigned char AnimatedSprite::checkForCollision(QList<Collision>& collisions, QPointF offset) {
    bool cTop=0, cRight=0, cBottom=0, cLeft=0;
    unsigned char side = 0;
    AnimatedSprite *collidee;
    QPointF offsetPos = this->pos() + offset;

    // Check top points for collision
    collidee = dynamic_cast<AnimatedSprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[0][0], this->transform()));
    if (this->scene()->itemAt(offsetPos + m_collisionPoints[0][0], this->transform()) != NULL &&
        this->scene()->itemAt(offsetPos + m_collisionPoints[0][1], this->transform()) != NULL) {
        cTop = true;
        side |= Top;
        Collision col = {collidee, Top};
        collisions.append(col);
    }

    // Check right points for collision
    collidee = dynamic_cast<AnimatedSprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[1][0], this->transform()));
    if (this->scene()->itemAt(offsetPos + m_collisionPoints[1][0], this->transform()) != NULL &&
        this->scene()->itemAt(offsetPos + m_collisionPoints[1][1], this->transform()) != NULL) {
        cRight = true;
        side |= Right;
        Collision col = {collidee, Right};
        collisions.append(col);
    }

    // Check left points for collision
    collidee = dynamic_cast<AnimatedSprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[2][0], this->transform()));
    if (this->scene()->itemAt(offsetPos + m_collisionPoints[2][0], this->transform()) != NULL &&
        this->scene()->itemAt(offsetPos + m_collisionPoints[2][1], this->transform()) != NULL) {
        cBottom = true;
        side |= Bottom;
        Collision col = {collidee, Bottom};
        collisions.append(col);
    }

    // Check right points for collision
    collidee = dynamic_cast<AnimatedSprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[3][0], this->transform()));
    if (this->scene()->itemAt(offsetPos + m_collisionPoints[3][0], this->transform()) != NULL &&
        this->scene()->itemAt(offsetPos + m_collisionPoints[3][1], this->transform()) != NULL) {
        cLeft = true;
        side |= Left;
        Collision col = {collidee, Left};
        collisions.append(col);
    }

    if ((cTop && cBottom) || (cLeft && cRight)) qDebug() << "SQUISH -- " << side;

    return side;
}

//Side AnimatedSprite::collisionSide(QPointF otherVelocity, QPointF otherCenter) {
//    QPointF thisCenter = this->pos() + QPointF(this->boundingRect().width() / 2, this->boundingRect().height()/2);
//    qreal dy = thisCenter.y() - otherCenter.y();
//    qreal dx = thisCenter.x() - otherCenter.x();

//    if ()
//}
