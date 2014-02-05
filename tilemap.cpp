/* Author: George Mitchell
 * Use Case: *
 */

#include "tilemap.h"

TileMap::TileMap(int cellWidth, int cellHeight, int dividerWidth, int dividerHeight, QString pixmapInit) :
    QPixmap(pixmapInit),
    m_cellWidth(cellWidth),
    m_cellHeight(cellHeight),
    m_dividerWidth(dividerWidth),
    m_dividerHeight(dividerHeight) { }

QPixmap TileMap::copyCellAt(unsigned int i, unsigned int j) {
    /* Here is an example image:
     01234567890123456789012345678901
     [   ]|[   ]|[   ]|[   ]|[   ]
     [   ]|[   ]|[   ]|[   ]|[   ]
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
