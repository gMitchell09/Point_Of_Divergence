#ifndef SPRITE_H
#define SPRITE_H

/* Author: George Mitchell
 * Use Case: *
 */

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

#include "global.h"

///
/// \brief The Sprite class is an abstract base class for all other sprites in engine
///

enum Side { Top = 1, Right = 2, Bottom = 4, Left = 8 };

class Sprite : public QGraphicsPixmapItem
{
private:
    bool m_solid;

protected:
    QPointF m_acceleration, m_velocity;

    virtual bool usesStack() { return false; }

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

    virtual void step(qint64 time, long delta) {}

    virtual void pushState(qint64 time, long delta) {}
    virtual void popState(qint64 time, long delta) {}

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
};

#endif // SPRITE_H
