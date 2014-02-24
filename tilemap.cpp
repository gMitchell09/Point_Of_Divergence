/* Author: George Mitchell
 * Use Case: *
 */

#include "tilemap.h"
#include <QDebug>
TileMap::TileMap(int cellWidth, int cellHeight, int dividerWidth, int dividerHeight, QString pixmapInit,
                 int tilesWide, int tilesHigh) :
    QPixmap(pixmapInit),
    m_cellWidth(cellWidth),
    m_cellHeight(cellHeight),
    m_dividerWidth(dividerWidth),
    m_dividerHeight(dividerHeight),
    m_tilesWide(tilesWide),
    m_tilesHigh(tilesHigh) {
    qDebug() << "TileMap: " << this->size();
}

QPixmap TileMap::copyCellAt(unsigned int i, unsigned int j) const {
    /* Here is an example image:
     01234567890123456789012345678901
     [0 0]|[0 0]|[0 0]|[0 0]|[_ _]|[^ ^]
     [ u ]|[ n ]|[ _ ]|[|o|]|[ ~ ]|[ _ ]
     -----------------------------
     ...
     */
    int boxLeft, boxTop, boxWidth, boxHeight;

    boxLeft = i * m_cellWidth + i * m_dividerWidth;
    boxWidth = m_cellWidth;
    boxTop = j * m_cellHeight + j * m_dividerHeight;
    boxHeight = m_cellHeight;

    QPixmap copyPix = this->copy(boxLeft, boxTop, boxWidth, boxHeight);
    copyPix.setMask(copyPix.createHeuristicMask(true));

    return copyPix;
}

QPixmap TileMap::copyCellAt(unsigned int idx) const {
    return this->copyCellAt(idx % m_tilesWide, idx / m_tilesHigh);
}
