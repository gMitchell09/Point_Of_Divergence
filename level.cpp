#include "level.h"

static const QString bkgPath = "./resources/backgrounds";
static const QString buttonPath = "./resources/buttons";
// No dot for bgmPath because we need the absolute path.
static const QString bgmPath = "/resources/bgm";
static const QString sfxPath = "./resources/sfx";
static const QString levelPath = "./resources/levels";
static const QString spritePath = "./resources/sprites";
static const QString tilesetPath = "./resources/tilesets";
static const QString enemyPath = spritePath + "/enemies";
static const QString itemPath = spritePath + "/items";
static const QString otherSpritePath = spritePath + "/other";

Level::Level(QString filePath, QString fileName, GameEngine *gameEngine, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    m_filePath(filePath),
    m_gameEngine(gameEngine) {
    QFile tileFile(filePath + "/" + fileName);
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

    this->setHandlesChildEvents(false);
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
        } else if (child.toElement().tagName() == "properties") {
            if (child.hasChildNodes()) {
                if (child.firstChild().toElement().tagName() == "property" &&
                    child.firstChild().toElement().attribute("name") == "bgm") {
                    // SET BGM MEMBER VARIABLE TO child element value
                    m_bgmPath = child.firstChild().toElement().attribute("value");
                }
            }
        } else if (child.toElement().tagName() == "objectgroup") {
            this->parseObjectGroup(child);
        } else {
            qDebug() << "Unrecognized 2nd level tag: " << child.toElement().tagName();
        }
        child = child.nextSibling();
    }
}

void Level::parseTileSet(QDomNode tileset) {
    QString tilemapName = tileset.toElement().attribute("name", "");
    QString tilemapPath;
    QMap<int, TileProperties> tp;
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

        } else if (child.toElement().tagName() == "tile" ) {
            // This tile has special properties... let's see what they are!!
            this->parseTile(child, tp);
        } else { qDebug() << "Eh..." << child.toElement().tagName(); }

        // Now let's check the next child object
        child = child.nextSibling();
    }

    // width = m*cellWidth + (m-1)*dividerWidth
    // tiles_wide = (width+dividerWidth)/(cellWidth+dividerWidth)
    int tiles_wide = (tileMapWidth+dividerWidth)/(cellWidth+dividerWidth);
    int tiles_high = (tileMapHeight+dividerHeight)/(cellHeight+dividerHeight);

    int lastGID = firstGID + tiles_wide * tiles_high - 1;

    TileMap *tileMap = new TileMap(cellWidth, cellHeight,
                                   dividerWidth, dividerHeight,
                                   m_filePath + "/" + tilemapPath, tiles_wide, tiles_high);

    qDebug() << "Tilemap Path: " << m_filePath + "/" + tilemapPath;

    tileMap->m_tileProperties = tp;

    // This is so evil.  Do it this way so we can use ::lower_bound() to
    //  determine correct tileset for range of tiles
    m_tileSets.insert(lastGID, tileMap);
}

void Level::parseLayer(QDomNode layer) {
    QDomNode child = layer.firstChild().firstChild(); // ugly
    int currentTile = 0;
    int validTiles = 0;
    int firstElement = 1;
    while (!child.isNull()) {
        int gid = child.toElement().attribute("gid", "0").toInt();
        if (gid != 0) {
            auto itr = m_tileSets.lowerBound(gid);

            if (itr == m_tileSets.begin()) firstElement = 1;
            else firstElement = (itr-1).key() + 1;
            // For line above:  We add one because the keys are the final index held
            //  in the mapped spritesheet

            TileMap* tileMap = *itr;
            QPointF pos = this->getTilePos(currentTile);
            // Normalize our tile index by getting our tile index relative to
            //  our current tilemap.
            int idx = gid - firstElement;

            Tile *tile = NULL;
            TileProperties tp;

            if (tileMap->m_tileProperties.contains(idx)) {
                tp = tileMap->m_tileProperties[idx];
            } else {
                tp.solid = true;
                tp.kind = kBlock;
            }

            if (tp.kind == kMainCharacter) {
                MainCharacter *mainChar = new MainCharacter(16, 32);
                mainChar->setPos(pos);
                mainChar->setSolid(true);
                m_gameEngine->addSprite(mainChar, true);
            } else if (tp.kind == kGoomba) {
                Enemy1 *goomba = new Enemy1(20, 18, enemyPath);
                goomba->setPos(pos);
                this->addToGroup(goomba);
            } else if (tp.kind == kLever) {
                SwitchObject *switchObj = new SwitchObject(32, 32);
                switchObj->setPixmaps(tileMap->copyCellAtWithoutMask(idx), tileMap->copyCellAtWithoutMask(idx + 1));
                switchObj->setPos(pos);
                this->addToGroup(switchObj);
            } else {
                // We have an actual tile and not a "special" tile, i.e. goomba, mainchar, ...
                QPixmap tileImage = tileMap->copyCellAtWithoutMask(idx);

                tile = new Tile(tileMap->getCellWidth(), tileMap->getCellHeight(), this);
                tile->setSolid(tp.solid);
                tile->setBlockType(tp.kind);

                tile->setPixmap(tileImage);

                tile->setPos(pos);
                tile->setShapeMode(Tile::BoundingRectShape);
                this->addToGroup(tile);

            }
            validTiles++;
        }
        currentTile++;
        child = child.nextSibling();
    }
    qDebug() << "nTiles = " << currentTile;
    qDebug() << "nValid = " << validTiles;
}

