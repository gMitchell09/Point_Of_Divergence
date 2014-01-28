#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPixmap>
#include <vector>

enum kAnimationType {
    Forward,
    Loop,
    Forward_Reverse,
    Reverse_Forward,
    Forward_Reverse_Loop,
    Reverse_Forward_Loop
};

enum Side { Top = 1, Right = 2, Bottom = 4, Left = 8 };

struct Collision {
    QGraphicsItem* collidee;
    Side side;
};

class AnimatedSprite : public QGraphicsPixmapItem {

private:
    /* Data for animations:
     *  - Path that pixmap will follow for the duration of the animation
     *  - Current Animation, Current Frame of Animation
     *  - List of series' of pixmaps that correspond to each animation that this sprite will contain
     */

    std::vector<QPainterPath> m_animationPathList;
    unsigned char m_nCurrentFrame;
    unsigned char m_nCurrentAnimation;
    std::vector<std::vector<QPixmap>> m_animationList;
    std::vector<kAnimationType> m_animationType;

    QPointF m_acceleration, m_velocity;

    bool m_countUp;
    unsigned int m_msPerFrame;
    unsigned int m_msCounter;

    int m_width, m_height;
    QPoint m_collisionPoints[4][2];

    float m_leftAccel, m_rightAccel, m_maxVelX, m_maxVelY, m_jumpStartVel, m_gravity;

    bool jumping, m_brake;

    /* Static: Unmoving, immobile, velocity=0.
     * Solid: Collisions between two solid objects will cause both the collider (the one that detects the collision in the step)
     *   to be bounced back into its previous position.
     */
    bool m_static;
    bool m_solid;

public:
    explicit AnimatedSprite(int width, int height, QGraphicsItem *parent = 0);
    void triggerAnimation(unsigned int anim);

    void addAnimation(std::vector<QPixmap> pixmapList, QPainterPath animationPath, kAnimationType animationType = Forward);
    void addAnimation(std::vector<QPixmap> pixmapList, kAnimationType animationType = Forward);

    void step(unsigned long time);

    void setAcceleration(QPointF acceleration) { m_acceleration = acceleration; }
    QPointF getAcceleration() { return m_acceleration; }

    void setVelocity(QPointF velocity) { m_velocity = velocity; }
    QPointF getVelocity() { return m_velocity; }

    void setSolid(bool solid) { m_solid = solid; }
    bool isSolid() { return m_solid; }

    QGraphicsItem* collision();
    unsigned char checkForCollision(QList<Collision>& collisions, QPointF offset);

    void jump();

    void setBrake(bool brake) { m_brake = brake; }
    
signals:
    
public slots:
    
};

#endif // ANIMATEDSPRITE_H
