/* Author: George Mitchell
 * Use Case: Step Through Animation
 */

#include "animatedsprite.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsTransform>
#include <QGraphicsView>
#include <assert.h>

#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

AnimatedSprite::AnimatedSprite(int width, int height, QGraphicsItem *parent) :
    Sprite(parent),
    m_width(width), m_height(height) {
}

void AnimatedSprite::addAnimation(std::vector<QPixmap> pixmapList, QPainterPath animationPath, kAnimationType animationType) {
    m_animationList.push_back(pixmapList);
    m_animationPathList.push_back(animationPath);
    m_animationType.push_back(animationType);
}

void AnimatedSprite::addAnimation(std::vector<QPixmap> pixmapList, kAnimationType animationType) {
    m_animationList.push_back(pixmapList);
    m_animationType.push_back(animationType);
}

void AnimatedSprite::triggerAnimation(unsigned int anim) {
    m_nCurrentAnimation = anim;
    m_msPerFrame = 200;

    if (m_nCurrentAnimation >= m_animationType.size() || m_nCurrentAnimation < 0) return;

    switch (m_animationType.at(anim)) {
        case Forward:
        case Forward_Reverse:
        case Forward_Reverse_Loop:
        case Loop:
            m_countUp = true;
            m_nCurrentFrame = 0;
            break;
        case Reverse_Forward:
        case Reverse_Forward_Loop:
            m_countUp = false;
            m_nCurrentFrame = m_animationList.at(anim).size()-1;
            break;
    }
    assert (m_nCurrentFrame <= m_animationList.at(m_nCurrentAnimation).size());
    this->setPixmap(m_animationList.at(m_nCurrentAnimation).at(m_nCurrentFrame));
}

void AnimatedSprite::step(unsigned long time) {
    if (m_animationType.empty() || m_animationList.empty()) return;

    m_msCounter += time;
    if (m_msCounter >= m_msPerFrame) {
        switch(m_animationType.at(m_nCurrentAnimation)) {
            case Forward:
                // DO NOT REPLACE +1 WITH ++.  IF YOU DO I _WILL_ FIND YOU.  :)
                m_nCurrentFrame = MIN(m_nCurrentFrame+1, m_animationList.at(m_nCurrentAnimation).size() - 1);
                break;

            case Loop:
                m_nCurrentFrame = ++m_nCurrentFrame % m_animationList.at(m_nCurrentAnimation).size();
                break;

            case Forward_Reverse:
                if (m_nCurrentFrame == 0 && !m_countUp) break;
                if (m_nCurrentFrame == m_animationList.at(m_nCurrentAnimation).size() - 1)
                    m_countUp = !m_countUp;

                if (m_countUp)
                    m_nCurrentFrame++;
                else
                    m_nCurrentFrame--;
                break;

            case Reverse_Forward:
                if (m_nCurrentFrame == m_animationList.at(m_nCurrentAnimation).size() - 1 && m_countUp) break;
                if (m_nCurrentFrame == 0)
                    m_countUp = !m_countUp;

                if (m_countUp)
                    m_nCurrentFrame++;
                else
                    m_nCurrentFrame--;
                break;

            case Forward_Reverse_Loop:
            case Reverse_Forward_Loop:
                if ((m_nCurrentFrame == m_animationList.at(m_nCurrentAnimation).size() - 1 && m_countUp)||
                        (m_nCurrentFrame == 0 && !m_countUp))
                    m_countUp = !m_countUp;

                if (m_countUp)
                    m_nCurrentFrame++;
                else
                    m_nCurrentFrame--;
                break;

            default:
                break;
        }
        m_msCounter = 0;
    }

    if (m_nCurrentFrame >= m_animationList.at(m_nCurrentAnimation).size() || m_nCurrentFrame < 0) m_nCurrentFrame = m_animationList.at(m_nCurrentAnimation).size() - 1;
    this->setPixmap(m_animationList.at(m_nCurrentAnimation).at(m_nCurrentFrame));
}
