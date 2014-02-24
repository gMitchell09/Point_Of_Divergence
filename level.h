#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsItemGroup>
#include <QFile>
#include <QFileDevice>
#include <QDebug>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>


#include "tilemap.h"
#include "tile.h"

class Level : public QGraphicsItemGroup
{
private:
    QMap<int, TileMap*> m_tileSets;

    // Width, Height in # tiles
    int m_levelWidth, m_levelHeight;
    int m_tileWidth, m_tileHeight;

    void parseMap(QDomElement map);
    void parseTileSet(QDomNode tileset);
    void parseTile(QDomNode tile);
    void parseLayer(QDomNode layer);

    QPointF getTilePos(int tileNum) const;

public:
    explicit Level(QString filePath, QGraphicsItem *parent = 0);

    // Overrides
    virtual QRectF boundingRect() const {
        return QRectF(0, 0, 0, 0);
    }

    virtual QPainterPath shape() const {
        return QPainterPath();
    }

    int getLevelHeight() { return m_levelHeight * m_tileHeight; }
    int getLevelWidth() { return m_levelWidth * m_tileWidth; }

};

#endif // LEVEL_H
