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
    m_gameEngine(gameEngine),
    m_filePath(filePath),
    m_world(gameEngine->getWorld()) {
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

    b2BodyDef groundBodyDef;
    //groundBodyDef.position.Set(PX_TO_M(m_levelWidth), PX_TO_M(-m_levelHeight));
    groundBodyDef.type = b2_staticBody;
    b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
    groundBody->SetUserData((void*)nullptr);

    b2PolygonShape tileShape;
    tileShape.SetAsBox(PX_TO_M(32)/2, PX_TO_M(32)/2);

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
                b2BodyDef bodyDef;
                bodyDef.position.Set(PX_TO_M(pos.x()), PX_TO_M(-pos.y()));
                bodyDef.type = b2_dynamicBody;
                b2Body* body = m_world->CreateBody(&bodyDef);
                b2PolygonShape mcShape;
                b2Vec2 verts[] = {
//                    b2Vec2(-0.95,1.0),
//                    b2Vec2(-1.0,0.95),
//                    b2Vec2(-1.0,-0.95),
//                    b2Vec2(-0.95,-1.0),
//                    b2Vec2(0.95,-1.0),
//                    b2Vec2(1.0,-0.95),
//                    b2Vec2(1.0,0.95),
//                    b2Vec2(0.95,1.0)
                    b2Vec2(0.55, 0.0),
                    b2Vec2(0.5, -0.05),
                    b2Vec2(0.5, -1.95),
                    b2Vec2(0.55, -2.0),
                    b2Vec2(1.45, -2),
                    b2Vec2(1.5, -1.95),
                    b2Vec2(1.5, -0.95),
                    b2Vec2(1.45, 0.0)
                };
                mcShape.Set(verts, 8);
                //mcShape.SetAsBox(PX_TO_M(16.)/2, PX_TO_M(32.)/2,
                //                 b2Vec2(PX_TO_M(16.), -PX_TO_M(32.)/2), 0);
                body->SetFixedRotation(true);
                body->CreateFixture(&mcShape, 1.0f)->SetFriction(0.);

                MainCharacter *mainChar = new MainCharacter(16, 32, body);
                mainChar->setPos(pos);
                mainChar->setSolid(true);

                body->SetUserData(mainChar);
                m_gameEngine->addSprite(mainChar, true);

            } else if (tp.kind == kGoomba) {
                b2BodyDef bodyDef;
                bodyDef.position.Set(PX_TO_M(pos.x()), PX_TO_M(-pos.y()));
                bodyDef.type = b2_dynamicBody;
                b2Body* body = m_world->CreateBody(&bodyDef);
                b2PolygonShape shape;
                shape.SetAsBox(PX_TO_M(20)/2., PX_TO_M(18)/2.,
                               b2Vec2(PX_TO_M(20.), -PX_TO_M(18.0)/2), 0);
                body->CreateFixture(&shape, 1.0f);

                Enemy1 *goomba = new Enemy1(20, 18, enemyPath, body);
                goomba->setPos(pos);

                body->SetUserData(goomba);
                this->addToGroup(goomba);
            } else if (tp.kind == kLever) {
                b2BodyDef bodyDef;
                bodyDef.position.Set(PX_TO_M(pos.x()), PX_TO_M(-pos.y()));
                bodyDef.type = b2_dynamicBody;
                b2Body* body = m_world->CreateBody(&bodyDef);
                b2PolygonShape shape;
                shape.SetAsBox(PX_TO_M(32)/2., PX_TO_M(32)/2.,
                               b2Vec2(PX_TO_M(32)/2, -PX_TO_M(32.)/2), 0);
                body->CreateFixture(&shape, 1.0f);

                SwitchObject *switchObj = new SwitchObject(32, 32, body);
                switchObj->setPixmaps(tileMap->copyCellAtWithoutMask(idx), tileMap->copyCellAtWithoutMask(idx + 1));
                switchObj->setPos(pos);

                body->SetUserData(switchObj);
                this->addToGroup(switchObj);
            } else if (tp.kind == kCoin) {
                b2BodyDef bodyDef;
                bodyDef.position.Set(PX_TO_M(pos.x()), PX_TO_M(-pos.y()));
                bodyDef.type = b2_staticBody;
                b2Body* body = m_world->CreateBody(&bodyDef);
                b2PolygonShape shape;
                shape.SetAsBox(PX_TO_M(32)/2., PX_TO_M(32)/2.,
                               b2Vec2(PX_TO_M(32.)/2, -PX_TO_M(32.)/2), 0);
                body->CreateFixture(&shape, 1.0f)->SetSensor(true);

                QPixmap tileImage = tileMap->copyCellAtWithoutMask(idx);
                Tile *coin = new Tile(tileMap->getCellWidth(), tileMap->getCellHeight(), body, this);
                coin->setSolid(tp.solid);
                coin->setBlockType(tp.kind);
                coin->setPixmap(tileImage);
                coin->setPos(pos);
                coin->setShapeMode(Tile::BoundingRectShape);

                body->SetUserData(coin);
                this->addToGroup(coin);
            } else {
                b2Body* body = nullptr;
                tileShape.SetAsBox(PX_TO_M(tileMap->getCellWidth()/2),
                                   PX_TO_M(tileMap->getCellHeight()/2),
                                   b2Vec2(PX_TO_M(pos.x() + tileMap->getCellWidth()/2),
                                   PX_TO_M(-pos.y() - tileMap->getCellHeight()/2)), 0);

                if (tp.kind == kBox) {
                    b2BodyDef bodyDef;
                    bodyDef.position.Set(PX_TO_M(pos.x() - 32./2), PX_TO_M(-pos.y()));
                    bodyDef.type = b2_dynamicBody;
                    bodyDef.fixedRotation = false;
                    body = m_world->CreateBody(&bodyDef);
                    b2PolygonShape shape;
                    shape.SetAsBox(PX_TO_M(32.)/2., PX_TO_M(32.)/2.,
                                   b2Vec2(PX_TO_M(32.), -PX_TO_M(32.)/2), 0);
                    body->CreateFixture(&shape, 1.0f);
                } else if (tp.kind == kSlope45Right) {
                    float32 xMeters = PX_TO_M(pos.x());
                    float32 yMeters = PX_TO_M(-pos.y());
                    b2Vec2 verts[] = {
                        b2Vec2(xMeters + 0.0, yMeters - 2.0),
                        b2Vec2(xMeters + 2.0, yMeters - 2.0),
                        b2Vec2(xMeters + 2.0, yMeters + 0.0)
                    };
                    tileShape.Set(verts, 3);
                    groundBody->CreateFixture(&tileShape, 0.0f);
                } else if (tp.kind == kSlope45Left) {
                    float32 xMeters = PX_TO_M(pos.x());
                    float32 yMeters = PX_TO_M(-pos.y());
                    b2Vec2 verts[] = {
                        b2Vec2(xMeters + 0.0, yMeters + 0.0),
                        b2Vec2(xMeters + 0.0, yMeters - 2.0),
                        b2Vec2(xMeters + 2.0, yMeters - 2.0)
                    };
                    tileShape.Set(verts, 3);
                    groundBody->CreateFixture(&tileShape, 0.0f);
                    // TODO: Set slope friction on the fixture
                } else if (tp.kind == kSlope30Right) {
                    float32 xMeters = PX_TO_M(pos.x());
                    float32 yMeters = PX_TO_M(-pos.y());
                    b2Vec2 verts[] = {
                        b2Vec2(xMeters + 0.0, yMeters - 2.0),
                        b2Vec2(xMeters + 2.0, yMeters - 2.0),
                        b2Vec2(xMeters + 2.0, yMeters - 1.0)
                    };
                    tileShape.Set(verts, 3);
                    groundBody->CreateFixture(&tileShape, 0.0f);
                } else if (tp.kind == kSlope30Left) {
                    float32 xMeters = PX_TO_M(pos.x());
                    float32 yMeters = PX_TO_M(-pos.y());
                    b2Vec2 verts[] = {
                        b2Vec2(xMeters + 0.0, yMeters - 1.0),
                        b2Vec2(xMeters + 0.0, yMeters - 2.0),
                        b2Vec2(xMeters + 2.0, yMeters - 2.0)
                    };
                    tileShape.Set(verts, 3);
                    groundBody->CreateFixture(&tileShape, 0.0f);
                } else if (tp.kind == kSlope60Right) {
                    float32 xMeters = PX_TO_M(pos.x());
                    float32 yMeters = PX_TO_M(-pos.y());
                    b2Vec2 verts[] = {
                        b2Vec2(xMeters + 0.0, yMeters - 1.0),
                        b2Vec2(xMeters + 2.0, yMeters - 1.0),
                        b2Vec2(xMeters + 2.0, yMeters - 0.0)
                    };
                    tileShape.Set(verts, 3);
                    groundBody->CreateFixture(&tileShape, 0.0f);
                } else if (tp.kind == kSlope60Left) {
                    float32 xMeters = PX_TO_M(pos.x());
                    float32 yMeters = PX_TO_M(-pos.y());
                    b2Vec2 verts[] = {
                        b2Vec2(xMeters + 0.0, yMeters + 0.0),
                        b2Vec2(xMeters + 0.0, yMeters - 1.0),
                        b2Vec2(xMeters + 2.0, yMeters - 1.0)
                    };
                    tileShape.Set(verts, 3);
                    groundBody->CreateFixture(&tileShape, 0.0f);
                } else if (tp.solid) {
                    groundBody->CreateFixture(&tileShape, 0.0f);
                    //body = groundBody;
                } else {
                    groundBody->CreateFixture(&tileShape, 0.0f)->SetSensor(true);
                }

                // We have an actual tile and not a "special" tile, i.e. goomba, mainchar, ...
                QPixmap tileImage = tileMap->copyCellAtWithoutMask(idx);

                tile = new Tile(tileMap->getCellWidth(), tileMap->getCellHeight(), body);
                tile->setSolid(tp.solid);
                tile->setBlockType(tp.kind);

                tile->setPixmap(tileImage);

                tile->setPos(pos);
                tile->setShapeMode(Tile::BoundingRectShape);\

                if (body && tp.kind == kBox) {
                    body->SetUserData(tile);
                }

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
    std::vector<ITriggerable*> controllers;

    QDomNode child = objectGroup.firstChild();

    while (!child.isNull()) {
        ITriggerable* obj = this->parseObject(child);
        if (obj == nullptr) return;

        if (obj->isController()) {
            controllers.push_back(obj);
        } else {
            receivers.push_back(obj);
        }
        child = child.nextSibling();
    }

    if (!controllers.empty() && !receivers.empty()) {
        for (auto itr = receivers.begin(); itr != receivers.end(); ++itr) {
            for (auto contItr = controllers.begin(); contItr != controllers.end(); ++contItr) {
                ITriggerable* rcvInst = dynamic_cast<ITriggerable*>(*itr);
                ITriggerable* contInst = dynamic_cast<ITriggerable*>(*contItr);

                QMetaObject::Connection conn = QObject::connect(contInst, SIGNAL(stateChanged(bool)), rcvInst, SLOT(controllerStateChanged(bool)));
                if (!((bool)conn)) {
                    qDebug() << "WTF";
                }
            }
        }
    }
}

ITriggerable* Level::parseObject(QDomNode object) {
    ITriggerable *spr = nullptr;
    int gid = object.toElement().attribute("gid", "0").toInt();
    int x = object.toElement().attribute("x", "0").toInt();
    int y = object.toElement().attribute("y", "0").toInt() - 32; // -32 Because of what looks like a bug in Tiled... boo...
    ItemType type = (ItemType)object.toElement().attribute("name", "0").toInt();
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
        case kLeverFlipped: {
            SwitchObject* switchObject = new SwitchObject(32, 32);
            switchObject->setPixmaps(tileMap->copyCellAtWithoutMask(idx - 1), tileMap->copyCellAtWithoutMask(idx));
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

        case kDoorOpen: {
            Door *door = new Door(32, 32);
            door->setPixmaps(tileMap->copyCellAtWithoutMask(idx - 1), tileMap->copyCellAtWithoutMask(idx));
            door->setPos(x, y);
            door->controllerStateChanged(true);
            this->addToGroup(door);
            spr = door;
            break;
        }

        default:
            qDebug() << "Invalid Object!!!";
    }
    return spr;
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
