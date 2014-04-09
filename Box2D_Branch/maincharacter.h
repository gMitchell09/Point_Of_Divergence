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
                         Squat_Right, Squat_Left,
                         Brake_Right, Brake_Left};

    MovementState m_currentState;
    // nextState is if you are running right and then pressed the left key... it is for mapping between states
//    MovementState m_nextState;

    float m_brakeAccel, m_brakeAccelSliding;
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity, m_ladderClimbSpeed;
    float m_boxPushVelocity, m_boxPushAcceleration;

    bool m_upPressed, m_downPressed, m_leftPressed, m_rightPressed;

    bool m_jumping, m_jumping_double;
    bool m_brake;

    bool m_isOnLadder;

    int m_keyRecentPress;

public:
    explicit MainCharacter(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);
    void keyPressEvent(QKeyEvent * keyEvent);
    void keyReleaseEvent(QKeyEvent * keyEvent);

    void setBrake(bool brake) { m_brake = brake; }

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);
    void jump();
    void climbLadder(int dir = 1);

    virtual QString className() { return "MainCharacter"; }

protected:
    virtual bool usesStack() { return true; }

};

#endif // MAINCHARACTER_H