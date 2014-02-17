/* Author: George Mitchell
 * Use Case: *
 */

#include <QDebug>
#include <QPixmap>
#include "gameengine.h"
GameEngine::GameEngine() : m_mainActor(NULL), m_prevTime(0)
{
}

GameEngine::GameEngine(int width, int height) :
    m_mainActor(NULL),
    m_prevTime(0),
    m_gameTime(0),
    m_hudGameTime(NULL) {
    this->setBackgroundBrush(QBrush(QColor(210, 210, 255, 255)));
}

void GameEngine::step(qint64 time) {
    qint64 deltaTime = 0;

    deltaTime = time - m_prevTime;
    if (m_timeReversed) deltaTime = -deltaTime;

    if (m_prevTime == 0) deltaTime = 0;

    // This is what we will timestamp all history events with so we can have a definite point-of-reference
    //   to accurately play back events.
    m_gameTime += deltaTime;

    m_prevTime = time;

    for(auto itr = m_spriteArray.begin(); itr != m_spriteArray.end(); itr++) {
        (*itr)->step(m_gameTime, deltaTime);
    }

    if (m_mainActor != NULL) {
        this->views()[0]->ensureVisible(m_mainActor, 200, 200);
    }

    if (m_hudGameTime != NULL) {
        m_hudGameTime->setText(QString("Time: %1").arg(m_gameTime/1000, 4, 10, QChar('0')));
    }

    this->removeDeletedItems();
}

void GameEngine::keyPressEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == Qt::Key_R) {
        m_timeReversed = true;
        displayBackground(QColor(120, 255, 120, 120));
//        this->setBackgroundBrush(QBrush(QColor(120, 255, 120, 120)));
    }
    else if (m_mainActor != NULL) {
        m_mainActor->keyPressEvent(keyEvent);
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == Qt::Key_R) {
        m_timeReversed = false;
        displayBackground(QColor(210, 210, 255, 255));
//        this->setBackgroundBrush(QBrush(QColor(210, 210, 255, 255)));
    }
    else if (m_mainActor != NULL) {
        m_mainActor->keyReleaseEvent(keyEvent);
    }
}

bool GameEngine::event(QEvent *event) {
    if (event->type() == QEvent::MetaCall) {
        for (auto itr = m_hudArray.begin(); itr != m_hudArray.end(); itr++) {
            (*itr)->step(0, 0);
        }
        for (auto itr = m_hudTextArray.begin(); itr != m_hudTextArray.end(); itr++) {
            (*itr)->step(0, 0);
        }
    }
    return QGraphicsScene::event(event);
}


//iterates through sprite array to remove deleted sprite
void GameEngine::removeItem(QGraphicsItem *item) {
    QGraphicsScene::removeItem(item);

    delete item;
}

void GameEngine::removeItem(Sprite *item) {
    QGraphicsScene::removeItem(item);

    m_deletedItems.push_back(item);
}

void GameEngine::removeDeletedItems() {
    for (auto itr = m_deletedItems.begin(); itr != m_deletedItems.end(); itr++) {
        m_spriteArray.erase(std::remove(m_spriteArray.begin(), m_spriteArray.end(), (*itr)), m_spriteArray.end());
    }

    m_deletedItems.empty();
}

void GameEngine::displayBackground(QColor mycolor) {
    this->setBackgroundBrush(QBrush(mycolor));
}

void GameEngine::displayBackground(QPixmap &bkgPix) {
    bkg = new StaticBackground(QPoint(0, 0));
    bkg->setPixmap(bkgPix);
    bkg->setCollideable(false);
    bkg->setZValue(-1001);
    this->addItem(bkg);
}
