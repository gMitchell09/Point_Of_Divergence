/***************************************************
** Filename: networkplayer.h
** Class: NetworkPlayer
** Created: 3/17/2014
** Author: mitchellge
** Use Case #XX
** Use Case Description: Lorem ipsum dolor sit amet
****************************************************/

#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include <animatedcollideablesprite.h>

class NetworkPlayer : public AnimatedCollideableSprite
{
private:
    enum MovementState { Stand_Right = 0, Stand_Left,
                         Walk_Right, Walk_Left,
                         Run_Right, Run_Left,
                         Jump_Right, Jump_Left,
                         Squat_Right, Squat_Left,
                         Brake_Right, Brake_Left};

    MovementState m_currentState;
    // nextState is if you are running right and then pressed the left key... it is for mapping between states
//    MovementState m_nextState;

public:
    explicit NetworkPlayer(int width, int height, b2Body* body = 0, QGraphicsItem *parent = 0);

    virtual void step(qint64 time, long delta);
    virtual void collisionOccurred(QList<Collision> &collisions, unsigned char side);

    virtual void decodeDatagram(NetworkManager::DatagramFormat dg);

    virtual QString className() { return "NetworkPlayer"; }

protected:
    virtual bool usesStack() { return false; }
};

#endif // NETWORKPLAYER_H
