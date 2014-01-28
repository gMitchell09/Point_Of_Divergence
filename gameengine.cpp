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
        if (keyEvent->key() == Qt::Key_Right) {
            m_mainActor->setAcceleration(QPointF(500, m_mainActor->getAcceleration().y()));
            m_mainActor->setBrake(false);
        }
        else if (keyEvent->key() == Qt::Key_Left)  {
            m_mainActor->setAcceleration(QPointF(-500, m_mainActor->getAcceleration().y()));
            m_mainActor->setBrake(false);
        }
        if (keyEvent->key() == Qt::Key_Up)  {
            m_mainActor->jump();
        }
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (m_mainActor != NULL) {
        if (keyEvent->key() == Qt::Key_Right ||
                keyEvent->key() == Qt::Key_Left) {
            m_mainActor->setAcceleration(QPointF(-m_mainActor->getAcceleration().x(), m_mainActor->getAcceleration().y()));
            m_mainActor->setBrake(true);
        }
//        if (keyEvent->key() == Qt::Key_Up)  {
//        }
    }
}
