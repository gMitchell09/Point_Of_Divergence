/* Author: George Mitchell
 * Use Case 02: Update Object Positions
 * Use Case: 03: Collide With Object
 */

#include "animatedcollideablesprite.h"
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
    m_collisionPoints[0][0] = QPointF(width/3, 0);
    m_collisionPoints[0][1] = QPointF((2*width)/3, 0);
    // Right
    m_collisionPoints[1][0] = QPointF(width, height/3);
    m_collisionPoints[1][1] = QPointF(width, (2*height)/3);
    // Bottom
    m_collisionPoints[2][0] = QPointF(width/3, height);
    m_collisionPoints[2][1] = QPointF((2*width)/3, height);
    // Left
    m_collisionPoints[3][0] = QPointF(0, height/3);
    m_collisionPoints[3][1] = QPointF(0, (2*height)/3);
}

void AnimatedCollideableSprite::updateWhiskers(QPointF offset) {
    QSizeF bottomWhiskerSize = QSizeF(2, MAX(abs(offset.y()), MIN_WHISKER));
    QSizeF rightWhiskerSize = QSizeF(MAX(abs(offset.x()), MIN_WHISKER_LR), 2);
    QSizeF topWhiskerSize = QSizeF(2, -MAX(abs(offset.y()), MIN_WHISKER));
    QSizeF leftWhiskerSize = QSizeF(-MAX(abs(offset.x()), MIN_WHISKER_LR), 2);

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

    QRectF tl(m_collisionPoints[0][0] + this->pos() - QPointF(MIN_WHISKER/2, 0), topWhiskerSize),
            tr(m_collisionPoints[0][1] + this->pos() - QPointF(MIN_WHISKER/2, 0), topWhiskerSize),
            rt(m_collisionPoints[1][0] + this->pos() - QPointF(0, MIN_WHISKER/2), rightWhiskerSize),
            rb(m_collisionPoints[1][1] + this->pos() - QPointF(0, MIN_WHISKER/2), rightWhiskerSize),
            br(m_collisionPoints[2][0] + this->pos() - QPointF(MIN_WHISKER/2, 0), bottomWhiskerSize),
            bl(m_collisionPoints[2][1] + this->pos() - QPointF(MIN_WHISKER/2, 0), bottomWhiskerSize),
            lb(m_collisionPoints[3][1] + this->pos() - QPointF(0, MIN_WHISKER/2), leftWhiskerSize),
            lt(m_collisionPoints[3][0] + this->pos() - QPointF(0, MIN_WHISKER/2), leftWhiskerSize);

    // Translate to center of rectangles
    tr.translate(tr.width()/2, tr.height()/2);
    tl.translate(tl.width()/2, tl.height()/2);

    rt.translate(rt.width()/2, rt.height()/2);
    rb.translate(rb.width()/2, rb.height()/2);

    br.translate(br.width()/2, br.height()/2);
    bl.translate(bl.width()/2, bl.height()/2);

    lb.translate(lb.width()/2, lb.height()/2);
    lt.translate(lt.width()/2, lt.height()/2);

    // Rotate
    topWhiskerLeft = this->transform().map(QPolygonF(tl));
    topWhiskerRight = this->transform().map(QPolygonF(tr));

    rightWhiskerTop = this->transform().map(QPolygonF(rt));
    rightWhiskerBottom = this->transform().map(QPolygonF(rb));

    bottomWhiskerRight = this->transform().map(QPolygonF(br));
    bottomWhiskerLeft = this->transform().map(QPolygonF(bl));

    leftWhiskerBottom = this->transform().map(QPolygonF(lb));
    leftWhiskerTop = this->transform().map(QPolygonF(lt));

    // Now to translate back to our origin
    topWhiskerLeft.translate(-tr.width()/2, -tr.height()/2);
    topWhiskerRight.translate(-tl.width()/2, -tl.height()/2);

    rightWhiskerTop.translate(-rt.width()/2, -rt.height()/2);
    rightWhiskerBottom.translate(-rb.width()/2, -rb.height()/2);

    bottomWhiskerRight.translate(-br.width()/2, -br.height()/2);
    bottomWhiskerLeft.translate(-bl.width()/2, -bl.height()/2);

    leftWhiskerBottom.translate(-lb.width()/2, -lb.height()/2);
    leftWhiskerTop.translate(-lt.width()/2, -lt.height()/2);

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
    }

    else {
        double timeStep = delta / 1000.0;
        QList<Collision> collisions;

        QPointF oldVel = m_velocity;
        QPointF newVel;
        QPointF angularVelocity = QPointF(1, 1);
        QPointF relativeVel = QPointF(0, 0);

        m_velocity += m_acceleration * timeStep;
        newVel = m_velocity;

        bool wasOnSlope = (this->isOnLeftSlope() || this->isOnRightSlope());

        if (!this->isStatic()) {
            unsigned char side = this->checkForCollision(collisions, (m_velocity + oldVel) * 0.5 * timeStep);

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

                                        angularVelocity.setX(cos(M_PI * 45.0 / 180));
                                        angularVelocity.setY(sin(M_PI * 45.0 / 180));
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
                                        angularVelocity.setX(cos(M_PI * 30.0 / 180));
                                        angularVelocity.setY(sin(M_PI * 30.0 / 180));
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
                                        angularVelocity.setX(cos(M_PI * 30.0 / 180));
                                        angularVelocity.setY(sin(M_PI * 30.0 / 180));
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
                                        angularVelocity.setX(cos(M_PI * 45.0 / 180));
                                        angularVelocity.setY(sin(M_PI * 45.0 / 180));
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
                                        angularVelocity.setX(cos(M_PI * 30.0 / 180));
                                        angularVelocity.setY(sin(M_PI * 30.0 / 180));
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
                                        angularVelocity.setX(cos(M_PI * 30.0 / 180));
                                        angularVelocity.setY(sin(M_PI * 30.0 / 180));
                                        break;
                                }
                                this->setPos(this->x(), second->pos().y() - this->boundingRect().height() + second->boundingRect().height() - lut_value);
                            }
                        } else if (side & Bottom) m_onLeftSlope = m_onRightSlope = false;

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
                            relativeVel.setX(col.secondSprite->getApparentVelocity().x());
                        }
                        if ((this->pos().y() <= col.secondSprite->pos().y() && col.normalVector.y() < 0) ||
                            (this->pos().y() >= col.secondSprite->pos().y() && col.normalVector.y() > 0)) {
                            //newPos.setY(newPos.y() + ((Collision)(*itr)).normalVector.y() * timeStep);
                            relativeVel.setY(col.secondSprite->getApparentVelocity().y());
                            qDebug() << "Normal Vector: " << ((Collision)(*itr)).normalVector.y();
                        }

                        if (((locSide == Top) && m_velocity.y() < 0) || ((locSide == Bottom) && m_velocity.y() > 0)) newVel.setY(0);

                        //  If we aren't on a slope then set our y-velocity to zero since we are standing on a solid platform
                        if (!second->isSlope() &&
                                !(this->isOnLeftSlope() || this->isOnRightSlope() || wasOnSlope) &&
                                // !(second->isOnLeftSlope() || second->isOnRightSlope()) &&
                                (((locSide == Left) && m_velocity.x() < 0) || ((locSide == Right) && m_velocity.x() > 0))) {
                            newVel.setX(0);
                            if (this->className() == "MainCharacter") {
                                qDebug() << "On Slope: " << ((this->isOnLeftSlope() || this->isOnRightSlope() || wasOnSlope) ? "YES" : "NO");
                                qDebug() << "velocity: " << this->getVelocity();
                                qDebug() << "Side: " << side << " Loc Side: " << locSide;
                            }
                        }

                    }
                }
                this->setPos(this->pos() + newPos);
            }

            m_velocity = newVel;

            QPointF tmpVel(angularVelocity.x() * ((m_velocity + oldVel) * 0.5 + relativeVel).x(),
                           angularVelocity.y() * ((m_velocity + oldVel) * 0.5 + relativeVel).y());
            this->setPos(this->pos() + tmpVel * timeStep);
            m_apparentVelocity = (m_velocity + oldVel) * 0.5 + relativeVel;
        }
    }
}

