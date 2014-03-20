#ifndef SPRITE_H
#define SPRITE_H

/* Author: George Mitchell
 * Use Case: *
 */

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

#include "global.h"
#include "networkmanager.h"

///
/// \brief The Sprite class is an abstract base class for all other sprites in engine
///

enum Side { Top = 1, Right = 2, Bottom = 4, Left = 8 };

class Sprite : public QGraphicsPixmapItem
{
private:
    bool m_solid;

    void beginSlice();
    void endSlice();

protected:
    QPointF m_acceleration, m_velocity, m_apparentVelocity;
    struct State { char m_nCurrentFrame, m_nCurrentAnimation; QPointF pos; qint64 timestamp; };
    virtual bool usesStack() { return false; }

    std::vector<State> m_stateStack;
    std::vector<State> m_stateSlice;

    bool m_useSlice;

    size_t m_sliceBegin, m_sliceEnd, m_sliceIndex;

public:
    explicit Sprite(QGraphicsItem *parent = 0);

    ///
    /// \brief isStatic
    /// \return true if sprite doesn't respond to changes in velocity and acceleration
    ///
    virtual bool isStatic() { return false; }

    ///
    /// \brief isAnimated
    /// \return true if sprite has multiple frames
    ///
    virtual bool isAnimated() { return false; }

    ///
    /// \brief isCollideable
    /// \return true if we need to perform collision checking on this sprite
    ///
    virtual bool isCollideable() { return false; }

    ///
    /// \brief isBackground
    /// \return true if this sprite is a background
    ///
    virtual bool isBackground() { return false; }

    void setAcceleration(QPointF acceleration) { m_acceleration = acceleration; }
    QPointF& getAcceleration() { return m_acceleration; }

    void setVelocity(QPointF velocity) { m_velocity = velocity; }
    QPointF& getVelocity() { return m_velocity; }

    QPointF& getApparentVelocity() { return m_apparentVelocity; }

    virtual void step(qint64 time, long delta) {
        if (delta > 0) {
            State state;
            this->pushState(time, delta, state);
        } else {
            this->popState(time, delta);
        }
    }

    virtual void pushState(qint64 time, long delta, State& state) {
        state.timestamp = time;
        m_stateStack.push_back(state);
    }

    void popState(qint64 time, long delta) {
        if (!m_stateStack.empty()) {
            State currentState;
            while (!m_stateStack.empty() && (currentState = m_stateStack.back()).timestamp > time) {
                m_stateStack.pop_back();
            }
            this->setState(currentState);
        }
    }

    virtual void setState(State currentState) { Q_UNUSED(currentState); }

    void setSolid(bool solid = true) { m_solid = solid; }
    bool isSolid() { return m_solid; }

    virtual QString className() { return "Sprite"; }

    virtual ItemType blockType() { return ItemType::kBlock; }

    virtual Side damagesChar() { return (Side)0; }

    bool isSlope() {
        ItemType t = this->blockType();
        return t == kSlope30Right ||
               t == kSlope30Left ||
               t == kSlope45Right ||
               t == kSlope45Left ||
               t == kSlope60Right ||
               t == kSlope60Left;
    }

    bool is45() {
        ItemType t = this->blockType();
        return t == kSlope45Right || t == kSlope45Left;
    }

    bool is30() {
        ItemType t = this->blockType();
        return t == kSlope30Right || t == kSlope30Left;
    }

    bool is60() {
        ItemType t = this->blockType();
        return t == kSlope60Right || t == kSlope60Left;
    }

    virtual void fillDatagram(NetworkManager::DatagramFormat &datagram) {
        datagram.pos = this->pos();
        datagram.vel = this->getVelocity();
    }

    virtual void decodeDatagram(NetworkManager::DatagramFormat dg)  { Q_UNUSED(dg); }

    std::vector<State> getStackSlice() { return m_stateSlice; }
};

#endif // SPRITE_H
