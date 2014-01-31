#ifndef TILEMAP_H
#define TILEMAP_H

#include <QPixmap>
#include <QBitmap>

class TileMap : public QPixmap
{
private:
    unsigned int m_cellWidth, m_cellHeight, m_dividerWidth, m_dividerHeight;
public:
    TileMap(int cellWidth, int cellHeight, int dividerWidth, int dividerHeight, QString pixmapInit);
    QPixmap copyCellAt(unsigned int i, unsigned int j);
};

#endif // TILEMAP_H
