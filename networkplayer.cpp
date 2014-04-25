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
    this->setSolid(true);

    TileMap * playerTiles = new TileMap(32, 32, 0, 0, "./resources/sprites/mainCharClone.png");


    std::vector<QPixmap> standRight;
    standRight.push_back(playerTiles->copyCellAt(0, 0));

    std::vector<QPixmap> standLeft;
    standLeft.push_back(playerTiles->copyCellAt(4, 0));

    std::vector<QPixmap> walkRight;
    walkRight.push_back(playerTiles->copyCellAt(0, 0));
    walkRight.push_back(playerTiles->copyCellAt(1, 0));
    walkRight.push_back(playerTiles->copyCellAt(2, 0));
    walkRight.push_back(playerTiles->copyCellAt(3, 0));

    std::vector<QPixmap> walkLeft;
    walkLeft.push_back(playerTiles->copyCellAt(7, 0));
    walkLeft.push_back(playerTiles->copyCellAt(6, 0));
    walkLeft.push_back(playerTiles->copyCellAt(5, 0));
    walkLeft.push_back(playerTiles->copyCellAt(4, 0));

    std::vector<QPixmap> runRight;
    runRight.push_back(playerTiles->copyCellAt(0, 1));
    runRight.push_back(playerTiles->copyCellAt(1, 1));
    runRight.push_back(playerTiles->copyCellAt(2, 1));

    std::vector<QPixmap> runLeft;
    runLeft.push_back(playerTiles->copyCellAt(5, 1));
    runLeft.push_back(playerTiles->copyCellAt(4, 1));
    runLeft.push_back(playerTiles->copyCellAt(3, 1));

    std::vector<QPixmap> jumpRight;
    jumpRight.push_back(playerTiles->copyCellAt(0, 2));
    jumpRight.push_back(playerTiles->copyCellAt(1, 2));
    jumpRight.push_back(playerTiles->copyCellAt(2, 2));

    std::vector<QPixmap> jumpLeft;
    jumpLeft.push_back(playerTiles->copyCellAt(5, 2));
    jumpLeft.push_back(playerTiles->copyCellAt(4, 2));
    jumpLeft.push_back(playerTiles->copyCellAt(3, 2));

    std::vector<QPixmap> climb;
    climb.push_back(playerTiles->copyCellAt(0, 3));
    climb.push_back(playerTiles->copyCellAt(1, 3));

    this->addAnimation(standRight, Loop);
    this->addAnimation(standLeft, Loop);

    this->addAnimation(walkRight, Loop);
    this->addAnimation(walkLeft, Loop);

    this->addAnimation(runRight, Loop);
    this->addAnimation(runLeft, Loop);

    this->addAnimation(jumpRight, Forward);
    this->addAnimation(jumpLeft, Forward);

    this->addAnimation(climb, Loop);

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
