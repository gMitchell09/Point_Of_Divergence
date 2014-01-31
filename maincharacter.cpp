#include "maincharacter.h"

MainCharacter::MainCharacter(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent)
{
    TileMap * playerTiles = new TileMap(16, 33, 1, 1, ":MarioRight/MarioMovement.png");

    std::vector<QPixmap> standRight;
    standRight.push_back(playerTiles->copyCellAt(4, 0));

    std::vector<QPixmap> standLeft;
    standLeft.push_back(playerTiles->copyCellAt(3, 0));

    std::vector<QPixmap> walkRight;
    walkRight.push_back(playerTiles->copyCellAt(5, 0));
    walkRight.push_back(playerTiles->copyCellAt(6, 0));

    std::vector<QPixmap> walkLeft;
    walkLeft.push_back(playerTiles->copyCellAt(1, 0));
    walkLeft.push_back(playerTiles->copyCellAt(2, 0));

    std::vector<QPixmap> runRight;
    runRight.push_back(playerTiles->copyCellAt(5, 0));
    runRight.push_back(playerTiles->copyCellAt(6, 0));

    std::vector<QPixmap> runLeft;
    runLeft.push_back(playerTiles->copyCellAt(1, 0));
    runLeft.push_back(playerTiles->copyCellAt(2, 0));

    std::vector<QPixmap> jumpRight;
    jumpRight.push_back(playerTiles->copyCellAt(3, 1));

    std::vector<QPixmap> jumpLeft;
    jumpLeft.push_back(playerTiles->copyCellAt(2, 1));

    std::vector<QPixmap> squatRight;
    squatRight.push_back(playerTiles->copyCellAt(1, 1));

    std::vector<QPixmap> squatLeft;
    squatLeft.push_back(playerTiles->copyCellAt(0, 1));

    std::vector<QPixmap> brakeRight;
    brakeRight.push_back(playerTiles->copyCellAt(7, 0));

    std::vector<QPixmap> brakeLeft;
    brakeLeft.push_back(playerTiles->copyCellAt(0, 0));

    this->addAnimation(standRight, Loop);
    this->addAnimation(standLeft, Loop);

    this->addAnimation(walkRight, Loop);
    this->addAnimation(walkLeft, Loop);

    this->addAnimation(runRight, Loop);
    this->addAnimation(runLeft, Loop);

    this->addAnimation(jumpRight, Loop);
    this->addAnimation(jumpLeft, Loop);

    this->addAnimation(squatRight, Loop);
    this->addAnimation(squatLeft, Loop);

    this->addAnimation(brakeRight, Loop);
    this->addAnimation(brakeLeft, Loop);

    this->setSolid(true);
    this->setVelocity(QPointF(0, 0));
    this->setAcceleration(QPointF(0, 0));

    this->triggerAnimation(Stand_Right);
    this->setAcceleration(QPointF(0, 2000));

}

void MainCharacter::keyPressEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == Qt::Key_Right) {
        this->setAcceleration(QPointF(500, this->getAcceleration().y()));
        this->setBrake(false);
        m_keyRecentPress = keyEvent->key();
        m_currentState = Walk_Right;
        this->triggerAnimation(Walk_Right);
    }
    else if (keyEvent->key() == Qt::Key_Left)  {
        this->setAcceleration(QPointF(-500, this->getAcceleration().y()));
        this->setBrake(false);
        m_keyRecentPress = keyEvent->key();
        m_currentState = Walk_Left;
        this->triggerAnimation(Walk_Left);
    }
    if (keyEvent->key() == Qt::Key_Up)  {
        this->jump();
        m_currentState = (MovementState) (Jump_Right + (m_currentState % 2));
        this->triggerAnimation(m_currentState);
        this->m_jumping = true;
    }
}

void MainCharacter::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == m_keyRecentPress) {
        this->setAcceleration(QPointF(-this->getAcceleration().x(), this->getAcceleration().y()));
        this->setBrake(true);
    }
}

void MainCharacter::step(unsigned long time) {
    AnimatedCollideableSprite::step(time);
    switch (m_currentState) {
        case Walk_Right:
        case Run_Right:
        case Brake_Left:
            if (this->getVelocity().x() < 2) {
                this->triggerAnimation(Stand_Right);
                this->setVelocity(QPointF(0, this->getVelocity().y()));
            }
            break;
        case Walk_Left:
        case Run_Left:
        case Brake_Right:
            if (this->getVelocity().x() > -2) {
                this->triggerAnimation(Stand_Left);
                this->setVelocity(QPointF(0, this->getVelocity().y()));
            }
            break;
    }
    if (this->getVelocity().y() >= 0 && (m_currentState == Jump_Left || m_currentState == Jump_Right)) {
        this->m_jumping = false;
        m_currentState = (MovementState)(m_currentState % 2);
        if (this->getVelocity().x() > 2) m_currentState = Walk_Right;
        else if (this->getVelocity().x() < -2) m_currentState = Walk_Left;
        this->triggerAnimation(m_currentState);
    }
    if (!m_jumping && (m_currentState == Jump_Left || m_currentState == Jump_Right)) this->triggerAnimation(m_currentState - Jump_Right);
}

/* Possible User Input:
 * Left - Walk and then run left
 * Right - Walk and then run right
 * Left -> Right - (Moving) left, Brake_Left, Walk_Right, Run_Right
 * Up (Moving Left) Jump_Left animation
 * Up (Moving Right) Jump_Right animation
*/
