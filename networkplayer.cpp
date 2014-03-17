/***************************************************
** Filename: networkplayer.cpp
** Class: NetworkPlayer
** Created: 3/17/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#include "networkplayer.h"

NetworkPlayer::NetworkPlayer(int width, int height, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent)
{
}

void NetworkPlayer::step(qint64 time, long delta) {}
void NetworkPlayer::collisionOccurred(QList<Collision> &collisions, unsigned char side) {}

void NetworkPlayer::decodeDatagram(NetworkManager::DatagramFormat dg) {
    AnimatedCollideableSprite::decodeDatagram(dg);
    this->setPos(dg.pos);
    this->setVelocity(dg.vel);
}
