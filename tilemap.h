#ifndef TILEMAP_H
#define TILEMAP_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPainter>

class TileMap : public QGraphicsPixmapItem
{
private:
    unsigned int m_cellWidth, m_cellHeight, m_dividerWidth, m_dividerHeight;
public:
    explicit TileMap(int cellWidth, int cellHeight, int dividerWidth, int dividerHeight, QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;
    virtual bool contains(const QPointF & point) const;
    virtual bool isObscuredBy(const QGraphicsItem * item) const;
    virtual QPainterPath opaqueArea() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    virtual QPainterPath shape() const;
};

#endif // TILEMAP_H
