#include "maincharacter.h"

#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

MainCharacter::MainCharacter(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent) {

    m_leftAccel = -250;
    m_maxVelX = 250;
    m_maxVelY = 2000;

    m_jumpStartVel = -500;
    m_gravity = 1000;
    m_brakeAccel = 500;

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
    this->setAcceleration(QPointF(0, m_gravity));
}

void MainCharacter::keyPressEvent(QKeyEvent * keyEvent) {
    if (keyEvent->isAutoRepeat()) return;
    qDebug() << "Key Press!!!";

    if (keyEvent->key() == Qt::Key_Down) {
        m_currentState = (MovementState) (Squat_Right + (m_currentState % 2));
        this->triggerAnimation(m_currentState);

        this->setAcceleration(QPointF(SIGN(-this->getAcceleration().x()) * m_brakeAccel, this->getAcceleration().y()));
        this->setBrake(true);
        m_downPressed = true;
    }
    else if (keyEvent->key() == Qt::Key_Right) {
        this->setAcceleration(QPointF(-m_leftAccel, this->getAcceleration().y()));
        this->setBrake(false);
        m_keyRecentPress = keyEvent->key();
        m_currentState = Walk_Right;
        this->triggerAnimation(Walk_Right);
        m_rightPressed = true;
    }
    else if (keyEvent->key() == Qt::Key_Left)  {
        this->setAcceleration(QPointF(m_leftAccel, this->getAcceleration().y()));
        this->setBrake(false);
        m_keyRecentPress = keyEvent->key();
        m_currentState = Walk_Left;
        this->triggerAnimation(Walk_Left);
        m_leftPressed = true;
    }
    if (keyEvent->key() == Qt::Key_Up)  {
        this->jump();
        m_currentState = (MovementState) (Jump_Right + (m_currentState % 2));
        this->triggerAnimation(m_currentState);
        this->m_jumping = true;
        m_upPressed = true;
    }
}

void MainCharacter::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == m_keyRecentPress) {
        this->setAcceleration(QPointF(((m_currentState % 2 == 0) ? -m_brakeAccel : m_brakeAccel), this->getAcceleration().y()));
        this->setBrake(true);
    }
    if (keyEvent->key() == Qt::Key_Down) {
        m_currentState = (MovementState) (m_currentState % 2);
        if (m_rightPressed) this->keyPressEvent(new QKeyEvent(keyEvent->type(), Qt::Key_Right, 0));
        if (m_leftPressed) this->keyPressEvent(new QKeyEvent(keyEvent->type(), Qt::Key_Left, 0));
    }

    switch (keyEvent->key()) {
    case Qt::Key_Up:
        m_upPressed = false;
        break;
    case Qt::Key_Down:
        m_downPressed = false;
        break;
    case Qt::Key_Left:
        m_leftPressed = false;
        break;
    case Qt::Key_Right:
        m_rightPressed = false;
        break;
    }
}

void MainCharacter::step(unsigned long time) {
    AnimatedCollideableSprite::step(time);
    switch (m_currentState) {
        case Walk_Right:
        case Run_Right:
        case Brake_Left:
            if (this->getVelocity().x() < 0.005 && !m_rightPressed) {
                m_currentState = Stand_Right;
                this->triggerAnimation(Stand_Right);
                this->setVelocity(QPointF(0, this->getVelocity().y()));
            }
            break;
        case Walk_Left:
        case Run_Left:
        case Brake_Right:
            if (this->getVelocity().x() > -0.005 && !m_leftPressed) {
                m_currentState = Stand_Left;
                this->triggerAnimation(Stand_Left);
                this->setVelocity(QPointF(0, this->getVelocity().y()));
            }
            break;
    }
    if (this->getVelocity().y() >= 0 && (m_currentState == Jump_Left || m_currentState == Jump_Right)) {
        this->m_jumping = false;
        m_currentState = (MovementState)(m_currentState % 2);
        if (this->getVelocity().x() > 0) m_currentState = Walk_Right;
        else if (this->getVelocity().x() < 0) m_currentState = Walk_Left;
        this->triggerAnimation(m_currentState);
    }
    if (!m_jumping && (m_currentState == Jump_Left || m_currentState == Jump_Right)) this->triggerAnimation(m_currentState - Jump_Right);

    if (m_brake) {
        if (SIGN(this->getVelocity().x()) == SIGN(this->getAcceleration().x())) {
            this->getVelocity().setX(0);
            this->getAcceleration().setX(0);
            m_brake = false;
        }
    }

    if (this->getVelocity().x() > 0)
        this->getVelocity().setX(MIN(this->getVelocity().x(), m_maxVelX));
    else
        this->getVelocity().setX(MAX(this->getVelocity().x(), -m_maxVelX));

    if (this->getVelocity().y() > 0)
        this->getVelocity().setY(MIN(this->getVelocity().y(), m_maxVelY));
    else
        this->getVelocity().setY(MAX(this->getVelocity().y(), -m_maxVelY));
}

/* Possible User Input:
 * Left - Walk and then run left
 * Right - Walk and then run right
 * Left -> Right - (Moving) left, Brake_Left, Walk_Right, Run_Right
 * Up (Moving Left) Jump_Left animation
 * Up (Moving Right) Jump_Right animation
*/

void MainCharacter::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    qDebug() << "COLLISIONS!!!! WOOOOOOOOOO!!!";
}

void MainCharacter::jump() {
    this->getVelocity().setY(m_jumpStartVel);
}
