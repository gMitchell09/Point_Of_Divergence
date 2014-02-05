#ifndef SPRITE_H
#define SPRITE_H

/* Author: George Mitchell
 * Use Case: *
 */

#include <QGraphicsPixmapItem>

///
/// \brief The Sprite class is an abstract base class for all other sprites in engine
///
class Sprite : public QGraphicsPixmapItem
{
protected:
    QPointF m_acceleration, m_velocity;

public:
    explicit Sprite(QGraphicsItem *parent = 0);

    ///
    /// \brief isStatic
    /// \return true if sprite doesn't respond to changes in velocity and acceleration
    ///
    virtual bool isStatic() = 0;

    ///
    /// \brief isAnimated
    /// \return true if sprite has multiple frames
    ///
    virtual bool isAnimated() = 0;

    ///
    /// \brief isCollideable
    /// \return true if we need to perform collision checking on this sprite
    ///
    virtual bool isCollideable() = 0;

    ///
    /// \brief isBackground
    /// \return true if this sprite is a background
    ///
    virtual bool isBackground() = 0;

    void setAcceleration(QPointF acceleration) { m_acceleration = acceleration; }
    QPointF& getAcceleration() { return m_acceleration; }

    void setVelocity(QPointF velocity) { m_velocity = velocity; }
    QPointF& getVelocity() { return m_velocity; }

    virtual void step (unsigned long time) = 0;

    
signals:
    
public slots:
    
};

#endif // SPRITE_H
