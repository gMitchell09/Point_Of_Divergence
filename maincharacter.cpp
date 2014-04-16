/* Author: George Mitchell
 * Use Case: *
 */

#include "sfxmanager.h"
#include "maincharacter.h"
#include "characterclone.h"

#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)
#define ABS(x) ((x>0)?(x):(-x))

MainCharacter::MainCharacter(int width, int height, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent),
    m_isOnLadder(false),
    m_leftPressed(false),
    m_rightPressed(false),
    m_downPressed(false),
    m_upPressed(false) {

    m_leftAccel = -400;
    m_rightAccel = -m_leftAccel;
    m_maxVelX = 300;
    m_maxVelY = 2000;

    m_jumpStartVel = -500;
    m_ladderClimbSpeed = -100;
    m_gravity = 2000;
    m_brakeAccel = 800;
    m_brakeAccelSliding = m_brakeAccel / 4;

    m_boxPushAcceleration = 100;
    m_boxPushVelocity = 100;

    this->setZValue(1);

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

    this->triggerAnimation(Stand_Right);
}

void MainCharacter::keyPressEvent(QKeyEvent * keyEvent) {
#ifdef TARGET_OS_MAC
    if (keyEvent->isAutoRepeat() || keyEvent->key() == m_keyRecentPress /* Hack to make it work properly on OS X */) return;
#else
    if (keyEvent->isAutoRepeat()) return;
#endif

    if (keyEvent->key() == Qt::Key_Down) {
        if (m_isOnLadder) {
            this->climbLadder(-1);
        }
        m_currentState = (MovementState) (Squat_Right + (m_currentState % 2));
        this->triggerAnimation(m_currentState);

        m_downPressed = true;
    }
    else if (keyEvent->key() == Qt::Key_Right) {
        m_currentState = Walk_Right;
        if (!m_jumping && this->getVelocity().x < 0) {
            m_currentState = Brake_Right; // We are already moving left so we have to play "brake" animation
        }
        this->triggerAnimation(m_currentState);
        m_rightPressed = true;
    }
    else if (keyEvent->key() == Qt::Key_Left)  {
        m_currentState = Walk_Left;
        if (!m_jumping && this->getVelocity().x > 0) {
            m_currentState = Brake_Left;
        }
        this->triggerAnimation(m_currentState);
        m_leftPressed = true;
    }
    else if (keyEvent->key() == Qt::Key_Up)  {
        m_upPressed = true;
        if (m_isOnLadder) {
            this->climbLadder();
            m_jumping = false;
            m_jumping_double = false;
        }
        else if (!(m_jumping && m_jumping_double)) {
            if(m_jumping) {
                m_jumping_double=true;
            }
            this->jump();

            m_currentState = (MovementState) (Jump_Right + (m_currentState % 2));
            this->triggerAnimation(m_currentState);

            m_jumping = true;
        }
    } else if (keyEvent->key() == Qt::Key_Shift) {
        this->beginSlice();
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
            if (m_isOnLadder)  {
//                this->getVelocity().setY(0);
//                this->getAcceleration().setY(0);
            }
            break;
        case Qt::Key_Down:
            m_downPressed = false;
            if (m_isOnLadder)  {
//                this->getVelocity().setY(0);
//                this->getAcceleration().setY(0);
            }
            break;
        case Qt::Key_Left:
            m_leftPressed = false;
//            if (this->getAcceleration().x() < 0) {
//                this->setAcceleration(QPointF(((m_currentState % 2 == 0) ? -m_brakeAccel : m_brakeAccel), this->getAcceleration().y()));
//                this->setBrake(true);
//            }
            break;
        case Qt::Key_Right:
            m_rightPressed = false;
//            if (this->getAcceleration().x() > 0) {
//                this->setAcceleration(QPointF(((m_currentState % 2 == 0) ? -m_brakeAccel : m_brakeAccel), this->getAcceleration().y()));
//                this->setBrake(true);
//            }
            break;
        case Qt::Key_Shift:
            this->endSlice();
            if (m_body) {
                b2BodyDef bodyDef;
                bodyDef.position.Set(m_body->GetPosition().x, m_body->GetPosition().y);
                bodyDef.type = b2_dynamicBody;
                b2Body* body = this->m_body->GetWorld()->CreateBody(&bodyDef);
                b2PolygonShape mcShape;
                b2Vec2 verts[] = {
                    b2Vec2(0.55, 0.0),
                    b2Vec2(0.5, -0.05),
                    b2Vec2(0.5, -1.95),
                    b2Vec2(0.55, -2.0),
                    b2Vec2(1.45, -2),
                    b2Vec2(1.5, -1.95),
                    b2Vec2(1.5, -0.95),
                    b2Vec2(1.45, 0.0)
                };
                mcShape.Set(verts, 8);
                body->SetFixedRotation(true);
                body->CreateFixture(&mcShape, 1.0f)->SetFriction(0.1f);

                CharacterClone* clone = new CharacterClone(16, 33, this->m_stateSlice, body);
                ((GameEngine*)this->scene())->addSprite(clone);
                body->SetUserData(clone);
            }

            break;
    }
}

