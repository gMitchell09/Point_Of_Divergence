/***************************************************
** Filename: networkplayer.cpp
** Class: NetworkPlayer
** Created: 3/17/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#include "tilemap.h"
#include "networkplayer.h"

NetworkPlayer::NetworkPlayer(int width, int height, b2Body* body, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, body, parent)
{
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

    this->triggerAnimation(Stand_Right);
}

void NetworkPlayer::step(qint64 time, long delta) {
    AnimatedCollideableSprite::step(time, delta);
}

void NetworkPlayer::collisionOccurred(AnimatedCollideableSprite* other, Side side) {
    AnimatedCollideableSprite::collisionOccurred(other, side);
}

void NetworkPlayer::decodeDatagram(NetworkManager::DatagramFormat dg) {
    AnimatedCollideableSprite::decodeDatagram(dg);

    this->setPosition(dg.pos, 0);
    this->setVelocity(dg.vel);
}
