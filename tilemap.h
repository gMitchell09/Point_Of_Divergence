#ifndef TILEMAP_H
#define TILEMAP_H

#include <QPixmap>
#include <QBitmap>
///
/// \brief The TileMap class is an extension of QPixmap which allows for the selection of specific cells in a tile map.
///
class TileMap : public QPixmap
{
private:
    unsigned int m_cellWidth, m_cellHeight, m_dividerWidth, m_dividerHeight;
public:
    ///
    /// \brief TileMap
    /// \param cellWidth - Width of cells
    /// \param cellHeight - Height of cells
    /// \param dividerWidth - Width of divider (space) (usually 1)
    /// \param dividerHeight - height of divider (space) (usually 1)
    /// \param pixmapInit - QString passed onto QPixmap constructor
    ///
    TileMap(int cellWidth, int cellHeight, int dividerWidth, int dividerHeight, QString pixmapInit);

    ///
    /// \brief copyCellAt will return the specified cell in the Tile Map
    /// \param i - horizontal index of cell
    /// \param j - vertical index of cell
    /// \return QPixmap item which has a transparent background (based on corner color)
    ///
    QPixmap copyCellAt(unsigned int i, unsigned int j);
};

#endif // TILEMAP_H