void Level::parseTile(QDomNode tile, QMap<int, TileProperties> &tileProperties) {
    int id = tile.toElement().attribute("id", "0").toInt();
    if (tileProperties.contains(id)) return;

    TileProperties tp;

    QDomNode child = tile.firstChild();
    if (child.toElement().tagName() == "properties") child = child.firstChild();
    else { qDebug() << "Fuck"; return; }

    while (!child.isNull()) {
        QDomElement property = child.toElement();
        if (property.tagName() == "property") {
            if (property.attribute("name") == "solid") {
                tp.solid = property.attribute("value", "1").toInt() != 0;
            } else if (property.attribute("name") == "type") {
                tp.kind = (ItemType) property.attribute("value", "0").toInt();
            } else qDebug() << "Unknown property: " << property.attribute("name");
        }
        child = child.nextSibling();
    }
    tileProperties.insert(id, tp);
}

void Level::parseObjectGroup(QDomNode objectGroup) {
    std::vector<ITriggerable*> receivers;
    ITriggerable* controller = nullptr;

    QDomNode child = objectGroup.firstChild();

    while (!child.isNull()) {
        ITriggerable* obj;
        if (this->parseObject(child, obj)) {
            controller = obj;
        } else {
            receivers.push_back(obj);
        }
        child = child.nextSibling();
    }

    if (controller != nullptr && !receivers.empty()) {
        for (auto itr = receivers.begin(); itr != receivers.end(); ++itr) {
            ITriggerable* obj = dynamic_cast<ITriggerable*>(*itr);
            QMetaObject::Connection conn = QObject::connect(controller, SIGNAL(stateChanged(bool)), obj, SLOT(controllerStateChanged(bool)));
            if (!((bool)conn)) {
                qDebug() << "WTF";
            }
        }
    }
}

bool Level::parseObject(QDomNode object, ITriggerable*& spr) {
    int gid = object.toElement().attribute("gid", "0").toInt();
    int x = object.toElement().attribute("x", "0").toInt();
    int y = object.toElement().attribute("y", "0").toInt() - 32; // -32 Because of what looks like a bug in Tiled... boo...
    QString role = object.toElement().attribute("name", "Receiver");
    ItemType type = (ItemType)object.toElement().attribute("type", "0").toInt();
    int firstElement = 1;
    auto itr = m_tileSets.lowerBound(gid);

    if (itr == m_tileSets.begin()) firstElement = 1;
    else firstElement = (itr-1).key() + 1;
    // For line above:  We add one because the keys are the final index held
    //  in the mapped spritesheet

    TileMap* tileMap = *itr;

    // Normalize our tile index by getting our tile index relative to
    //  our current tilemap.
    int idx = gid - firstElement;

    switch (type) {
        case kLever: {
            SwitchObject* switchObject = new SwitchObject(32, 32);
            switchObject->setPixmaps(tileMap->copyCellAtWithoutMask(idx), tileMap->copyCellAtWithoutMask(idx + 1));
            switchObject->setPos(x, y);
            this->addToGroup(switchObject);
            spr = switchObject;
            break;
        }
        case kDoor: {
            Door *door = new Door(32, 32);
            door->setPixmaps(tileMap->copyCellAtWithoutMask(idx), tileMap->copyCellAtWithoutMask(idx + 1));
            door->setPos(x, y);
            this->addToGroup(door);
            spr = door;
            break;
        }
        default:
            qDebug() << "Invalid Object!!!";
    }

    if (role == "Controller") return true;
    return false;
}

QPointF Level::getTilePos(int tileNum) const {
    int x = tileNum % m_levelWidth;
    int y = tileNum / m_levelWidth;

    return QPointF(m_tileWidth * x, m_tileHeight * y);
}

void Level::step(qint64 time, long delta) {
    for (int i = 0; i < this->childItems().size(); i++) {
        ((Tile*)this->childItems()[i])->step(time, delta);
    }
}
