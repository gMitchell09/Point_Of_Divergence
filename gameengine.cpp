/* Author: George Mitchell
 * Use Case: *
 */

#include <QDebug>
#include "gameengine.h"
GameEngine::GameEngine() : m_mainActor(NULL), m_prevTime(0)
{
}

GameEngine::GameEngine(int width, int height) :
    m_mainActor(NULL),
    m_prevTime(0) {
    this->setBackgroundBrush(QBrush(QColor(210, 210, 255, 255)));
}

void GameEngine::step(qint64 time) {
    qint64 deltaTime = 0;

    deltaTime = time - m_prevTime;
    if (m_timeReversed) deltaTime = -deltaTime;
    m_prevTime = time;

    if (m_prevTime == 0) deltaTime = 0;

    for(auto itr = m_spriteArray.begin(); itr != m_spriteArray.end(); itr++) {
        (*itr)->step(deltaTime);
    }

    if (m_mainActor != NULL) {
        this->views()[0]->ensureVisible(m_mainActor, 200, 200);
    }
}

void GameEngine::keyPressEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == Qt::Key_R) {
        m_timeReversed = true;
        this->setBackgroundBrush(QBrush(QColor(120, 255, 120, 120)));
    }
    else if (m_mainActor != NULL) {
        m_mainActor->keyPressEvent(keyEvent);
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == Qt::Key_R) {
        m_timeReversed = false;
        this->setBackgroundBrush(QBrush(QColor(210, 210, 255, 255)));
    }
    else if (m_mainActor != NULL) {
        m_mainActor->keyReleaseEvent(keyEvent);
    }
}

bool GameEngine::event(QEvent *event) {
    if (event->type() == QEvent::MetaCall)
        for(auto itr = m_hudArray.begin(); itr != m_hudArray.end(); itr++) {
            (*itr)->step(0);
        }
    return QGraphicsScene::event(event);
}
