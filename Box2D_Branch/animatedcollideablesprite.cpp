/* Author: George Mitchell
* Use Case 02: Update Object Positions
* Use Case: 03: Collide With Object
*/

#include "animatedcollideablesprite.h"
#include "level.h"

#define DEGREE_TO_RADIAN_FACTOR 57.295779513082320876798154814105

AnimatedCollideableSprite::AnimatedCollideableSprite(int width, int height, b2Body* body, QGraphicsItem *parent) :
   AnimatedSprite(width, height, parent),
   m_body(body) { }

void AnimatedCollideableSprite::step(qint64 time, long delta) {
   AnimatedSprite::step(time, delta);

   bool timeReversed = false;

   if (delta < 0) {
       //time = time;
       timeReversed = true;
   }

   if (timeReversed && this->usesStack()) {
   }

   else {
       if (m_body != nullptr) {
           b2Vec2 position = m_body->GetPosition();
           float32 angle = m_body->GetAngle();
           this->setPos(M_TO_PX(position.x) + this->pixmap().width()/2, M_TO_PX(-position.y));
           this->setTransformOriginPoint(0, 0);
           this->setRotation(-(angle * DEGREE_TO_RADIAN_FACTOR));

           for (b2ContactEdge* edge = m_body->GetContactList(); edge; edge = edge->next) {
               if (edge->contact->IsTouching()) {
                   if (edge->contact->GetFixtureA()->GetBody()->GetUserData() != nullptr && edge->contact->GetFixtureB()->GetBody()->GetUserData() != nullptr) {
                       AnimatedCollideableSprite *obj1, *obj2;
                       obj1 = (AnimatedCollideableSprite*)edge->contact->GetFixtureA()->GetBody()->GetUserData();
                       obj2 = (AnimatedCollideableSprite*)edge->contact->GetFixtureB()->GetBody()->GetUserData();
                       if (obj1 && obj2 && (obj1->className() == "MainCharacter" || obj2->className() == "MainCharacter")) {
                            qDebug() << "Collision between: " << obj1->className() << " and " << obj2->className();
                       }
                   }
               }
           }
       }
   }
}

void AnimatedCollideableSprite::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
   // Override this method in your subclasses if you want to be alerted when collisions occur.
   Q_UNUSED(collisions);
   Q_UNUSED(side);
}

void AnimatedCollideableSprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    AnimatedSprite::paint(painter, option, widget);
    if (m_body) {
        b2Fixture *fix = m_body->GetFixtureList();
        while(fix) {
            QRect aabb;
            b2AABB b2aabb = fix->GetAABB(0);
            aabb.setBottomLeft(QPoint(M_TO_PX(b2aabb.lowerBound.x), M_TO_PX(-b2aabb.lowerBound.y)));
            aabb.setTopRight(QPoint(M_TO_PX(b2aabb.upperBound.x), M_TO_PX(-b2aabb.upperBound.y)));

            painter->setPen(Qt::red);
            painter->setBrush(QColor(Qt::red));

            painter->translate(-this->pos());
            painter->drawRect(aabb);
            fix = fix->GetNext();

//            qDebug() << this->className()
//                     << "Rect: " << aabb;

//            qDebug() << "Lower: " << QPoint(M_TO_PX(b2aabb.lowerBound.x), M_TO_PX(-b2aabb.lowerBound.y));
//            qDebug() << "Upper: " << QPoint(M_TO_PX(b2aabb.upperBound.x), M_TO_PX(-b2aabb.upperBound.y));
        }
    }
}