void MainCharacter::step(qint64 time, long delta) {
    AnimatedCollideableSprite::step(time, delta);

//    if (!m_isOnLadder) {
//        this->getAcceleration().setY(m_gravity);
//    } else {
//        this->getAcceleration().setY(0);
//    }

    if (delta > 0) {
        switch (m_currentState) {
            case Walk_Right:
            case Run_Right:
            case Brake_Left:
                if (this->getVelocity().x < 1 && !m_rightPressed) {
                    m_currentState = Stand_Right;
                    this->triggerAnimation(m_currentState);
                    this->resetForces();
                }
                break;
            case Walk_Left:
            case Run_Left:
            case Brake_Right:
                if (this->getVelocity().x > -1 && !m_leftPressed) {
                    m_currentState = Stand_Left;
                    this->triggerAnimation(m_currentState);
                    this->resetForces();
                }
                break;
        }

        // If we are magically moving at a walking pace in either direction then play the walking animation
        if ((m_currentState == Stand_Left || m_currentState == Stand_Right) && this->getVelocity().x < -1) {
            m_currentState = Walk_Left;
            this->triggerAnimation(m_currentState);
        } else if ((m_currentState == Stand_Left || m_currentState == Stand_Right) && this->getVelocity().x > 1) {
            m_currentState = Walk_Right;
            this->triggerAnimation(m_currentState);
        }

        if (!m_jumping && (m_currentState == Jump_Left || m_currentState == Jump_Right)) {
            m_currentState = (MovementState) (m_currentState - Jump_Right);
            this->triggerAnimation(m_currentState);
        }
    }

    if (m_leftPressed) {
        this->m_body->ApplyForceToCenter(b2Vec2(-20, 0), true);
    }

    if (m_rightPressed) {
        this->m_body->ApplyForceToCenter(b2Vec2(20, 0), true);
    }

    // Velocity Limiting:
    b2Vec2 vel = this->getVelocity();
    if (ABS(vel.x) > ABS(this->m_maxSpeed)) {
        this->setVelocity(b2Vec2(SIGN(vel.x) * this->m_maxSpeed, vel.y), false);
    }

    // We only want to check if we are going UP faster than our max speed
    if (vel.y > this->m_maxSpeed) {
        this->setVelocity(b2Vec2(vel.x, this->m_maxSpeed), false);
    }
}

/* Possible User Input:
 * Left - Walk and then run left
 * Right - Walk and then run right
 * Left -> Right - (Moving) left, Brake_Left, Walk_Right, Run_Right
 * Up (Moving Left) Jump_Left animation
 * Up (Moving Right) Jump_Right animation
*/

void MainCharacter::collisionOccurred(Sprite *other, Side side) {
    AnimatedCollideableSprite::collisionOccurred(other, side);
    unsigned int ladderSide = 0;
    bool m_isOnLadder = false;

    if (side == Bottom && other->isSolid() && m_body->GetLinearVelocity().y <= 0) {
        m_jumping_double = false;
        m_jumping = false;
        if (m_currentState == Jump_Left || m_currentState == Jump_Right)
            m_currentState = (MovementState) (m_currentState % 2);
    }

    if (side & other->damagesChar()) {
        ((GameEngine*)this->scene())->characterDamaged();

        SFXManager *inst = SFXManager::Instance();
        inst->playSound(SFXManager::SFX::MainChar_Damaged);
    }

    switch (other->blockType()) {
        case ItemType::kBlock: break;
        case ItemType::kCoin: {
            ((GameEngine*)this->scene())->removeItem(other);
            ((GameEngine*)this->scene())->incrementCoins();

            SFXManager *inst = SFXManager::Instance();
            inst->playSound(SFXManager::SFX::Coin_Grab);

            break;
        }
        case ItemType::kBox:
            if ((side & Left && m_leftPressed) || (side & Right && m_rightPressed)) {
                if (side & Left && m_leftPressed) {
//                    SFXManager *inst = SFXManager::Instance();
//                    inst->playSound(SFXManager::SFX::MainChar_PushBox);
                }
                else if (side & Right && m_rightPressed) {
//                    SFXManager *inst = SFXManager::Instance();
//                    inst->playSound(SFXManager::SFX::MainChar_PushBox);
                }
            }
            break;
        case ItemType::kLadder:
            ladderSide |= side;
            if (side & Top) m_isOnLadder = true;
            if (m_upPressed) {
                if (!m_isOnLadder) {
                    this->jump();
                }
                b2Vec2 vel = this->m_body->GetLinearVelocity();
                vel.x = 0;
                this->m_body->SetLinearVelocity(vel);
            }
            break;
        case ItemType::kLever:
            if (m_downPressed) {
                dynamic_cast<SwitchObject*>(other)->toggle();
            }

        default:
            break;
    }

}

void MainCharacter::jump() {
    this->m_body->ApplyForceToCenter(b2Vec2(0, 1000), true);

    SFXManager *inst = SFXManager::Instance();
    inst->playSound(SFXManager::SFX::MainChar_Jump);
}

void MainCharacter::climbLadder(int dir) {
    b2Vec2 vel = this->m_body->GetLinearVelocity();
    vel.y = 0.05;
    this->m_body->SetLinearVelocity(vel);
}
