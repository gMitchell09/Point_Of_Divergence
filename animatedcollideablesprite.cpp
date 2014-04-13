/* Author: George Mitchell
* Use Case 02: Update Object Positions
* Use Case: 03: Collide With Object
*/

#include "animatedcollideablesprite.h"
#include "level.h"

#define RADIAN_TO_DEGREE_FACTOR 57.295779513082320876798154814105

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
           this->updatePosition();

           for (b2ContactEdge* edge = m_body->GetContactList(); edge; edge = edge->next) {
               if (edge->contact->IsTouching()) {
                   if (edge->contact->GetFixtureA()->GetBody()->GetUserData() != nullptr && edge->contact->GetFixtureB()->GetBody()->GetUserData() != nullptr) {
                       AnimatedCollideableSprite *obj1, *obj2;
                       obj1 = (AnimatedCollideableSprite*)edge->contact->GetFixtureA()->GetBody()->GetUserData();
                       obj2 = (AnimatedCollideableSprite*)edge->contact->GetFixtureB()->GetBody()->GetUserData();
                       if (obj1 && obj2) {
                           if (obj1 == this || obj2 == this) {
                                Side side = (Side)0;

                                // we force world normal to always point from this to
                                //  the other object
                                b2Vec2 worldNormal;
                                Sprite *otherObject;

                                if (this == obj1) {
                                    otherObject = obj2;
                                    b2WorldManifold worldManifold;
                                    edge->contact->GetWorldManifold(&worldManifold);

                                    worldNormal = worldManifold.normal;

                                } else {
                                    otherObject = obj1;
                                    b2WorldManifold worldManifold;
                                    edge->contact->GetWorldManifold(&worldManifold);

                                    worldNormal = -worldManifold.normal;
                                }

                                if (worldNormal.x > 0.707) { // Collision on Right
                                    side = Right;
                                } else if (worldNormal.x < -0.707) { // Collision on Left
                                    side = Left;
                                } else if (worldNormal.y > 0.707) { // collision on Top
                                    side = Top;
                                } else if (worldNormal.y < -0.707) { // collision on Bottom
                                    side = Bottom;
                                }

                                this->collisionOccurred(otherObject, side);
                           }
                       } // Blocks?
                   } // Nested
               } // Enough
           } // Have
       } // I
    } // Do
}

void AnimatedCollideableSprite::updatePosition() {
    if (m_body != nullptr) {
        b2Vec2 position = m_body->GetPosition();
        float32 angle = m_body->GetAngle(); //M_PI/2;
        this->setPos(M_TO_PX(position.x) + this->pixmap().width()/2, M_TO_PX(-position.y));
        this->setTransformOriginPoint(-this->pixmap().width()/2, 0);
        this->setRotation(-((angle) * RADIAN_TO_DEGREE_FACTOR));
    }
}

void AnimatedCollideableSprite::collisionsOccurred(QList<Collision> &collisions, unsigned char side) {
   // Override this method in your subclasses if you want to be alerted when collisions occur.
    Q_UNUSED(collisions);
    Q_UNUSED(side);
}

void AnimatedCollideableSprite::collisionOccurred(Sprite* other, Side side) {
    Q_UNUSED(other);
    Q_UNUSED(side);
}

void AnimatedCollideableSprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    AnimatedSprite::paint(painter, option, widget);
    if (0 && m_body) {
        b2Fixture *fix = m_body->GetFixtureList();
        while(fix) {
            QPolygon poly;
            if (fix->GetShape()->GetType() == b2Shape::e_polygon) {
                b2PolygonShape *shape = static_cast<b2PolygonShape*>(fix->GetShape());
                for (int i = 0; i < shape->GetVertexCount(); i++) {
                    b2Vec2 vertex = shape->GetVertex(i);
                    poly << QPoint(M_TO_PX(vertex.x), M_TO_PX(-vertex.y));
                }
            }

            painter->setPen(Qt::red);
            painter->setBrush(QColor(Qt::red));

            painter->translate(-this->pixmap().width()/2, 0);
            painter->drawPolygon(poly);
            fix = fix->GetNext();
        }
    }
}

AnimatedCollideableSprite::~AnimatedCollideableSprite() {
    qDebug() << "Deleting " << this->className();
    if (m_body) {
        m_body->GetWorld()->DestroyBody(m_body); // Ouch!
    }
}
