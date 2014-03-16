/* Author: George Mitchell
 * Use Case: *
 */

#include "sfxmanager.h"
#include "maincharacter.h"

#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

MainCharacter::MainCharacter(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent) {

    m_leftAccel = -400;
    m_rightAccel = -m_leftAccel;
    m_maxVelX = 300;
    m_maxVelY = 2000;

    m_jumpStartVel = -500;
    m_gravity = 2000;
    m_brakeAccel = 800;
    m_brakeAccelSliding = m_brakeAccel / 4;

    m_boxPushAcceleration = 100;
    m_boxPushVelocity = 100;

    this->setZValue(1000);

    TileMap * playerTiles = new TileMap(16, 33, 1, 1, "./resources/sprites/MarioMovement.png");

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
#ifdef TARGET_OS_MAC
    if (keyEvent->isAutoRepeat() || keyEvent->key() == m_keyRecentPress /* Hack to make it work properly on OS X */) return;
#else
    if (keyEvent->isAutoRepeat()) return;
#endif

    if (keyEvent->key() == Qt::Key_Down) {
        if ((this->isOnLeftSlope() && this->getVelocity().x() > 0) ||
                (this->isOnRightSlope() && this->getVelocity().x() < 0))
            this->setAcceleration(QPointF(SIGN(-this->getAcceleration().x()) * m_brakeAccelSliding, this->getAcceleration().y()));
        else
            this->setAcceleration(QPointF(SIGN(-this->getAcceleration().x()) * m_brakeAccel, this->getAcceleration().y()));
        this->setBrake(true);

        m_currentState = (MovementState) (Squat_Right + (m_currentState % 2));
        this->triggerAnimation(m_currentState);

        m_downPressed = true;
    }
    else if (keyEvent->key() == Qt::Key_Right) {
        m_currentState = Walk_Right;
        if (!m_jumping && this->getVelocity().x() < 0) {
            m_currentState = Brake_Right; // We are already moving left so we have to play "brake" animation
            this->setAcceleration(QPointF(2*m_rightAccel, this->getAcceleration().y()));
        } else {
            this->setAcceleration(QPointF(m_rightAccel, this->getAcceleration().y()));
        }
        this->setBrake(false);

        this->triggerAnimation(m_currentState);

        m_rightPressed = true;
    }
    else if (keyEvent->key() == Qt::Key_Left)  {
        m_currentState = Walk_Left;
        if (!m_jumping && this->getVelocity().x() > 0) {
            m_currentState = Brake_Left;
            this->setAcceleration(QPointF(2*m_leftAccel, this->getAcceleration().y()));
        } else {
            this->setAcceleration(QPointF(m_leftAccel, this->getAcceleration().y()));
        }
        this->setBrake(false);

        this->triggerAnimation(m_currentState);

        m_leftPressed = true;
    }
    if (keyEvent->key() == Qt::Key_Up && !(m_jumping && m_jumping_double))  {
        if(m_jumping)
            m_jumping_double=true;
        this->jump();

        m_currentState = (MovementState) (Jump_Right + (m_currentState % 2));
        this->triggerAnimation(m_currentState);

        m_jumping = true;
        m_upPressed = true;
    }

    m_keyRecentPress = keyEvent->key();
}

void MainCharacter::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (keyEvent->isAutoRepeat()) return; // workaround for CentOS Systems

    if (keyEvent->key() == Qt::Key_Down) {
        if (m_currentState == Squat_Left || m_currentState == Squat_Right) {
            m_currentState = (MovementState) (m_currentState % 2);
            this->triggerAnimation(m_currentState);
            if (m_rightPressed) this->keyPressEvent(new QKeyEvent(keyEvent->type(), Qt::Key_Right, 0));
            if (m_leftPressed) this->keyPressEvent(new QKeyEvent(keyEvent->type(), Qt::Key_Left, 0));
        }
    }

    if (keyEvent->key() == m_keyRecentPress) {
        m_keyRecentPress = 0;
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
        if (this->getAcceleration().x() < 0) {
            this->setAcceleration(QPointF(((m_currentState % 2 == 0) ? -m_brakeAccel : m_brakeAccel), this->getAcceleration().y()));
            this->setBrake(true);
        }
        break;
    case Qt::Key_Right:
        m_rightPressed = false;
        if (this->getAcceleration().x() > 0) {
            this->setAcceleration(QPointF(((m_currentState % 2 == 0) ? -m_brakeAccel : m_brakeAccel), this->getAcceleration().y()));
            this->setBrake(true);
        }
        break;
    }
}

