#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>

class Sprite : public QGraphicsPixmapItem
{
private:

public:
    explicit Sprite(QGraphicsItem *parent = 0);

    // Let's just make this an abstract class
    virtual bool isStatic() = 0;
    virtual bool isAnimated() = 0;
    virtual bool isCollideable() = 0;
    virtual bool isBackground() = 0;
    
signals:
    
public slots:
    
};

#endif // SPRITE_H
