#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPixmap>
#include <vector>

#include "sprite.h"

enum kAnimationType {
    Forward,
    Loop,
    Forward_Reverse,
    Reverse_Forward,
    Forward_Reverse_Loop,
    Reverse_Forward_Loop
};

class AnimatedSprite : public Sprite {

private:
    /* Data for animations:
     *  - Path that pixmap will follow for the duration of the animation
     *  - Current Animation, Current Frame of Animation
     *  - List of series' of pixmaps that correspond to each animation that this sprite will contain
     */

    std::vector<QPainterPath> m_animationPathList;
    unsigned char m_nCurrentFrame;
    unsigned char m_nCurrentAnimation;
    std::vector<std::vector<QPixmap>> m_animationList;
    std::vector<kAnimationType> m_animationType;

    bool m_countUp;
    unsigned int m_msPerFrame;
    unsigned int m_msCounter;

    int m_width, m_height;

public:
    explicit AnimatedSprite(int width, int height, QGraphicsItem *parent = 0);
    void triggerAnimation(unsigned int anim);

    void addAnimation(std::vector<QPixmap> pixmapList, QPainterPath animationPath, kAnimationType animationType = Forward);
    void addAnimation(std::vector<QPixmap> pixmapList, kAnimationType animationType = Forward);

    virtual void step(unsigned long time);

    bool isStatic() { return false; }
    bool isAnimated() { return true; }
    bool isCollideable() { return false; }
    bool isBackground() { return false; }

signals:
    
public slots:
    
};

#endif // ANIMATEDSPRITE_H
