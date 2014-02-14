#ifndef GAMEENGINE_H
#define GAMEENGINE_H

/* Author: George Mitchell
 * Use Case: *
 */

#include <functional>
#include <vector>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QBrush>
#include <QColor>

#include "animatedsprite.h"
#include "animatedcollideablesprite.h"
#include "maincharacter.h"
#include "hudsprite.h"
#include "hudtext.h"

class GameEngine : public QGraphicsScene
{
private:
    /* Arbitrary Constants, chosen by fair dice roll */
    static const int m_gravity = -5; // The enemy's gate is down.

    /* This is the minimum character distance from the edge of the screen before
     *  the viewport will start following the character.
     */
    static const int m_windowXTolerance = 40;
    static const int m_windowYTolerance = 120;
    /* --- Arbitrary Constants End */

    qint64 m_prevTime;
    qint64 m_gameTime;

    MainCharacter * m_mainActor;

    std::vector<std::function<void(long)>> m_stepHandlerVector;

    // This is a vector of sprites that can be interacted with
    std::vector<Sprite*> m_spriteArray;
    std::vector<HUDsprite*> m_hudArray;
    std::vector<HUDText*> m_hudTextArray;

    HUDText* m_hudGameTime;

    //int m_keyRecentPress;
    bool m_timeReversed = false;


public:
    GameEngine();
    GameEngine(int width, int height);

    /* Iterate over m_stepHandlerVector and call each object so that they may update
     * 0. Calculate dt and update m_prevTime
     * 1. Call update on all child sprites by iterating over m_stepHandlerVector
     * 2. Check m_mainActor position to see if we need to update the viewport
     */
    void step(qint64 time);

    inline void addHandler(std::function<void(long)> callback) {
        m_stepHandlerVector.push_back(callback);
    }

    inline void addHUD(HUDsprite* sprite) {
        this->addItem(sprite);
        m_hudArray.push_back(sprite);
    }

    inline void addHUDText(HUDText* text) {
        this->addItem(text);
        m_hudTextArray.push_back(text);
        m_hudGameTime = text;
    }

    inline size_t addSprite(Sprite* sprite, bool mainActor = false) {
        this->addItem(sprite);
        if (mainActor) {
            m_mainActor = dynamic_cast<MainCharacter*>(sprite);
        }
        m_spriteArray.push_back(sprite);
        return m_spriteArray.size();
    }

    virtual bool event(QEvent *event);

protected:
    virtual void keyPressEvent(QKeyEvent * keyEvent);
    virtual void keyReleaseEvent(QKeyEvent * keyEvent);
};

#endif // GAMEENGINE_H
