#include "level.h"

Level::Level(QString filePath, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent) {
    QFile tileFile(filePath);
    if(!tileFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Couldn't open file";
    }

    QDomDocument levelXML;
    QString errorStr;
    int errorLine, errorCol;
    if(!levelXML.setContent(&tileFile, false, &errorStr, &errorLine, &errorCol)) {
        qDebug() << "Error: Parse error at line " << errorLine << ", "
                    << "column " << errorCol << ": " <<
                       errorStr;
    } else {
        QDomElement root = levelXML.documentElement();
        this->parseMap(root);
    }
}

void Level::parseMap(QDomElement map) {
    m_tileWidth = map.attribute("tilewidth", "0").toInt();
    m_tileHeight = map.attribute("tileheight", "0").toInt();
    m_levelWidth = map.attribute("width", "0").toInt();
    m_levelHeight = map.attribute("height", "0").toInt();

    QDomNode child = map.firstChild();
    while (!child.isNull()) {
        if (child.toElement().tagName() == "tileset") {
            this->parseTileSet(child);
        } else if (child.toElement().tagName() == "layer") {
            this->parseLayer(child);
        } else {
            qDebug() << "Unrecognized 2nd level tag: " << child.toElement().tagName();
        }
        child = child.nextSibling();
    }
}

void Level::parseTileSet(QDomNode tileset) {
    QString tilemapName = tileset.toElement().attribute("name", "");
    QString tilemapPath;
    int cellWidth, cellHeight, dividerWidth, dividerHeight, tileMapWidth, tileMapHeight;

    QDomElement tileElement = tileset.toElement();

    cellWidth = tileElement.attribute("tilewidth", "0").toInt();
    cellHeight = tileElement.attribute("tileheight", "0").toInt();

    dividerWidth = tileElement.attribute("spacing", "0").toInt();
    dividerHeight = tileElement.attribute("spacing", "0").toInt();

    int firstGID = tileElement.attribute("firstgid", "1").toInt();

    QDomNode child = tileset.firstChild();
    while (!child.isNull()) {
        if (child.toElement().tagName() == "image") {
            tilemapPath = child.toElement().attribute("source", "");
            tileMapWidth = child.toElement().attribute("width", "0").toInt();
            tileMapHeight = child.toElement().attribute("height", "0").toInt();

        } else { qDebug() << "Eh..."; }
        child = child.nextSibling();
    }

    // width = m*cellWidth + (m-1)*dividerWidth
    // tiles_wide = (width+dividerWidth)/(cellWidth+dividerWidth)
    int tiles_wide = (tileMapWidth+dividerWidth)/(cellWidth+dividerWidth);
    int tiles_high = (tileMapHeight+dividerHeight)/(cellHeight+dividerHeight);

    int lastGID = firstGID + tiles_wide * tiles_high - 1;

    qDebug() << "Tile Map Path: " << ":/Levels/" + tilemapPath;
    qDebug() << "Should be: " << "://Levels/PlatformTiles_brownNature_ByEris_0/Tiles 32x32/Tiles_32x32.png";

    TileMap *tileMap = new TileMap(cellWidth, cellHeight,
                                   dividerWidth, dividerHeight,
                                   ":/Levels/" + tilemapPath, tiles_wide, tiles_high);

    // This is so evil.  Do it this way so we can use ::lower_bound() to
    //  determine correct tileset for range of tiles
    m_tileSets.insert(lastGID, tileMap);
}

void Level::parseLayer(QDomNode layer) {
    QDomNode child = layer.firstChild().firstChild(); // ugly
    int currentTile = 0;
    int validTiles = 0;
    int firstElement = 0;
    while (!child.isNull()) {
        int gid;
        if ((gid = child.toElement().attribute("gid", "0").toInt()) != 0) {
            auto itr = m_tileSets.lowerBound(gid);

            if (itr == m_tileSets.begin()) firstElement = 0;
            else firstElement = (itr-1).key();

            TileMap* tileMap = *itr;
            QPointF pos = this->getTilePos(currentTile);
            // Normalize our tile index by getting our tile index relative to
            //  our current tilemap.
            int idx = gid - firstElement;
            QPixmap tileImage = tileMap->copyCellAtWithoutMask(idx - 1);
            Tile *tile = new Tile(tileMap->getCellWidth(), tileMap->getCellHeight(), this);

            qDebug() << "Idx: " << idx << "\nTile: " << tileImage << "Pos: " << pos;

            tile->setPixmap(tileImage);
            tile->setSolid(true);
            tile->setBlockType(ItemType::kBlock);
            tile->setPos(pos);
            tile->setShapeMode(Tile::BoundingRectShape);

            this->addToGroup(tile);
            validTiles++;
        }
        currentTile++;
        child = child.nextSibling();
    }
    qDebug() << "nTiles = " << currentTile;
    qDebug() << "nValid = " << validTiles;
}

QPointF Level::getTilePos(int tileNum) const {
    int x = tileNum % m_levelWidth;
    int y = tileNum / m_levelWidth;

    return QPointF(m_tileWidth * x, m_tileHeight * y);
}
