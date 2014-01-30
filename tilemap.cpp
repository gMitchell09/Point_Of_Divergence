#include "tilemap.h"

TileMap::TileMap(int cellWidth, int cellHeight, int dividerWidth, int dividerHeight, QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent),
    m_cellWidth(cellWidth),
    m_cellHeight(cellHeight),
    m_dividerWidth(dividerWidth),
    m_dividerHeight(dividerHeight) { }

QRectF TileMap::boundingRect() const { return QRectF(); }
bool TileMap::contains(const QPointF & point) const { return false; }
bool TileMap::isObscuredBy(const QGraphicsItem * item) const { return false; }
QPainterPath TileMap::opaqueArea() const { return QPainterPath(); }
void TileMap::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {}
QPainterPath TileMap::shape() const { return QPainterPath(); }
