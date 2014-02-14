#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

/* Author: George Mitchell
 * Use Case: Step Through Animation
 */

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPixmap>
#include <vector>
#include <stack>

#include "sprite.h"

enum kAnimationType {
    Loop,                   Loop_Reverse,
    Forward,                Reverse,
    Forward_Reverse,        Reverse_Forward,
    Forward_Reverse_Loop,   Reverse_Forward_Loop
};

struct State { char m_nCurrentFrame, m_nCurrentAnimation; };

class AnimatedSprite : public Sprite {

private:
    /* Data for animations:
     *  - Path that pixmap will follow for the duration of the animation
     *  - Current Animation, Current Frame of Animation
     *  - List of series' of pixmaps that correspond to each animation that this sprite will contain
     */

    std::vector<QPainterPath> m_animationPathList;
    char m_nCurrentFrame;
    char m_nCurrentAnimation;
    std::vector<std::vector<QPixmap>> m_animationList;
    std::vector<kAnimationType> m_animationType;

    std::stack<State> m_stateStack;

    bool m_countUp;
    unsigned int m_msPerFrame;
    unsigned int m_msCounter;

    int m_width, m_height;

public:
    explicit AnimatedSprite(int width, int height, QGraphicsItem *parent = 0);
    void triggerAnimation(unsigned int anim);

    void addAnimation(std::vector<QPixmap> pixmapList, QPainterPath animationPath, kAnimationType animationType = Forward);
    void addAnimation(std::vector<QPixmap> pixmapList, kAnimationType animationType = Forward);

    virtual void step(qint64 time, long delta);

    virtual bool isStatic() { return false; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return false; }
    virtual bool isBackground() { return false; }

protected:
    virtual bool usesStack() { return false; }

signals:
    
public slots:
    
};

#endif // ANIMATEDSPRITE_H