unsigned char AnimatedCollideableSprite::checkForCollision(QList<Collision>& collisions, QPointF offset) {
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
            if (!spr) continue;
            if (spr->isSolid()) side |= Top;
            if (spr != this) {
    //            col = {this, collidee, collidee->getVelocity(), Left, Left, QPointF(0,0)};
                // CAN'T USE LINE ABOVE BECAUSE MSVS SUCKS!!!!  OR SHOULD I SAY, MS* SUCKS.
                QPointF bottomRight = spr->mapToScene(spr->boundingRect().bottomRight());
                //float penetrationDepth = bottomRight.y() - topWhiskerLeft.top();
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

        if (isCol && isCol2 && solidCollisionList.size() > 1) {
            if (solidCollisionList.at(0)->blockType() != solidCollisionList.at(1)->blockType()) {
                //qDebug() << "UH-OH" << solidCollisionList.at(0)->blockType() << " : " << solidCollisionList.at(1)->blockType();
            }
        }

        for (auto itr = solidCollisionList.begin(); itr != solidCollisionList.end(); itr++) {
            Sprite* spr = dynamic_cast<Sprite*>(*itr);
            if (!spr) continue;
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
            if (!spr) continue;
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
            if (!spr) continue;
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

bool AnimatedCollideableSprite::spriteWithinWhisker(QPolygonF whisker, QList<Sprite *> &collisions) {
    QList<QGraphicsItem *> items = this->scene()->items(whisker, Qt::IntersectsItemShape, Qt::DescendingOrder, this->transform());

    for (auto itr = items.begin(); itr != items.end(); itr++) {
        Sprite *spr = dynamic_cast<Sprite*>(*itr);
        if (spr != NULL && spr->isCollideable() && spr != this) {
            collisions.push_back(spr);
        }
    }
    return !items.empty();
}

void AnimatedCollideableSprite::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    // Override this method in your subclasses if you want to be alerted when collisions occur.
    Q_UNUSED(collisions);
    Q_UNUSED(side);
}

void AnimatedCollideableSprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    AnimatedSprite::paint(painter, option, widget);
//    painter->setPen(Qt::red);
//    painter->setBrush(QColor(Qt::red));

//    painter->translate(-this->pos());

//    painter->drawPolygon(topWhiskerLeft);
//    painter->drawPolygon(topWhiskerRight);

//    painter->drawPolygon(rightWhiskerTop);
//    painter->drawPolygon(rightWhiskerBottom);

//    painter->drawPolygon(bottomWhiskerRight);
//    painter->drawPolygon(bottomWhiskerLeft);

//    painter->drawPolygon(leftWhiskerBottom);
//    painter->drawPolygon(leftWhiskerTop);

}
