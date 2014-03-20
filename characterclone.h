#ifndef CHARACTERCLONE_H
#define CHARACTERCLONE_H

#include "animatedcollideablesprite.h"

class CharacterClone : public AnimatedCollideableSprite
{
private:
    enum MovementState { Stand_Right = 0, Stand_Left,
                         Walk_Right, Walk_Left,
                         Run_Right, Run_Left,
                         Jump_Right, Jump_Left,
                         Squat_Right, Squat_Left,
                         Brake_Right, Brake_Left};

    MovementState m_currentMovementState;

public:
    explicit CharacterClone(int width, int height, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta) {}
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side) {}

    virtual QString className() { return "CharacterClone"; }

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }

protected:
    virtual bool usesStack() { return false; }

};

#endif // CHARACTERCLONE_H
