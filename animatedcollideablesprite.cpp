/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedcollideablesprite.h"
#include "sprite.h"

AnimatedCollideableSprite::AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
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
}

void AnimatedCollideableSprite::updateWhiskers(QPointF offset) {
    QPointF offsetPos = this->pos() + offset;

    QPointF wideWhisker = QPointF(2, 0);
    QPointF tallWhisker = QPointF(0, 2);

    QPointF offsetX = QPointF(offsetPos.x(), offsetPos.y()) + tallWhisker;
    QPointF offsetY = QPointF(offsetPos.x(), offsetPos.y()) + wideWhisker;

    topWhiskerLeft = QRectF(m_collisionPoints[0][0] + offsetY, m_collisionPoints[0][0] + this->pos());
    topWhiskerRight = QRectF(m_collisionPoints[0][1] + offsetY, m_collisionPoints[0][1] + this->pos());

    rightWhiskerTop = QRectF(m_collisionPoints[1][0] + offsetX, m_collisionPoints[1][0] + this->pos());
    rightWhiskerBottom = QRectF(m_collisionPoints[1][1] + offsetX, m_collisionPoints[1][1] + this->pos());

    bottomWhiskerRight = QRectF(m_collisionPoints[2][0] + offsetY, m_collisionPoints[2][0] + this->pos());
    bottomWhiskerLeft = QRectF(m_collisionPoints[2][1] + offsetY, m_collisionPoints[2][1] + this->pos());

    leftWhiskerBottom = QRectF(m_collisionPoints[3][0] + offsetX, m_collisionPoints[3][0] + this->pos());
    leftWhiskerTop = QRectF(m_collisionPoints[3][1] + offsetX, m_collisionPoints[3][1] + this->pos());
}

void AnimatedCollideableSprite::step(qint64 time, long delta) {
    AnimatedSprite::step(time, delta);

    bool timeReversed = false;

    if (delta < 0) {
        //time = time;
        timeReversed = true;
        this->setVelocity(QPointF(0, 0));
    }

    if (timeReversed && this->usesStack()) {
        if (!m_positionStateStack.empty()) {
            PositionState currentState;

            // Loop until we are ahead of the past... wait, WTF does that even mean?!?!
            while (!m_positionStateStack.empty() && (currentState = m_positionStateStack.top()).timestamp > time) {
                m_positionStateStack.pop();
            }

            this->setPos(currentState.pos);
        }
    }

    else {
        double timeStep = delta / 1000.0;
        QList<Collision> collisions;

        QPointF oldVel = m_velocity;
        QPointF newVel;
        QPointF relativeVel = QPointF(0, 0);

        m_velocity += m_acceleration * timeStep;
        newVel = m_velocity;

        if (!this->isStatic()) {
            unsigned char side = this->checkForCollision(collisions, (m_velocity + oldVel) * 0.5 * timeStep, timeReversed);

            if (!collisions.empty()) {
                QPointF newPos = QPointF(0,0);
                this->collisionOccurred(collisions, side);
                for (auto itr = collisions.begin(); itr != collisions.end(); itr++) {

                    AnimatedCollideableSprite *first = (AnimatedCollideableSprite*)((Collision)(*itr)).firstSprite;
                    AnimatedCollideableSprite *second = (AnimatedCollideableSprite*)((Collision)(*itr)).secondSprite;
                    Side locSide = (*itr).firstSide;

                    if (this->isSolid() && second->isSolid()) {
                        if (((locSide == Top) && m_velocity.y() < 0) || ((locSide == Bottom) && m_velocity.y() > 0)) newVel.setY(0);
                        if (((locSide == Left) && m_velocity.x() < 0) || ((locSide == Right) && m_velocity.x() > 0)) newVel.setX(0);

                        if ((side & Bottom) || (this->pos().x() <= ((Collision)(*itr)).secondSprite->pos().x() && ((Collision)(*itr)).normalVector.x() < 0) ||
                            (this->pos().x() >= ((Collision)(*itr)).secondSprite->pos().x() && ((Collision)(*itr)).normalVector.x() > 0)) {
                            //newPos.setX(newPos.x() + ((Collision)(*itr)).normalVector.x() * timeStep);
                            relativeVel.setX(((Collision)(*itr)).secondSprite->getVelocity().x());
                        }
                        if ((this->pos().y() <= ((Collision)(*itr)).secondSprite->pos().y() && ((Collision)(*itr)).normalVector.y() < 0) ||
                            (this->pos().y() >= ((Collision)(*itr)).secondSprite->pos().y() && ((Collision)(*itr)).normalVector.y() > 0)) {
                            //newPos.setY(newPos.y() + ((Collision)(*itr)).normalVector.y() * timeStep);
                            relativeVel.setY(((Collision)(*itr)).secondSprite->getVelocity().y());
//                            qDebug() << "Normal Vector: " << ((Collision)(*itr)).normalVector.y();
                        }
                    }
                    //newPos += ((Collision)(*itr)).normalVector * timeStep;
                }
                this->setPos(this->pos() + newPos);
            }

            m_velocity = newVel;

            QPointF oldPos = this->pos();

            this->setPos(this->pos() + ((m_velocity + oldVel) * 0.5 + relativeVel) * timeStep);
        }

        if (this->usesStack()) {
            PositionState currentState;
            currentState.pos = this->pos();
            currentState.timestamp = time;
            m_positionStateStack.push(currentState);
        }
    }
}

