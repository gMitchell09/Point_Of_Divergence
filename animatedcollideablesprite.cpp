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
    float newYVel, newXVel;

    m_velocity += m_acceleration * timeStep;
    newXVel = m_velocity.x();
    newYVel = m_velocity.y();

    unsigned char side = this->checkForCollision(collisions, (m_velocity + oldVel) * 0.5 * timeStep);
    if (((side & Top) && m_velocity.y() < 0) || ((side & Bottom) && m_velocity.y() > 0)) newYVel = 0;
    if (((side & Left) && m_velocity.x() < 0) || ((side & Right) && m_velocity.x() > 0)) newXVel = 0;

    if (!collisions.empty()) this->collisionOccurred(collisions, side);

    m_velocity = QPointF(newXVel, newYVel);

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
        Collision col = {this, collidee, QPointF(0,0), Top, Top, QPointF(0,0)};
        collisions.append(col);
    }

    // Check right points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[1][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[1][1], this->transform()));
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cRight = true;
        side |= Right;
        Collision col = {this, collidee, QPointF(0,0), Right, Right, QPointF(0,0)};
        collisions.append(col);
    }

    // Check left points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[2][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[2][1], this->transform()));
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cBottom = true;
        side |= Bottom;
        Collision col = {this, collidee, QPointF(0,0), Bottom, Bottom, QPointF(0,0)};
        collisions.append(col);
    }

    // Check right points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[3][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[3][1], this->transform()));
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cLeft = true;
        side |= Left;
        Collision col = {this, collidee, QPointF(0,0), Left, Left, QPointF(0,0)};
        collisions.append(col);
    }

    if ((cTop && cBottom) || (cLeft && cRight)) qDebug() << "SQUISH -- " << side;

    return side;
}

void AnimatedCollideableSprite::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Override this method in your subclasses if you want to be alerted when collisions occur.
}
