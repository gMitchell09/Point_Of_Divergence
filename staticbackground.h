#ifndef STATICBACKGROUND_H
#define STATICBACKGROUND_H

#include "sprite.h"

///
/// \brief The StaticBackground class... an immobile, uncollideable sprite that sits in the background and looks pretty
///
class StaticBackground : public Sprite
{
private:
    QPoint m_repeatDistance;
    bool m_collideable; // REMOVE ASAP
public:
    explicit StaticBackground(QPoint repeatDistance, QGraphicsItem* parent = 0);

    // Overrides
    virtual bool isStatic();
    virtual bool isAnimated();
    virtual bool isCollideable();
    virtual bool isBackground();

    virtual void step (qint64 time, long delta);
    inline void setCollideable(bool c) {
        m_collideable = c;
    }

    virtual QString className() { return "StaticBackground"; }
};

#endif // STATICBACKGROUND_H
