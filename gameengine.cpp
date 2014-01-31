#include <QDebug>
#include "gameengine.h"
GameEngine::GameEngine() : m_mainActor(NULL)
{
}

GameEngine::GameEngine(int width, int height) :
    m_viewportWidth(width),
    m_viewportHeight(height),
    m_mainActor(NULL) {}

void GameEngine::step(qint64 time) {
    qint64 deltaTime = time - m_prevTime;
    if (m_prevTime == 0) deltaTime = 0;
    m_prevTime = time;

//    for(auto itr = m_stepHandlerVector.begin(); itr != m_stepHandlerVector.end(); itr++) {
//        (*itr)(deltaTime);
//    }

    for(auto itr = m_spriteArray.begin(); itr != m_spriteArray.end(); itr++) {
        (*itr)->step(deltaTime);
    }

    if (m_mainActor != NULL) {
        this->views()[0]->ensureVisible(m_mainActor, 200, 200);
    }
}

void GameEngine::keyPressEvent(QKeyEvent * keyEvent) {
    if (m_mainActor != NULL) {
        m_mainActor->keyPressEvent(keyEvent);
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (m_mainActor != NULL) {
        m_mainActor->keyReleaseEvent(keyEvent);
    }
}
