 #ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

/* Author: George Mitchell
 * Use Case: *
 */

#include <QKeyEvent>
#include <QDebug>

#include "tilemap.h"

#include "animatedcollideablesprite.h"
#include "global.h"
#include "gameengine.h"
#include "networkmanager.h"

class MainCharacter : public AnimatedCollideableSprite
{
private:
    enum MovementState { Stand_Right = 0, Stand_Left,
                         Walk_Right, Walk_Left,
                         Run_Right, Run_Left,
                         Jump_Right, Jump_Left,
                         Climb };

    MovementState m_currentState { Stand_Right };
    // nextState is if you are running right and then pressed the left key... it is for mapping between states
//    MovementState m_nextState;

    const float32 m_ladderClimbSpeed { 5 };
    const float32 m_maxSpeed { 10 };

    bool m_upPressed { false }, m_downPressed { false }, m_leftPressed { false }, m_rightPressed { false };
    bool m_jumping { false }, m_jumping_double { false };
    bool m_isOnLadder { false };
    int m_keyRecentPress { 0 };

public:
    explicit MainCharacter(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);
    void keyPressEvent(QKeyEvent * keyEvent);
    void keyReleaseEvent(QKeyEvent * keyEvent);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(Sprite *sprite, Side side);
    void jump();
    void climbLadder(int dir = 1);

    virtual QString className() { return "MainCharacter"; }

    void setB2DPosition(const QPointF &pos) {
        if (m_body) {
            m_body->SetTransform(b2Vec2(PX_TO_M(pos.x()), PX_TO_M(-pos.y())), 0);
        }
    }

    void setB2DPosition(qreal x, qreal y) {
        if (m_body) {
            m_body->SetTransform(b2Vec2(PX_TO_M(x), PX_TO_M(-y)), 0);
        }
    }

protected:
    virtual bool usesStack() { return true; }

};

#endif // MAINCHARACTER_H
