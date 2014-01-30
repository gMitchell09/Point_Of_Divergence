#include "animatedcollideablesprite.h"
#include "sprite.h"

#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

AnimatedCollideableSprite::AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
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

}

unsigned char AnimatedCollideableSprite::checkForCollision(QList<Collision>& collisions, QPointF offset) {
    bool cTop=0, cRight=0, cBottom=0, cLeft=0;
    unsigned char side = 0;
    Sprite *collidee, *collidee2;
    QPointF offsetPos = this->pos() + offset;

    // Check top points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[0][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[0][1], this->transform()));
    if ((collidee != NULL && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2->isCollideable())) {
        cTop = true;
        side |= Top;
        Collision col = {this, collidee, QPointF(0,0), Top, Top, QPointF(0,0)};
        collisions.append(col);
    }

    // Check right points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[1][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[1][1], this->transform()));
    if ((collidee != NULL && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2->isCollideable())) {
        cRight = true;
        side |= Right;
        Collision col = {this, collidee, QPointF(0,0), Right, Right, QPointF(0,0)};
        collisions.append(col);
    }

    // Check left points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[2][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[2][1], this->transform()));
    if ((collidee != NULL && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2->isCollideable())) {
        cBottom = true;
        side |= Bottom;
        Collision col = {this, collidee, QPointF(0,0), Bottom, Bottom, QPointF(0,0)};
        collisions.append(col);
    }

    // Check right points for collision
    collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[3][0], this->transform()));
    collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[3][1], this->transform()));
    if ((collidee != NULL && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2->isCollideable())) {
        cLeft = true;
        side |= Left;
        Collision col = {this, collidee, QPointF(0,0), Left, Left, QPointF(0,0)};
        collisions.append(col);
    }

    if ((cTop && cBottom) || (cLeft && cRight)) qDebug() << "SQUISH -- " << side;

    return side;
}

void AnimatedCollideableSprite::jump() {
    m_velocity.setY(m_jumpStartVel);
}
