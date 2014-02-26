/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedcollideablesprite.h"
#include "sprite.h"
#include "level.h"

double LUT_SLOPE45[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
double LUT_SLOPE30[32] = {0.5773502691896256, 1.1547005383792512, 1.7320508075688767, 2.3094010767585025, 2.886751345948128, 3.4641016151377535, 4.04145188432738, 4.618802153517005, 5.19615242270663, 5.773502691896256, 6.350852961085882, 6.928203230275507, 7.505553499465133, 8.08290376865476, 8.660254037844384, 9.23760430703401, 9.814954576223636, 10.39230484541326, 10.969655114602887, 11.547005383792513, 12.124355652982137, 12.701705922171763, 13.27905619136139, 13.856406460551014, 14.43375672974064, 15.011106998930266, 15.588457268119893, 16.16580753730952, 16.74315780649914, 17.320508075688767, 17.897858344878394, 18.47520861406802};
double LUT_SLOPE60[32] = {19.052558883257646, 19.629909152447272, 20.2072594216369, 20.78460969082652, 21.361959960016147, 21.939310229205773, 22.5166604983954, 23.094010767585026, 23.671361036774652, 24.248711305964274, 24.8260615751539, 25.403411844343527, 25.980762113533153, 26.55811238272278, 27.135462651912405, 27.712812921102028, 28.290163190291654, 28.86751345948128, 29.444863728670907, 30.022213997860533, 30.59956426705016, 31.176914536239785, 31.754264805429408, 32.33161507461904, 32.90896534380866, 33.48631561299828, 34.06366588218791, 34.641016151377535, 35.218366420567165, 35.79571668975679, 36.37306695894642, 36.95041722813604};


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

                    Collision col = ((Collision)(*itr));
                    AnimatedCollideableSprite *first = (AnimatedCollideableSprite*)(col).firstSprite;
                    AnimatedCollideableSprite *second = (AnimatedCollideableSprite*)(col).secondSprite;
                    Side locSide = (*itr).firstSide;

                    if (this->isSolid() && second->isSolid()) {
                        if (1 || !second->isSlope()) {
                            if (((locSide == Top) && m_velocity.y() < 0) || ((locSide == Bottom) && m_velocity.y() > 0)) newVel.setY(0);
                            if (!second->isSlope() && (((locSide == Left) && m_velocity.x() < 0) || ((locSide == Right) && m_velocity.x() > 0))) newVel.setX(0);

                            if (second->isSlope()) {
                                /* Picture to help:
                                 *        _______
                                 *      M/
                                 * ____/
                                 */
                            }

                            switch (locSide) {
                                case Top:
                                    this->setY(col.overlapDistance.y());
                                    break;
                                case Bottom:
                                    this->setY(col.overlapDistance.y() - this->boundingRect().height()+1);
                                    break;
                                case Right:
                                    if (!second->isSlope()) this->setX(col.overlapDistance.x() - this->boundingRect().width());
                                    break;
                                case Left:
                                    qDebug() << "Left";
                                    // EUREKA MOMENT!!! THERE ISN'T A LEFT COLLISION IF MARIO ISN'T MOVING LEFT!!!  Need minimum whisker length.
                                    if (!second->isSlope()) this->setX(col.overlapDistance.x());
                                    break;
                            }

                        } else {
                            if (second->is30()) qDebug() << "30";
                            else if (second->is60()) qDebug() << "60";
                            else if (second->is45()) qDebug() << "45";
                        }

                        if ((side & Bottom) || (this->pos().x() <= col.secondSprite->pos().x() && col.normalVector.x() < 0) ||
                            (this->pos().x() >= col.secondSprite->pos().x() && col.normalVector.x() > 0)) {
                            //newPos.setX(newPos.x() + ((Collision)(*itr)).normalVector.x() * timeStep);
                            relativeVel.setX(col.secondSprite->getVelocity().x());
                        }
                        if ((this->pos().y() <= col.secondSprite->pos().y() && col.normalVector.y() < 0) ||
                            (this->pos().y() >= col.secondSprite->pos().y() && col.normalVector.y() > 0)) {
                            //newPos.setY(newPos.y() + ((Collision)(*itr)).normalVector.y() * timeStep);
                            relativeVel.setY(col.secondSprite->getVelocity().y());
//                            qDebug() << "Normal Vector: " << ((Collision)(*itr)).normalVector.y();
                        }
                    }
                    //newPos += ((Collision)(*itr)).normalVector * timeStep;
                }
                this->setPos(this->pos() + newPos);
            }

            m_velocity = newVel;

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
            col.overlapDistance = collidee->mapToScene(0, collidee->boundingRect().height());
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Top;
            col.secondSide = Top;
            col.overlapDistance = collidee2->mapToScene(0, collidee2->boundingRect().height());
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
            col.overlapDistance = collidee->mapToScene(QPointF(0, 0));
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Right;
            col.secondSide = Right;
            col.overlapDistance = collidee2->mapToScene(QPointF(0, 0));
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
            col.overlapDistance = collidee->mapToScene(QPointF(0, 0));
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Bottom;
            col.secondSide = Bottom;
            col.overlapDistance = collidee2->mapToScene(QPointF(0, 0));
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
            col.overlapDistance = collidee->mapToScene(QPointF(collidee->boundingRect().width(), 0));
            collisions.append(col);
        }
        if (collidee2 != NULL && collidee2 != collidee && collidee2 != this) {
            col.firstSprite = this;
            col.secondSprite = collidee2;
            col.normalVector = collidee2->getVelocity();
            col.firstSide = Left;
            col.secondSide = Left;
            col.overlapDistance = collidee2->mapToScene(QPointF(collidee2->boundingRect().width(), 0));
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

