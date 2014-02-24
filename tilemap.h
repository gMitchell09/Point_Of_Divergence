#ifndef TILEMAP_H
#define TILEMAP_H

/* Author: George Mitchell
 * Use Case: *
 */

#include <QPixmap>
#include <QBitmap>

/*!
    \class TileMap
    \brief The TileMap class represents a vector or vertex in 3D space.
    \since Sprint 1
    \ingroup TileMap

    Vectors are one of the main building blocks of 3D representation and
    drawing.  They consist of three coordinates, traditionally called
    x, y, and z.

    The QVector3D class can also be used to represent vertices in 3D space.
    We therefore do not need to provide a separate vertex class.

    \b{Note:} By design values in the QVector3D instance are stored as \c float.
    This means that on platforms where the \c qreal arguments to QVector3D
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa QVector2D, QVector4D, QQuaternion
*/
class TileMap : public QPixmap
{
private:
    unsigned int m_cellWidth, m_cellHeight, m_dividerWidth, m_dividerHeight;
    unsigned int m_tilesWide, m_tilesHigh;
public:
    TileMap() : m_cellWidth(0),
    m_cellHeight(0),
    m_dividerWidth(0),
    m_dividerHeight(0) {}

    ///
    /// \brief TileMap
    /// \param cellWidth - Width of cells
    /// \param cellHeight - Height of cells
    /// \param dividerWidth - Width of divider (space) (usually 1)
    /// \param dividerHeight - height of divider (space) (usually 1)
    /// \param pixmapInit - QString passed onto QPixmap constructor
    ///
    TileMap(int cellWidth, int cellHeight, int dividerWidth, int dividerHeight, QString pixmapInit,
            int tilesHigh = 0, int tilesWide = 0);

    ///
    /// \brief copyCellAt will return the specified cell in the Tile Map
    /// \param i - horizontal index of cell
    /// \param j - vertical index of cell
    /// \return QPixmap item which has a transparent background (based on corner color)
    ///
    QPixmap copyCellAt(unsigned int i, unsigned int j) const;
    QPixmap copyCellAtWithoutMask(unsigned int i, unsigned int j) const;

    QPixmap copyCellAt(unsigned int idx) const;
    QPixmap copyCellAtWithoutMask(unsigned int idx) const;

    inline unsigned int getCellWidth() const { return m_cellWidth; }
    inline unsigned int getCellHeight() const { return m_cellHeight; }
};

#endif // TILEMAP_H
