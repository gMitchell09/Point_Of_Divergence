/* Author: George Mitchell
 * Use Case: *
 */

#include <QDebug>
#include <QPixmap>
#include "gameengine.h"
GameEngine::GameEngine(QObject* parent) : QGraphicsScene(parent), m_mainActor(NULL), m_prevTime(0)
{
}

GameEngine::GameEngine(int width, int height, QObject *parent) :
    QGraphicsScene(parent),
    m_mainActor(NULL),
    m_prevTime(0),
    m_gameTime(0),
    m_hudGameTime(NULL),
    m_coinCount(0),
    m_timeReversed(false),
    m_hud(0),
    m_timeDivider(1),
    m_audioSettings(true),
    m_gamePaused(true),
    m_gamePausedDueToDamage(false) {
    this->setBackgroundBrush(QBrush(QColor(210, 210, 255, 255)));

    heartbeat = new QTimer(this);
    connect(heartbeat, SIGNAL(timeout()), this, SLOT(invalidateTimer()));
    heartbeat->start(1); // 20fps
}

void GameEngine::invalidateTimer() {
    qint64 nMS = QDateTime::currentMSecsSinceEpoch();
    step(nMS);
}

void GameEngine::step(qint64 time) {
    qint64 deltaTime = 0;

    deltaTime = time - m_prevTime;
    if (m_timeReversed) deltaTime = -deltaTime;

    if (m_prevTime == 0) deltaTime = 0;
    deltaTime /= m_timeDivider;


    m_prevTime = time;
    if (m_gamePausedDueToDamage) this->setForegroundBrush(QColor(200, 0, 0, 127));
    if (m_gamePaused) this->setForegroundBrush(QColor(0, 0, 0, 127));
    if (!m_gamePaused && !m_gamePausedDueToDamage) {
        // This is what we will timestamp all history events with so we can have a definite point-of-reference
        //   to accurately play back events.
        m_gameTime += deltaTime;

        for(auto itr = m_spriteArray.begin(); itr != m_spriteArray.end(); itr++) {
            Sprite* spr = dynamic_cast<Sprite*>(*itr);
            if (spr) {
                spr->step(m_gameTime, deltaTime);
            }
        }

        for(auto itr = m_levels.begin(); itr != m_levels.end(); itr++) {
            (*itr)->step(m_gameTime, deltaTime);
        }

        if (m_mainActor != NULL) {
            this->views()[0]->ensureVisible(m_mainActor, 200, 200);
        }

        if (m_hudGameTime != NULL) {
            m_hudGameTime->setText(QString("Time: %1 Coins: %2").arg(m_gameTime/1000, 4, 10, QChar('0')).arg(m_coinCount, 3, 10, QChar('0')));
        }

        if(m_timeReversed){
             this->setForegroundBrush(QColor(255, 255, 255, 127));
        }
        else this->setForegroundBrush(QColor(255, 255, 255, 0));

        this->removeDeletedItems();
    }
}

void GameEngine::keyPressEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == Qt::Key_R) {
        if (m_gamePaused || m_gamePausedDueToDamage)
            m_gamePaused = m_gamePausedDueToDamage = false;
        m_timeReversed = true;
    } else if (keyEvent->key() == Qt::Key_1) {
        m_timeDivider = 2;
    } else if (keyEvent->key() == Qt::Key_2) {
        m_timeDivider = 3;
    } else if (keyEvent->key() == Qt::Key_3) {
        m_timeDivider = 4;
    } else if (keyEvent->key() == Qt::Key_4) {
        m_timeDivider = 5;
    } else if (keyEvent->key() == Qt::Key_Space) {
        m_gamePaused = !m_gamePaused;
    } else if (m_mainActor != NULL) {
        m_mainActor->keyPressEvent(keyEvent);
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent * keyEvent) {
    if (keyEvent->key() == Qt::Key_R) {
        m_timeReversed = false;
    } else if (keyEvent->key() == Qt::Key_1 || keyEvent->key() == Qt::Key_2 || keyEvent->key() == Qt::Key_3 || keyEvent->key() == Qt::Key_4) {
        m_timeDivider = 1;
    } else if (m_mainActor != NULL) {
        m_mainActor->keyReleaseEvent(keyEvent);
    }
}

