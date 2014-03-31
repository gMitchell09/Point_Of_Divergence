#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsItemGroup>
#include <QFile>
#include <QFileDevice>
#include <QDebug>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>


#include "tile.h"
#include "tilemap.h"
#include "maincharacter.h"
#include "switchobject.h"
#include "door.h"

class TileMap;
class GameEngine;
struct TileProperties;

class Level : public QGraphicsItemGroup
{
private:
    // m_tileSets maps THE UPPER GID contained within the tilemap to the tilemap itself
    QMap<int, TileMap*> m_tileSets;

    // Width, Height in # tiles
    int m_levelWidth, m_levelHeight;
    int m_tileWidth, m_tileHeight;
    GameEngine* m_gameEngine;

    QString m_filePath;

    QString m_bgmPath;

    void parseMap(QDomElement map);
    void parseTileSet(QDomNode tileset);
    void parseTile(QDomNode tile, QMap<int, TileProperties> &tileProperties);
    void parseLayer(QDomNode layer);

    bool parseObject(QDomNode object, ITriggerable*& spr);
    void parseObjectGroup(QDomNode objectgroup);

    QPointF getTilePos(int tileNum) const;

public:
    explicit Level(QString filePath, QString fileName, GameEngine* gameEngine, QGraphicsItem *parent = 0);

    // Overrides to fix QT collision stupidity
    virtual QRectF boundingRect() const {
        return QRectF(0, 0, 0, 0);
    }

    virtual QPainterPath shape() const {
        return QPainterPath();
    }

    // getLevel* returns level size in PIXELS
    int getLevelHeight() { return m_levelHeight * m_tileHeight; }
    int getLevelWidth() { return m_levelWidth * m_tileWidth; }

    QString getBGMPath() { return "/" + m_bgmPath; }
    QString getReversedBGMPath() { return "/REVERSED_" + m_bgmPath; }

    virtual void step(qint64 time, long delta);

};

#endif // LEVEL_H
