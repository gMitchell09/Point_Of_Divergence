#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include <QKeyEvent>
#include "tilemap.h"

#include "animatedcollideablesprite.h"

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
    MovementState m_nextState;
    bool m_jumping;

    int m_keyRecentPress;

public:
    explicit MainCharacter(int width, int height, QGraphicsItem *parent = 0);
    void keyPressEvent(QKeyEvent * keyEvent);
    void keyReleaseEvent(QKeyEvent * keyEvent);

    virtual void step(unsigned long time);
};

#endif // MAINCHARACTER_H
