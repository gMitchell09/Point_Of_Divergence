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

enum MovementState { Stand_Right = 0, Stand_Left,
                     Walk_Right, Walk_Left,
                     Run_Right, Run_Left,
                     Jump_Right, Jump_Left,
                     Squat_Right, Squat_Left,
                     Brake_Right, Brake_Left};

class MainCharacter : public AnimatedCollideableSprite
{
private:
    MovementState m_currentState;
    // nextState is if you are running right and then pressed the left key... it is for mapping between states
//    MovementState m_nextState;

    int m_brakeAccel;
    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;

    bool m_upPressed, m_downPressed, m_leftPressed, m_rightPressed;

    bool m_jumping, m_jumping_double;
    bool m_brake;

    int m_keyRecentPress;

public:
    explicit MainCharacter(int width, int height, QGraphicsItem *parent = 0);
    void keyPressEvent(QKeyEvent * keyEvent);
    void keyReleaseEvent(QKeyEvent * keyEvent);

    void setBrake(bool brake) { m_brake = brake; }

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);
    void jump();

    virtual QString className() { return "MainCharacter"; }

protected:
    virtual bool usesStack() { return true; }

};

#endif // MAINCHARACTER_H
