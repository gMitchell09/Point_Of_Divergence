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
#include "networkmanager.h"

enum kAnimationType {
    Loop,                   Loop_Reverse,
    Forward,                Reverse,
    Forward_Reverse,        Reverse_Forward,
    Forward_Reverse_Loop,   Reverse_Forward_Loop
};


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

    std::vector<State> m_stateStack;

    std::vector<State> m_stateSlice;
    int m_stateSliceBegin, m_stateSliceEnd;
    int m_stateSliceIndex;

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


    virtual void pushState(qint64 time, long delta, State &state) {
        state.m_nCurrentAnimation = this->m_nCurrentAnimation;
        state.m_nCurrentFrame = this->m_nCurrentFrame;

        Sprite::pushState(time, delta, state);
    }

    virtual void fillDatagram(NetworkManager::DatagramFormat &datagram) {
        Sprite::fillDatagram(datagram);
        datagram.mainCharFrame = m_nCurrentFrame;
        datagram.mainCharAnim = m_nCurrentAnimation;
    }

    virtual void decodeDatagram(NetworkManager::DatagramFormat dg) {
        Sprite::decodeDatagram(dg);
        m_nCurrentAnimation = dg.mainCharAnim;
        m_nCurrentFrame = dg.mainCharFrame;

        this->setPixmap(m_animationList.at(m_nCurrentAnimation).at(m_nCurrentFrame));
    }

    virtual void setState(State currentState) {
        Sprite::setState(currentState);
        m_nCurrentAnimation = currentState.m_nCurrentAnimation;
        m_nCurrentFrame = currentState.m_nCurrentFrame;
    }

    virtual bool isStatic() { return false; }
    virtual bool isAnimated() { return true; }
    virtual bool isCollideable() { return false; }
    virtual bool isBackground() { return false; }

    virtual QString className() { return "AnimatedSprite"; }

protected:
    virtual bool usesStack() { return false; }
    
};

#endif // ANIMATEDSPRITE_H