void MainCharacter::step(qint64 time, long delta) {
    AnimatedCollideableSprite::step(time, delta);

    if (delta > 0) {
        switch (m_currentState) {
            case Walk_Right:
            case Run_Right:
            case Brake_Left:
                if (this->getVelocity().x() < 0.005 && !m_rightPressed) {
                    m_currentState = Stand_Right;
                    this->triggerAnimation(m_currentState);
                    this->setVelocity(QPointF(0, this->getVelocity().y()));
                }
                break;
            case Walk_Left:
            case Run_Left:
            case Brake_Right:
                if (this->getVelocity().x() > -0.005 && !m_leftPressed) {
                    m_currentState = Stand_Left;
                    this->triggerAnimation(m_currentState);
                    this->setVelocity(QPointF(0, this->getVelocity().y()));
                }
                break;
        }

        // If we are magically moving at a walking pace in either direction then play the walking animation
        if ((m_currentState == Stand_Left || m_currentState == Stand_Right) && this->getVelocity().x() < -5) {
            if (m_acceleration.x() < 2*m_leftAccel) this->getAcceleration().setX(m_leftAccel);
            m_currentState = Walk_Left;
            this->triggerAnimation(m_currentState);
        } else if ((m_currentState == Stand_Left || m_currentState == Stand_Right) && this->getVelocity().x() > 5) {
            if (m_acceleration.x() > 2*m_rightAccel) this->getAcceleration().setX(m_rightAccel);
            m_currentState = Walk_Right;
            this->triggerAnimation(m_currentState);
        }

        if (!m_jumping && (m_currentState == Jump_Left || m_currentState == Jump_Right)) {
            m_currentState = (MovementState) (m_currentState - Jump_Right);
            this->triggerAnimation(m_currentState);
        }

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
}

/* Possible User Input:
 * Left - Walk and then run left
 * Right - Walk and then run right
 * Left -> Right - (Moving) left, Brake_Left, Walk_Right, Run_Right
 * Up (Moving Left) Jump_Left animation
 * Up (Moving Right) Jump_Right animation
*/

void MainCharacter::collisionOccurred(QList<Collision> &collisions, unsigned char side) {
    bool hitCoin = false;
    for (auto itr = collisions.begin(); itr != collisions.end(); itr++) {
        Sprite* other = ((Collision)(*itr)).secondSprite;
        Side locSide = ((Collision)(*itr)).firstSide;

        if (locSide & Bottom && this->getVelocity().y() >= 0 && other->isSolid() && other->y() > this->y()) {
            m_jumping_double = false;
            m_jumping = false;
        }

        if (locSide & other->damagesChar()) {
            ((GameEngine*)this->scene())->characterDamaged();

            SFXManager *inst = SFXManager::Instance();
            inst->playSound(SFXManager::SFX::MainChar_Damaged);
        }

        switch (other->blockType()) {
        case ItemType::kBlock: break;
        case ItemType::kCoin:
            if (!hitCoin) {
                ((GameEngine*)this->scene())->removeItem((((Collision)(*itr)).secondSprite));
                ((GameEngine*)this->scene())->incrementCoins();
                hitCoin = true;

                SFXManager *inst = SFXManager::Instance();
                inst->playSound(SFXManager::SFX::Coin_Grab);
            }
            break;
        case ItemType::kBox:
            if ((locSide & Left && m_leftPressed) || (locSide & Right && m_rightPressed)) {
                qDebug() << "PUSH THE BOX";
                if (locSide & Left && m_leftPressed) {
//                    SFXManager *inst = SFXManager::Instance();
//                    inst->playSound(SFXManager::SFX::MainChar_PushBox);
                    other->setVelocity(QPointF(-m_boxPushVelocity, other->getVelocity().y()));
                    qDebug() << "Push Left";
                    //other->setPos(this->pos().x() - other->boundingRect().width() - 1, other->pos().y());
                }
                else if (locSide & Right && m_rightPressed) {
//                    SFXManager *inst = SFXManager::Instance();
//                    inst->playSound(SFXManager::SFX::MainChar_PushBox);
                    other->setVelocity(QPointF(m_boxPushVelocity, other->getVelocity().y()));
                    qDebug() << "Push Right";
                    //other->setPos(this->pos().x() + this->boundingRect().width() + 1, other->pos().y());
                }
            }
            break;
//        case ItemType::kSlope30Left:
//            qDebug() << "kSlope30Left";
//            break;
//        case ItemType::kSlope45Left:
//            qDebug() << "kSlope45Left";
//            break;
//        case ItemType::kSlope60Left:
//            qDebug() << "kSlope60Left";
//            break;
//        case ItemType::kSlope30Right:
//            qDebug() << "kSlope30Right";
//            break;
//        case ItemType::kSlope45Right:
//            qDebug() << "kSlope45Right";
//            break;
//        case ItemType::kSlope60Right:
//            qDebug() << "kSlope60Right";
//            break;
        default:
            break;
        }
    }
}

void MainCharacter::jump() {
    this->getVelocity().setY(m_jumpStartVel);

    SFXManager *inst = SFXManager::Instance();
    inst->playSound(SFXManager::SFX::MainChar_Jump);
}
