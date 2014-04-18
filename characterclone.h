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
    explicit CharacterClone(int width, int height, std::vector<State> stateStack, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta) {
        AnimatedCollideableSprite::step(time, delta);
    }
    virtual void collisionOccurred(Sprite* other, Side side) { Q_UNUSED(other); Q_UNUSED(side); }

    virtual QString className() { return "CharacterClone"; }

    bool isStatic() { return true; }
    bool isAnimated() { return true; }
    bool isCollideable() { return true; }
    bool isBackground() { return false; }

    // Mute these methods so we can use our custom stack
    virtual void pushState(qint64 time, long delta, State& state) {
        Q_UNUSED(time); Q_UNUSED(delta); Q_UNUSED(state);
        ++m_sliceIndex;

        if (m_sliceIndex >= m_stateStack.size()) m_sliceIndex = 0;

        State currentState = m_stateStack.at(m_sliceIndex);
        this->setState(currentState);
    }

    virtual void popState(qint64 time, long delta) {
        Q_UNUSED(time);
        Q_UNUSED(delta);
        --m_sliceIndex;

        if (m_sliceIndex < 0) m_sliceIndex = m_stateStack.size() - 1;

        State currentState = m_stateStack.at(m_sliceIndex);
        this->setState(currentState);
    }

    virtual void setState(State currentState) {
        AnimatedCollideableSprite::setState(currentState);
    }

protected:
    virtual bool usesStack() { return true; }

};

#endif // CHARACTERCLONE_H
