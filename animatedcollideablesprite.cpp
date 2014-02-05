/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedcollideablesprite.h"
#include "sprite.h"

#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

AnimatedCollideableSprite::AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
    // Top
    m_collisionPoints[0][0] = QPoint(width/6, 0);
    m_collisionPoints[0][1] = QPoint((5*width)/6, 0);
    // Right
    m_collisionPoints[1][0] = QPoint(width, height/6);
    m_collisionPoints[1][1] = QPoint(width, (5*height)/6);
    // Bottom
    m_collisionPoints[2][0] = QPoint(width/6, height);
    m_collisionPoints[2][1] = QPoint((5*width)/6, height);
    // Left
    m_collisionPoints[3][0] = QPoint(0, height/6);
    m_collisionPoints[3][1] = QPoint(0, (5*height)/6);
}

void AnimatedCollideableSprite::step(unsigned long time) {
    AnimatedSprite::step(time);

    double timeStep = time / 1000.0;
    QList<Collision> collisions;

    QPointF oldVel = m_velocity;
    QPointF newVel;

    m_velocity += m_acceleration * timeStep;
    newVel = m_velocity;

    unsigned char side = this->checkForCollision(collisions, (m_velocity + oldVel) * 0.5 * timeStep);
    if (((side & Top) && m_velocity.y() < 0) || ((side & Bottom) && m_velocity.y() > 0)) newVel.setY(0);
    if (((side & Left) && m_velocity.x() < 0) || ((side & Right) && m_velocity.x() > 0)) newVel.setX(0);

    if (!collisions.empty()) {
        QPointF newPos = QPointF(0,0);
        this->collisionOccurred(collisions, side);
        for (auto itr = collisions.begin(); itr != collisions.end(); itr++) {
            if ((side & Bottom) || (this->pos().x() <= ((Collision)(*itr)).secondSprite->pos().x() && ((Collision)(*itr)).normalVector.x() < 0) ||
                (this->pos().x() >= ((Collision)(*itr)).secondSprite->pos().x() && ((Collision)(*itr)).normalVector.x() > 0)) {
                newPos.setX(newPos.x() + ((Collision)(*itr)).normalVector.x() * timeStep);
            }
            if ((this->pos().y() <= ((Collision)(*itr)).secondSprite->pos().y() && ((Collision)(*itr)).normalVector.y() < 0) ||
                (this->pos().y() >= ((Collision)(*itr)).secondSprite->pos().y() && ((Collision)(*itr)).normalVector.y() > 0)) {
                newPos.setY(newPos.y() + ((Collision)(*itr)).normalVector.y() * timeStep);
            }

            //newPos += ((Collision)(*itr)).normalVector * timeStep;
        }
        this->setPos(this->pos() + newPos);
    }


    m_velocity = newVel;

    QPointF oldPos = this->pos();

    this->setPos(this->pos() + (m_velocity + oldVel) * 0.5 * timeStep);

    if (side & Top) {
        // If m_velocity.y is positive then set it to zero
        if (m_velocity.y() < 0) this->m_velocity.setY(0);
        //if (m_acceleration.y() < 0) this->m_acceleration.setY(0);
        this->pos().setY(oldPos.y());
    }

}

unsigned char AnimatedCollideableSprite::checkForCollision(QList<Collision>& collisions, QPointF offset) {
    bool cTop=0, cRight=0, cBottom=0, cLeft=0;
    unsigned char side = 0;
    Sprite *collidee, *collidee2;
    QPointF offsetPos = this->pos() + offset;

    // Check top points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[0][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[0][1], this->transform()));
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cTop = true;
        side |= Top;

        Collision col;
        if (collidee != NULL) {
            col = {this, collidee, collidee->getVelocity(), Top, Top, QPointF(0,0)};
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee) {
            col = {this, collidee2, collidee2->getVelocity(), Top, Top, QPointF(0,0)};
            collisions.append(col);
        }
    }

    // Check right points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[1][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[1][1], this->transform()));
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cRight = true;
        side |= Right;

        Collision col;
        if (collidee != NULL) {
            col = {this, collidee, collidee->getVelocity(), Right, Right, QPointF(0,0)};
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee) {
            col = {this, collidee2, collidee2->getVelocity(), Right, Right, QPointF(0,0)};
            collisions.append(col);
        }
    }

    // Check left points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[2][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[2][1], this->transform()));
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cBottom = true;
        side |= Bottom;
        Collision col;
        if (collidee != NULL) {
            col = {this, collidee, collidee->getVelocity(), Bottom, Bottom, QPointF(0,0)};
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee) {
            col = {this, collidee2, collidee2->getVelocity(), Bottom, Bottom, QPointF(0,0)};
            collisions.append(col);
        }
    }

    // Check right points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[3][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[3][1], this->transform()));
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cLeft = true;
        side |= Left;

        Collision col;
        if (collidee != NULL) {
            col = {this, collidee, collidee->getVelocity(), Left, Left, QPointF(0,0)};
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee) {
            col = {this, collidee2, collidee2->getVelocity(), Left, Left, QPointF(0,0)};
            collisions.append(col);
        }
    }

    if ((cTop && cBottom) || (cLeft && cRight)) qDebug() << "SQUISH -- " << side;

    return side;
}

void AnimatedCollideableSprite::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Override this method in your subclasses if you want to be alerted when collisions occur.
}
