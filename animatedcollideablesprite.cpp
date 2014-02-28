/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedcollideablesprite.h"
#include "sprite.h"
#include "level.h"

double LUT_SLOPE45[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
double LUT_SLOPE30[32] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16};
double LUT_SLOPE60[32] = {17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32};
#define SLOPE45_VERTICAL_OFFSET 4
#define SLOPE30_VERTICAL_OFFSET 2
#define SLOPE60_VERTICAL_OFFSET 2

#define MIN_WHISKER 1.
#define MIN_WHISKER_LR 4.

AnimatedCollideableSprite::AnimatedCollideableSprite(int width, int height, QGraphicsItem *parent) :
    AnimatedSprite(width, height, parent)
{
    m_onLeftSlope = m_onRightSlope = false;

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
    QSize bottomWhiskerSize = QSize(2, MAX(abs(offset.y()), MIN_WHISKER));
    QSize rightWhiskerSize = QSize(MAX(abs(offset.x()), MIN_WHISKER_LR), 2);
    QSize topWhiskerSize = QSize(2, -MAX(abs(offset.y()), MIN_WHISKER));
    QSize leftWhiskerSize = QSize(-MAX(abs(offset.x()), MIN_WHISKER_LR), 2);

    // For top: height = topBottomWhiskerSize * QSize(1, -1);
    // For left: width = leftRightWhiskerSize * QSize(-1, 1);

//    QPointF offsetPos = this->pos() + offset;

//    QPointF wideWhisker = QPointF(2, 0);
//    QPointF tallWhisker = QPointF(0, 2);

//    QPointF offsetX = QPointF(offsetPos.x(), offsetPos.y()) + tallWhisker;
//    QPointF offsetY = QPointF(offsetPos.x(), offsetPos.y()) + wideWhisker;

//    topWhiskerLeft = QRectF(m_collisionPoints[0][0] + offsetY, m_collisionPoints[0][0] + this->pos());
//    topWhiskerRight = QRectF(m_collisionPoints[0][1] + offsetY, m_collisionPoints[0][1] + this->pos());

//    rightWhiskerTop = QRectF(m_collisionPoints[1][0] + offsetX, m_collisionPoints[1][0] + this->pos());
//    rightWhiskerBottom = QRectF(m_collisionPoints[1][1] + offsetX, m_collisionPoints[1][1] + this->pos());

//    bottomWhiskerRight = QRectF(m_collisionPoints[2][0] + offsetY, m_collisionPoints[2][0] + this->pos());
//    bottomWhiskerLeft = QRectF(m_collisionPoints[2][1] + offsetY, m_collisionPoints[2][1] + this->pos());

//    leftWhiskerBottom = QRectF(m_collisionPoints[3][0] + offsetX, m_collisionPoints[3][0] + this->pos());
//    leftWhiskerTop = QRectF(m_collisionPoints[3][1] + offsetX, m_collisionPoints[3][1] + this->pos());

    topWhiskerLeft = QRectF(m_collisionPoints[0][0] + this->pos() - QPointF(MIN_WHISKER/2, 0), topWhiskerSize);
    topWhiskerRight = QRectF(m_collisionPoints[0][1] + this->pos() - QPointF(MIN_WHISKER/2, 0), topWhiskerSize);

    rightWhiskerTop = QRectF(m_collisionPoints[1][0] + this->pos() - QPointF(0, MIN_WHISKER/2), rightWhiskerSize);
    rightWhiskerBottom = QRectF(m_collisionPoints[1][1] + this->pos() - QPointF(0, MIN_WHISKER/2), rightWhiskerSize);

    bottomWhiskerRight = QRectF(m_collisionPoints[2][0] + this->pos() - QPointF(MIN_WHISKER/2, 0), bottomWhiskerSize);
    bottomWhiskerLeft = QRectF(m_collisionPoints[2][1] + this->pos() - QPointF(MIN_WHISKER/2, 0), bottomWhiskerSize);

    leftWhiskerBottom = QRectF(m_collisionPoints[3][1] + this->pos() - QPointF(0, MIN_WHISKER/2), leftWhiskerSize);
    leftWhiskerTop = QRectF(m_collisionPoints[3][0] + this->pos() - QPointF(0, MIN_WHISKER/2), leftWhiskerSize);
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

        bool wasOnSlope = (this->isOnLeftSlope() || this->isOnRightSlope());

        if (!this->isStatic()) {
            unsigned char side = this->checkForCollision(collisions, (m_velocity + oldVel) * 0.5 * timeStep, timeReversed);

            if (!collisions.empty()) {
                QPointF newPos = QPointF(0,0);
                this->collisionOccurred(collisions, side);
                for (auto itr = collisions.begin(); itr != collisions.end(); itr++) {

                    Collision col = ((Collision)(*itr));
                    AnimatedCollideableSprite *second = (AnimatedCollideableSprite*)(col).secondSprite;
                    //if (this->className() == "MainCharacter") qDebug() << "Time: " << time << "This: " << this->className() << " Other: " << second->blockType();
                    Side locSide = (*itr).firstSide;
                    if (this->className() == "MainCharacter" && second->className() == "Enemy1") {
                        qDebug() << "Time: " << time << "This Side: " << side << " Other Side: " << locSide;
                    }

                    if (this->isSolid() && second->isSolid()) {
                        if (second->isSlope()) {
                            if (side && Bottom) {
                                switch (second->blockType()) {
                                    case kSlope30Left: case kSlope45Left: case kSlope60Left:
                                        m_onLeftSlope = true;
                                        break;
                                    case kSlope30Right: case kSlope45Right: case kSlope60Right:
                                        m_onRightSlope = true;
                                        break;
                                }
                            }
                            if (m_velocity.y() >= 0) {
                                // We want the point that touches the slope to be the bottom-center of our player
                                int index = (int)(((this->pos().x() + 0.5*this->boundingRect().width()) - second->pos().x()));
                                double lut_value;
                                switch (second->blockType()) {
                                    case kSlope45Left:
                                        if (index >= 0 && index < 32) {
                                            lut_value = LUT_SLOPE45[32 - index] + SLOPE45_VERTICAL_OFFSET;
                                        }
                                        else if (index < 0) {
                                            lut_value = 33;
                                        } else {
                                            lut_value = 0;
                                        }
                                        break;
                                    case kSlope30Left:
                                        if (index >= 0 && index < 32) {
                                            lut_value = LUT_SLOPE30[32 - index] + SLOPE30_VERTICAL_OFFSET;
                                        }
                                        else if (index < 0) {
                                            lut_value = 17;
                                        } else {
                                            lut_value = 0;
                                        }
                                        break;
                                    case kSlope60Left:
                                        if (index >= 0 && index < 32) {
                                            lut_value = LUT_SLOPE60[32 - index] + SLOPE60_VERTICAL_OFFSET;
                                        }
                                        else if (index < 0) {
                                            lut_value = 33;
                                        } else {
                                            lut_value = 17;
                                        }
                                        break;
                                    case kSlope45Right:
                                        if (index >= 0 && index < 32) {
                                            lut_value = LUT_SLOPE45[index] + SLOPE45_VERTICAL_OFFSET;
                                        }
                                        else if (index < 0) {
                                            lut_value = 0;
                                        } else {
                                            lut_value = 33;
                                        }
                                        break;
                                    case kSlope30Right:
                                        if (index >= 0 && index < 32) {
                                            lut_value = LUT_SLOPE30[index] + SLOPE30_VERTICAL_OFFSET;
                                        }
                                        else if (index < 0) {
                                            lut_value = 0;
                                        } else {
                                            lut_value = 17;
                                        }
                                        break;
                                    case kSlope60Right:
                                        if (index >= 0 && index < 32) {
                                            lut_value = LUT_SLOPE60[index] + SLOPE60_VERTICAL_OFFSET;
                                        }
                                        else if (index < 0) {
                                            lut_value = 17;
                                        } else {
                                            lut_value = 33;
                                        }
                                        break;
                                }
                                this->setPos(this->x(), second->pos().y() - this->boundingRect().height() + second->boundingRect().height() - lut_value);
                            }
                        } else if (side && Bottom) m_onLeftSlope = m_onRightSlope = false;

                        switch (locSide) {
                            case Top:
                                this->setY(col.overlapDistance.y());
                                break;
                            case Bottom:
                                if (!second->isSlope()) this->setY(col.overlapDistance.y() - this->boundingRect().height() + 1);
                                break;
                            case Right:
                                // Only reposition the player if they are (not on a slope) and moving in the direction of the collision.  This prevents that weird
                                //   suction bug.
                                if (!second->isSlope() && this->getVelocity().x() > 0) this->setX(col.overlapDistance.x() - this->boundingRect().width() - 4);
                                break;
                            case Left:
                                // Only reposition the player if they are (not on a slope) and moving in the direction of the collision.  This prevents that weird
                                //   suction bug.
                                if (!second->isSlope() && this->getVelocity().x() < 0) this->setX(col.overlapDistance.x() + 4);
                                break;
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

                        if (((locSide == Top) && m_velocity.y() < 0) || ((locSide == Bottom) && m_velocity.y() > 0)) newVel.setY(0);

                        //  If we aren't on a slope then set our y-velocity to zero since we are standing on a solid platform
                        if (!second->isSlope() &&
                                !(this->isOnLeftSlope() || this->isOnRightSlope() || wasOnSlope) &&
                                // !(second->isOnLeftSlope() || second->isOnRightSlope()) &&
                                (((locSide == Left) && m_velocity.x() < 0) || ((locSide == Right) && m_velocity.x() > 0))) {
                            newVel.setX(0);
                            if (this->className() == "MainCharacter") {
                                qDebug() << "On Slope: " << (int)(this->isOnLeftSlope() || this->isOnRightSlope() || wasOnSlope);
                                qDebug() << "velocity: " << this->getVelocity().x();
                                qDebug() << "Side: " << side << " Loc Side: " << locSide;
                            }
                        }

                    }
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
    bool isCol, isCol2;

    QList<Sprite *> solidCollisionList;

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

    isCol = spriteWithinWhisker(topWhiskerLeft, solidCollisionList);
    isCol2 = spriteWithinWhisker(topWhiskerRight, solidCollisionList);

    if (isCol || isCol2) {
        cTop = true;

        Collision col;

        for (auto itr = solidCollisionList.begin(); itr != solidCollisionList.end(); itr++) {
            Sprite* spr = dynamic_cast<Sprite*>(*itr);
            if (spr->isSolid()) side |= Top;
            if (spr != this) {
    //            col = {this, collidee, collidee->getVelocity(), Left, Left, QPointF(0,0)};
                // CAN'T USE LINE ABOVE BECAUSE MSVS SUCKS!!!!  OR SHOULD I SAY, MS* SUCKS.
                QPointF bottomRight = spr->mapToScene(spr->boundingRect().bottomRight());
                float penetrationDepth = bottomRight.y() - topWhiskerLeft.top();
                //qDebug() << "penetrationDepth: " << penetrationDepth;
                col.firstSprite = this;
                col.secondSprite = spr;
                col.normalVector = spr->getVelocity();
                col.firstSide = Top;
                col.secondSide = Top;
                col.overlapDistance = spr->mapToScene(0, spr->boundingRect().height());
                collisions.append(col);
            }
        }
        solidCollisionList.clear();
    }

    // Check bottom points for collision
    isCol = spriteWithinWhisker(bottomWhiskerRight, solidCollisionList);
    isCol2 = spriteWithinWhisker(bottomWhiskerLeft, solidCollisionList);

    if (isCol || isCol2) {
        cBottom = true;
        Collision col;

        for (auto itr = solidCollisionList.begin(); itr != solidCollisionList.end(); itr++) {
            Sprite* spr = dynamic_cast<Sprite*>(*itr);
            if (spr->isSolid()) side |= Bottom;
            if (spr != this) {
                col.firstSprite = this;
                col.secondSprite = spr;
                col.normalVector = spr->getVelocity();
                col.firstSide = Bottom;
                col.secondSide = Bottom;
                col.overlapDistance = spr->mapToScene(QPointF(0, 0));
                collisions.append(col);
            }
        }
        solidCollisionList.clear();
    }

    // Check right points for collision
    isCol = spriteWithinWhisker(rightWhiskerTop, solidCollisionList);
    isCol2 = spriteWithinWhisker(rightWhiskerBottom, solidCollisionList);
    if (isCol || isCol2) {
        cRight = true;

        Collision col;

        for (auto itr = solidCollisionList.begin(); itr != solidCollisionList.end(); itr++) {
            Sprite* spr = dynamic_cast<Sprite*>(*itr);
            if (spr->isSolid()) side |= Right;
            if (spr != this) {
                col.firstSprite = this;
                col.secondSprite = spr;
                col.normalVector = spr->getVelocity();
                col.firstSide = Right;
                col.secondSide = Right;
                col.overlapDistance = spr->mapToScene(QPointF(0, 0));
                collisions.append(col);
            }
        }
        solidCollisionList.clear();
    }

    // Check left points for collision
    isCol = spriteWithinWhisker(leftWhiskerBottom, solidCollisionList);
    isCol2 = spriteWithinWhisker(leftWhiskerTop, solidCollisionList);

    if (isCol || isCol2) {
        cLeft = true;

        Collision col;

        for (auto itr = solidCollisionList.begin(); itr != solidCollisionList.end(); itr++) {
            Sprite* spr = dynamic_cast<Sprite*>(*itr);
            if (spr->isSolid()) side |= Left;
            if (spr != this) {
                col.firstSprite = this;
                col.secondSprite = spr;
                col.normalVector = spr->getVelocity();
                col.firstSide = Left;
                col.secondSide = Left;
                col.overlapDistance = spr->mapToScene(QPointF(spr->boundingRect().width(), 0));
                collisions.append(col);
            }
        }
        solidCollisionList.clear();
    }

//    if ((cTop && cBottom) || (cLeft && cRight)) qDebug() << "SQUISH -- " << side;

    return side;
}

bool AnimatedCollideableSprite::spriteWithinWhisker(QRectF whisker, QList<Sprite *> &collisions) {
    QList<QGraphicsItem *> items = this->scene()->items(whisker, Qt::IntersectsItemShape, Qt::DescendingOrder, this->transform());

    for (auto itr = items.begin(); itr != items.end(); itr++) {
        if (((Sprite*)(*itr))->isCollideable() && (*itr) != this) {
            collisions.push_back(dynamic_cast<Sprite*>(*itr));
        }
    }
    return !items.empty();
}

void AnimatedCollideableSprite::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Override this method in your subclasses if you want to be alerted when collisions occur.        
}