unsigned char AnimatedCollideableSprite::checkForCollision(QList<Collision>& collisions, QPointF offset, bool timeReversed) {
    bool cTop=0, cRight=0, cBottom=0, cLeft=0;
    unsigned char side = 0;
    Sprite *collidee, *collidee2;

    this->updateWhiskers(offset);

    // Check top points for collision
    //collidee = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[0][0], this->transform()));
    //collidee2 = dynamic_cast<Sprite*>(this->scene()->itemAt(offsetPos + m_collisionPoints[0][1], this->transform()));

    /* Images always make things easier:
     *
     *   |----[---------]----|
     *   |    [   Top   ]    |
     *[ Left ]           [ Right]
     *[  |   ]           [   |  ]
     *   |                   |
     *   |____[_Bottom__]____|
     *        [         ]
     */

    collidee = spriteWithinWhisker(topWhiskerLeft);
    collidee2 = spriteWithinWhisker(topWhiskerRight);

    if ((collidee != NULL) || (collidee2 != NULL)) {
        cTop = true;
        side |= Top;

        Collision col;
        if (collidee != NULL && collidee != this) {
//            col = {this, collidee, collidee->getVelocity(), Left, Left, QPointF(0,0)};
            // CAN'T USE LINE ABOVE BECAUSE MSVS SUCKS!!!!  OR SHOULD I SAY, MS* SUCKS.
            col.firstSprite = this;
            col.secondSprite = collidee;
            col.normalVector = collidee->getVelocity();
            col.firstSide = Top;
            col.secondSide = Top;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Top;
            col.secondSide = Top;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
    }

    // Check right points for collision
    collidee = spriteWithinWhisker(rightWhiskerTop);
    collidee2 = spriteWithinWhisker(rightWhiskerBottom);
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cRight = true;
        side |= Right;

        Collision col;
        if (collidee != NULL && collidee != this) {
//            col = {this, collidee, collidee->getVelocity(), Left, Left, QPointF(0,0)};
            // CAN'T USE LINE ABOVE BECAUSE MSVC++ SUCKS!!!!  OR SHOULD I SAY, MS* SUCKS.
            col.firstSprite = this;
            col.secondSprite = collidee;
            col.normalVector = collidee->getVelocity();
            col.firstSide = Right;
            col.secondSide = Right;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Right;
            col.secondSide = Right;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
    }

    // Check bottom points for collision
    collidee = spriteWithinWhisker(bottomWhiskerRight);
    collidee2 = spriteWithinWhisker(bottomWhiskerLeft);
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cBottom = true;
        side |= Bottom;
        Collision col;
        if (collidee != NULL && collidee != this) {
//            col = {this, collidee, collidee->getVelocity(), Left, Left, QPointF(0,0)};
            // CAN'T USE LINE ABOVE BECAUSE MSVC++ SUCKS!!!!  OR SHOULD I SAY, MS* SUCKS.
            col.firstSprite = this;
            col.secondSprite = collidee;
            col.normalVector = collidee->getVelocity();
            col.firstSide = Bottom;
            col.secondSide = Bottom;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Bottom;
            col.secondSide = Bottom;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
    }

    // Check left points for collision
    collidee = spriteWithinWhisker(leftWhiskerBottom);
    collidee2 = spriteWithinWhisker(leftWhiskerTop);
    if ((collidee != NULL && collidee != this && collidee->isCollideable()) ||
            (collidee2 != NULL && collidee2 != this && collidee2->isCollideable())) {
        cLeft = true;
        side |= Left;

        Collision col;
        if (collidee != NULL && collidee != this) {
//            col = {this, collidee, collidee->getVelocity(), Left, Left, QPointF(0,0)};
            // CAN'T USE LINE ABOVE BECAUSE MSVC++ SUCKS!!!!  OR SHOULD I SAY, MS* SUCKS.
            col.firstSprite = this;
            col.secondSprite = collidee;
            col.normalVector = collidee->getVelocity();
            col.firstSide = Left;
            col.secondSide = Left;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Left;
            col.secondSide = Left;
            col.overlapDistance = QPointF(0, 0);
            collisions.append(col);
        }
    }

    if ((cTop && cBottom) || (cLeft && cRight)) qDebug() << "SQUISH -- " << side;

    return side;
}

Sprite* AnimatedCollideableSprite::spriteWithinWhisker(QRectF whisker, Side side) {
    QList<QGraphicsItem *> items = this->scene()->items(whisker, Qt::IntersectsItemShape, Qt::DescendingOrder, this->transform());

    // Following stolen from QGraphicsScene.cpp
    Sprite* a = (items.size() > 0) ? dynamic_cast<Sprite*>(items.at(0)) : 0;

    for (auto itr = items.begin(); itr != items.end(); itr++) {
        if (((Sprite*)(*itr))->isCollideable() && (*itr) != this) {
//            qDebug() << "This: " << this->className() << "Them: " << ((Sprite*)(*itr))->className();
            return dynamic_cast<Sprite*>(*itr);
        }
    }
    return 0;
}

void AnimatedCollideableSprite::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Override this method in your subclasses if you want to be alerted when collisions occur.        
}