bool GameEngine::event(QEvent *event) {
    if (m_hud != NULL && event->type() == QEvent::MetaCall) {
        m_hud->step(0, 0);
    }
    return QGraphicsScene::event(event);
}


//iterates through sprite array to remove deleted sprite
void GameEngine::removeItem(QGraphicsItem *item) {
    QGraphicsScene::removeItem(item);
}

void GameEngine::removeItem(Sprite *item) {
    QGraphicsScene::removeItem(item);

    m_deletedItems.push_back(item);
}

void GameEngine::removeDeletedItems() {
    for (auto itr = m_deletedItems.begin(); itr != m_deletedItems.end(); itr++) {
        m_spriteArray.erase(std::remove(m_spriteArray.begin(), m_spriteArray.end(), (*itr)), m_spriteArray.end());
    }

    m_deletedItems.empty();
}

void GameEngine::displayBackground(QColor mycolor) {
    this->setBackgroundBrush(QBrush(mycolor));
}

void GameEngine::displayBackground(QPixmap &bkgPix) {
    bkg = new StaticBackground(QPoint(0, 0));
    bkg->setPixmap(bkgPix);
    bkg->setCollideable(false);
    bkg->setZValue(-1001);
    this->addItem(bkg);
}

void GameEngine::displayInitialMenu() {
    initialMenu = new QGraphicsItemGroup();

    QPixmap *button_static = new QPixmap(":/Buttons/newgame_static.png");
    QPixmap *button_clicked = new QPixmap(":/Buttons/newgame_clicked.png");
    QPixmap *button_hover = new QPixmap(":/Buttons/newgame_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);
    m_newgameButton = new MenuButton(button_static, button_clicked, button_hover);
    m_newgameButton->setPos(this->width()/2-m_newgameButton->boundingRect().width()/2, this->height()/5-m_newgameButton->boundingRect().height()/2);
    std::function<void(void)> func = std::bind(&GameEngine::startSinglePlayer, this);
    m_newgameButton->setCallback(func);

    button_static = new QPixmap(":/Buttons/loadgame_static.png");
    button_clicked = new QPixmap(":/Buttons/loadgame_clicked.png");
    button_hover = new QPixmap(":/Buttons/loadgame_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);
    m_loadgameButton = new MenuButton(button_static, button_clicked, button_hover);
    m_loadgameButton->setPos(this->width()/2-m_loadgameButton->boundingRect().width()/2, this->height()*2/5-m_loadgameButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::displayLoadMenu, this);
    m_loadgameButton->setCallback(func);

    button_static = new QPixmap(":/Buttons/options_static.png");
    button_clicked = new QPixmap(":/Buttons/options_clicked.png");
    button_hover = new QPixmap(":/Buttons/options_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);
    m_optionsButton = new MenuButton(button_static, button_clicked, button_hover);
    m_optionsButton->setPos(this->width()/2-m_optionsButton->boundingRect().width()/2, this->height()*3/5-m_optionsButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::displayOptionsMenu, this);
    m_optionsButton->setCallback(func);

    button_static = new QPixmap(":/Buttons/quit_static.png");
    button_clicked = new QPixmap(":/Buttons/quit_clicked.png");
    button_hover = new QPixmap(":/Buttons/quit_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);
    m_quitButton = new MenuButton(button_static, button_clicked, button_hover);
    m_quitButton->setPos(this->width()/2-m_quitButton->boundingRect().width()/2, this->height()*4/5-m_quitButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::QuitGame, this);
    m_quitButton->setCallback(func);

    initialMenu->addToGroup(m_newgameButton);
    initialMenu->addToGroup(m_loadgameButton);
    initialMenu->addToGroup(m_optionsButton);
    initialMenu->addToGroup(m_quitButton);
    initialMenu->setHandlesChildEvents(false);
    initialMenu->setPos(0, 0);

    optionsMenu = new QGraphicsItemGroup();

    button_static = new QPixmap(":/Buttons/music_static.png");
    button_clicked = new QPixmap(":/Buttons/music_clicked.png");
    m_musicButton = new OptionButton(button_static, button_clicked);
    m_musicButton->setPos(this->width()/2-m_musicButton->boundingRect().width()/2, this->height()/3-m_musicButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::toggleAudio, this);
    m_musicButton->setCallback(func);


    button_static = new QPixmap(":/Buttons/save_static.png");
    button_clicked = new QPixmap(":/Buttons/save_clicked.png");
    button_hover = new QPixmap(":/Buttons/save_hover.png");
    m_saveButton = new MenuButton(button_static, button_clicked, button_hover);
    m_saveButton->setPos(this->width()/3-m_saveButton->boundingRect().width()/2, this->height()*2/3-m_saveButton->boundingRect().height()/2);

    button_static = new QPixmap(":/Buttons/cancel_static.png");
    button_clicked = new QPixmap(":/Buttons/cancel_clicked.png");
    button_hover = new QPixmap(":/Buttons/cancel_hover.png");
    m_cancelButton = new MenuButton(button_static, button_clicked, button_hover);
    m_cancelButton->setPos(this->width()*2/3-m_cancelButton->boundingRect().width()/2, this->height()*2/3-m_cancelButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::modifiedOptionsWarning, this);
    m_cancelButton->setCallback(func);

    optionsMenu->addToGroup(m_musicButton);
    optionsMenu->addToGroup(m_saveButton);
    optionsMenu->addToGroup(m_cancelButton);
    optionsMenu->setHandlesChildEvents(false);
    optionsMenu->setPos(0, 0);

    loadMenu = new QGraphicsItemGroup();

    button_static = new QPixmap(":/Buttons/mainmenu_static.png");
    button_clicked = new QPixmap(":/Buttons/mainmenu_clicked.png");
    button_hover = new QPixmap(":/Buttons/mainmenu_hover.png");
    m_mainmenuButton = new MenuButton(button_static, button_clicked, button_hover);
    m_mainmenuButton->setPos(this->width()/2-m_mainmenuButton->boundingRect().width()/2, this->height()*2/3-m_mainmenuButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::displayMainMenu_load, this);
    m_mainmenuButton->setCallback(func);

    loadMenu->addToGroup(m_mainmenuButton);
    loadMenu->setHandlesChildEvents(false);
    loadMenu->setPos(0, 0);

//    initialMenu->setAcceptedMouseButtons(Qt::AllButtons);
//    initialMenu->setAcceptHoverEvents(true);
//    initialMenu->mousePressEvent(Qt::LeftButton);
    this->addItem(initialMenu);
}

void GameEngine::displayLoadMenu() {
    this->removeItem(initialMenu);
    this->addItem(loadMenu);

    DatabaseController* m_table = DatabaseController::Instance();//
    m_table->buildTables();

    m_table->deleteTable();

//    m_table->addVals("save_table", "name", "score", "Jesse", QString::number(200));
//    m_table->addVals("save_table", "name", "score", "Atley", QString::number(199));
//    m_table->addVals("save_table", "name", "score", "George", QString::number(400));
//    m_table->addVals("save_table", "name", "score", "jesse2", QString::number(1));
//    m_table->addVals("save_table", "name", "score", "atley2", QString::number(2));
//    m_table->addVals("save_table", "name", "score", "george2", QString::number(3));

    std::vector<QString> names, values;
    names.push_back("name");
    names.push_back("score");

    values.push_back("Jesse");
    values.push_back("1000");
    m_table->addSave(names, values);
    values.clear();

    values.push_back("Atley");
    values.push_back("2000");
    m_table->addSave(names, values);

    m_table->readVals("*", "save_table");

    m_table->deleteVals("save_table", "score < 400");

    qDebug() << "------------------------------------------------------";

    m_table->readVals("*", "save_table");
}

void GameEngine::displayOptionsMenu() {
    this->removeItem(initialMenu);
    this->addItem(optionsMenu);
}

void GameEngine::displayMainMenu_load() {
    this->removeItem(loadMenu);
    this->addItem(initialMenu);
}

void GameEngine::displayMainMenu_option() {
    this->removeItem(optionsMenu);
    this->addItem(initialMenu);
}

void GameEngine::startSinglePlayer() {
    this->removeItem(initialMenu);

    StaticPlatform *testSprite, *testSprite2;
    StaticBackground *bkg;
    Sprite *life1, *life2, *life3;
    QGraphicsSimpleTextItem *gameTime;
    ObjectItem *object1;
    MovingPlatform *floater;

    MainCharacter *mainChar;
    Enemy1 *goomba;
    mainChar = new MainCharacter(16, 32);
    mainChar->setPos(120 , 1020);
    mainChar->setSolid(true);

    goomba = new Enemy1(20, 18);
    goomba->setPos(500, 1120);

    floater = new MovingPlatform(48, 64);
    floater->setPixmap(QPixmap(":Simple_Sprite/1.png"));
    floater->setPos(300, 900);
    floater->setVelocity(QPointF(200, 0));
    floater->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    floater->setSolid(true);

    QPixmap myPix1(":Simple_Sprite/1.png");
    QPixmap myPix2(":Simple_Sprite/2.png");
    QPixmap myPix3(":Simple_Sprite/3.png");
    QPixmap myPix4(":Simple_Sprite/4.png");
    QPixmap myPix5(":Simple_Sprite/5.png");

    QPixmap bkgPix(":Backgrounds/background.jpg");
    QPixmap bkgImg(":Backgrounds/sky2.jpg");
    QPixmap bkgMask(":Backgrounds/background_mask.png");

    std::vector<QPixmap> pixmapList2;
    pixmapList2.push_back(myPix1);
    pixmapList2.push_back(myPix2);
    pixmapList2.push_back(myPix3);
    pixmapList2.push_back(myPix4);
    pixmapList2.push_back(myPix5);

    testSprite2 = new StaticPlatform(48, 64);
    testSprite2->addAnimation(pixmapList2, Loop);
    testSprite2->setPos(800, 1020);
    testSprite2->setVelocity(QPointF(0, 0));
    testSprite2->setAcceleration(QPointF(0, 0));
    testSprite2->triggerAnimation(0);
    testSprite2->setSolid(true);

    life1 = new Sprite();
    life1->setPos(QPointF(25, 25));
    life1->setPixmap(QPixmap(":Life/HeartContainer.png"));

    life2 = new Sprite();
    life2->setPos(QPointF(50, 25));
    life2->setPixmap(QPixmap(":Life/HeartContainer.png"));

    life3 = new Sprite();
    life3->setPos(QPointF(75, 25));
    life3->setPixmap(QPixmap(":Life/HeartContainer.png"));

    object1 = new ObjectItem(16, 16);
    object1->setPos(700, 1020);
    object1->setSolid(false);

    gameTime = new QGraphicsSimpleTextItem("Wooo!!!");
    gameTime->setPos(QPointF(630, 15));

    Level *level = new Level("://Levels/LevelTest.tmx", 0);
    level->setPos(QPointF(0, 0));

    this->setSceneRect(0, 0, level->getLevelWidth(), level->getLevelHeight());

    this->addLevel(level);
    this->displayBackground(bkgImg);

    this->addSprite(mainChar, true);
    this->addSprite(testSprite2);
    this->addSprite(floater);
    this->addHUD(life1);
    this->addHUD(life2);
    this->addHUD(life3);
    this->addHUDText(gameTime);

    this->addSprite(object1);
    this->addSprite(goomba); // Add our goomba

    m_gamePaused = false;
}

void GameEngine::characterDamaged() {
    m_gamePausedDueToDamage = true;
    this->setForegroundBrush(QColor(200, 0, 0, 127));
}

void GameEngine::render(QPainter *painter, const QRectF &target, const QRectF &source, Qt::AspectRatioMode aspectRatioMode) {
//    if (painter->testRenderHint(QPainter::Antialiasing)) {
//        painter->setRenderHint(QPainter::Antialiasing, true);
//        qDebug() << "Antialising toggled on";
//    } else {
//        painter->setRenderHint(QPainter::Antialiasing, off);
//        qDebug() << "Antialiasing toggled off";
//    }
//    QGraphicsScene::render(painter, target, source, aspectRatioMode);
//------------------
//    painter->setRenderHint(QPainter::Antialiasing, m_videoSettings);
//    QGraphicsScene::render(painter, target, source, aspectRatioMode);
}

void GameEngine::toggleAudio() {
    m_audioSettings = !m_audioSettings;
    qDebug() << m_audioSettings;
}

void GameEngine::modifiedOptionsWarning() {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("The settings have been modified.");
    msgBox.setInformativeText("Do you want to save or discard your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:
        qDebug() << "The modified settings are saved.";
        this->saveSettings();
        break;
    case QMessageBox::Discard:
        qDebug() << "The modified settings are discarded.";
        this->displayMainMenu_option();
        break;
    }
}

void GameEngine::saveSettings() {
    //...
    this->displayMainMenu_option();
}

void GameEngine::QuitGame() {
    exit(0);
}

