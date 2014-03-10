/* Author: George Mitchell
 * Use Case: *
 */

#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <QApplication>
#include "sfxmanager.h"
#include "gameengine.h"

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
    m_gamePaused(false),
    m_gamePausedDueToDamage(false) {
    this->setBackgroundBrush(QBrush(QColor(210, 210, 255, 255)));

    qDebug() << "Level Path: " << levelPath + "/LevelTest.tmx";
    QFile file(levelPath + "/LevelTest.tmx");

    m_mediaPlayer = new QMediaPlayer(this);
    m_mediaPlayerReverse = new QMediaPlayer(this);
    // Pre-load the SFX
    SFXManager::Instance();

    if (!file.exists()) {
        QMessageBox *msg = new QMessageBox("Level not found!", "Level Not Found!!", QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        msg->show();
    }
}

GameEngine::~GameEngine() {
    delete m_mainActor;
    delete m_bkg;
    delete m_initialMenu;
    delete m_loadMenu;
    delete m_optionsMenu;

    delete m_hud;
    delete m_hudGameTime;
    delete heartbeat;

    for (auto itr = m_spriteArray.begin(); itr != m_spriteArray.end(); ++itr) {
        this->removeItem(*itr);
    }
    m_spriteArray.clear();

    for (auto itr = m_deletedItems.begin(); itr != m_deletedItems.end(); ++itr) {
        delete *itr;
    }

    for (auto itr = m_levels.begin(); itr != m_levels.end(); ++itr) {
        this->removeItem(*itr);
    }
    m_levels.clear();
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
    if (keyEvent->isAutoRepeat()) return;
    if (keyEvent->key() == Qt::Key_R) {
        if (m_gamePaused || m_gamePausedDueToDamage)
            m_gamePaused = m_gamePausedDueToDamage = false;
        if (!m_timeReversed) {
            m_mediaPlayer->pause();
            m_mediaPlayerReverse->play();
        }
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
    if (keyEvent->isAutoRepeat()) return;
    if (keyEvent->key() == Qt::Key_R) {
        if (m_timeReversed) {
            m_mediaPlayerReverse->pause();
            m_mediaPlayer->play();
        };
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
    m_bkg = new StaticBackground(QPoint(0, 0));
    m_bkg->setPixmap(bkgPix);
    m_bkg->setCollideable(false);
    m_bkg->setZValue(-1001);
    this->addItem(m_bkg);
}

void GameEngine::saveGame(std::vector<QString> values) {
    std::vector<QString> names = {"name", "clocks", "score", "coins", "checkpoint", "powerups", "gametime", "items"};
    DatabaseController::Instance()->addSave(names, values);
}

void GameEngine::startSinglePlayer() {
    this->removeItem(m_initialMenu);

    heartbeat = new QTimer(this);
    connect(heartbeat, SIGNAL(timeout()), this, SLOT(invalidateTimer()));
    heartbeat->start(1); // 20fps

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

    goomba = new Enemy1(20, 18, enemyPath);
    goomba->setPos(500, 1120);

    floater = new MovingPlatform(48, 64);
    floater->setPixmap(QPixmap(otherSpritePath + "/1.png"));
    floater->setPos(300, 900);
    floater->setVelocity(QPointF(200, 0));
    floater->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    floater->setSolid(true);

    QPixmap myPix1(otherSpritePath + "/1.png");
    QPixmap myPix2(otherSpritePath + "/2.png");
    QPixmap myPix3(otherSpritePath + "/3.png");
    QPixmap myPix4(otherSpritePath + "/4.png");
    QPixmap myPix5(otherSpritePath + "/5.png");

    QPixmap bkgImg(bkgPath + "/sky2.jpg");

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
    life1->setPixmap(QPixmap(itemPath + "/HeartContainer.png"));

    life2 = new Sprite();
    life2->setPos(QPointF(50, 25));
    life2->setPixmap(QPixmap(itemPath + "/HeartContainer.png"));

    life3 = new Sprite();
    life3->setPos(QPointF(75, 25));
    life3->setPixmap(QPixmap(itemPath + "/HeartContainer.png"));

    object1 = new ObjectItem(16, 16, itemPath);
    object1->setPos(700, 1020);
    object1->setSolid(false);

    gameTime = new QGraphicsSimpleTextItem("Wooo!!!");
    gameTime->setPos(QPointF(630, 15));

    Level *level = new Level(levelPath, "LevelTest.tmx", 0);
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

    this->initBGM(level->getBGMPath(), level->getReversedBGMPath());

    m_gamePaused = false;
}

void GameEngine::characterDamaged() {
    m_gamePausedDueToDamage = true;
    this->setForegroundBrush(QColor(200, 0, 0, 127));
}

void GameEngine::forwardPositionChanged(qint64 pos) {
    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState)
        m_mediaPlayerReverse->setPosition(m_mediaPlayerReverse->duration() - pos);
}
void GameEngine::reversePositionChanged(qint64 pos) {
    if (m_mediaPlayerReverse->state() == QMediaPlayer::PlayingState)
        m_mediaPlayer->setPosition(m_mediaPlayer->duration() - pos);
}

void GameEngine::initBGM(QString bgmFileName, QString revBgmFileName) {
    if (bgmFileName != "" && !m_selectedOptions.muteBGM) {
        QFile ft(QApplication::applicationDirPath() + bgmPath + bgmFileName);

        qDebug() << "BGM: " << QApplication::applicationDirPath() + bgmPath + bgmFileName;
        qDebug() << "REV: " << QApplication::applicationDirPath() + bgmPath + revBgmFileName;
        if (!ft.exists()) qDebug() << "WTF?!?!?!?!?!";

        m_mediaPlayer->setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath() + bgmPath + bgmFileName));
        m_mediaPlayerReverse->setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath() + bgmPath + revBgmFileName));

        m_mediaPlayer->setVolume(50);
        m_mediaPlayerReverse->setVolume(50);


//        qDebug() << "Supported: " << m_mediaPlayer->hasSupport()

        connect(m_mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(playBGM(QMediaPlayer::MediaStatus)));
        connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(forwardPositionChanged(qint64)));
        connect(m_mediaPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError(QMediaPlayer::Error)));
        connect(m_mediaPlayerReverse, SIGNAL(positionChanged(qint64)), this, SLOT(reversePositionChanged(qint64)));

        m_mediaPlayer->play();
    }

}

//*** Menu Related Functionality
void GameEngine::displayInitialMenu() {
    MenuButton * testButton;
    MenuButton * newgameButton;
    MenuButton * loadgameButton;
    MenuButton * optionsButton;
    MenuButton * quitButton;
    MenuButton * mainmenuButton;
    MenuButton * saveButton;
    MenuButton * cancelButton;
    OptionButton * musicButton;
    OptionButton * sfxButton;

    //*** Instantiate Initial Menu
    m_initialMenu = new QGraphicsItemGroup();

    QPixmap *button_static = new QPixmap(buttonPath + "/menuButton_newgame_static.png");
    QPixmap *button_clicked = new QPixmap(buttonPath + "/menuButton_newgame_clicked.png");
    QPixmap *button_hover = new QPixmap(buttonPath + "/menuButton_newgame_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);

    newgameButton = new MenuButton(button_static, button_clicked, button_hover);
    newgameButton->setPos(this->width()/2-newgameButton->boundingRect().width()/2, this->height()/5-newgameButton->boundingRect().height()/2);
    std::function<void(void)> func = std::bind(&GameEngine::startSinglePlayer, this);
    newgameButton->setCallback(func);

    button_static = new QPixmap(buttonPath + "/menuButton_loadgame_static.png");
    button_clicked = new QPixmap(buttonPath + "/menuButton_loadgame_clicked.png");
    button_hover = new QPixmap(buttonPath + "/menuButton_loadgame_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);
    loadgameButton = new MenuButton(button_static, button_clicked, button_hover);
    loadgameButton->setPos(this->width()/2-loadgameButton->boundingRect().width()/2, this->height()*2/5-loadgameButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::displayLoadMenu, this);
    loadgameButton->setCallback(func);

    button_static = new QPixmap(buttonPath + "/menuButton_options_static.png");
    button_clicked = new QPixmap(buttonPath + "/menuButton_options_clicked.png");
    button_hover = new QPixmap(buttonPath + "/menuButton_options_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);
    optionsButton = new MenuButton(button_static, button_clicked, button_hover);
    optionsButton->setPos(this->width()/2-optionsButton->boundingRect().width()/2, this->height()*3/5-optionsButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::displayOptionsMenu, this);
    optionsButton->setCallback(func);

    button_static = new QPixmap(buttonPath + "/menuButton_quit_static.png");
    button_clicked = new QPixmap(buttonPath + "/menuButton_quit_clicked.png");
    button_hover = new QPixmap(buttonPath + "/menuButton_quit_hover.png");
    *button_static = button_static->scaled(200, 100);
    *button_clicked = button_clicked->scaled(200, 100);
    *button_hover = button_hover->scaled(200, 100);
    quitButton = new MenuButton(button_static, button_clicked, button_hover);
    quitButton->setPos(this->width()/2-quitButton->boundingRect().width()/2, this->height()*4/5-quitButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::QuitGame, this);
    quitButton->setCallback(func);

    m_initialMenu->addToGroup(newgameButton);
    m_initialMenu->addToGroup(loadgameButton);
    m_initialMenu->addToGroup(optionsButton);
    m_initialMenu->addToGroup(quitButton);
    m_initialMenu->setHandlesChildEvents(false);
    m_initialMenu->setPos(0, 0);
    //*** End Instantiation of Initial Menu

    //*** Instantiate Options Menu
    m_optionsMenu = new QGraphicsItemGroup();

    button_static = new QPixmap(buttonPath + "/option_music_static.png");
    button_clicked = new QPixmap(buttonPath + "/option_music_clicked.png");
    musicButton = new OptionButton(button_static, button_clicked);
    musicButton->setPos(this->width()/2-musicButton->boundingRect().width()/2, this->height()/4-musicButton->boundingRect().height()/2);
    musicButton->setListener(&(m_selectedOptions.muteBGM));

    button_static = new QPixmap(buttonPath + "/option_music_static.png");
    button_clicked = new QPixmap(buttonPath + "/option_music_clicked.png");
    sfxButton = new OptionButton(button_static, button_clicked);
    sfxButton->setPos(this->width()/2-musicButton->boundingRect().width()/2, this->height()*2/4-musicButton->boundingRect().height()/2);
    sfxButton->setListener(&(m_selectedOptions.muteSFX));

    button_static = new QPixmap(buttonPath + "/menuButton_save_static.png");
    button_clicked = new QPixmap(buttonPath + "/menuButton_save_clicked.png");
    button_hover = new QPixmap(buttonPath + "/menuButton_save_hover.png");
    saveButton = new MenuButton(button_static, button_clicked, button_hover);
    saveButton->setPos(this->width()/3-saveButton->boundingRect().width()/2, this->height()*3/4-saveButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::saveSettings, this);
    saveButton->setCallback(func);

    button_static = new QPixmap(buttonPath + "/menuButton_cancel_static.png");
    button_clicked = new QPixmap(buttonPath + "/menuButton_cancel_clicked.png");
    button_hover = new QPixmap(buttonPath + "/menuButton_cancel_hover.png");
    cancelButton = new MenuButton(button_static, button_clicked, button_hover);
    cancelButton->setPos(this->width()*2/3-cancelButton->boundingRect().width()/2, this->height()*3/4-cancelButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::modifiedOptionsWarning, this);
    cancelButton->setCallback(func);

    m_optionsMenu->addToGroup(musicButton);
    m_optionsMenu->addToGroup(sfxButton);
    m_optionsMenu->addToGroup(saveButton);
    m_optionsMenu->addToGroup(cancelButton);
    m_optionsMenu->setHandlesChildEvents(false);
    m_optionsMenu->setPos(0, 0);
    //*** End Instantiation of Options Menu

    //*** Instantiate Load Menu
    m_loadMenu = new QGraphicsItemGroup();

    button_static = new QPixmap(buttonPath + "/menuButton_mainmenu_static.png");
    button_clicked = new QPixmap(buttonPath + "/menuButton_mainmenu_clicked.png");
    button_hover = new QPixmap(buttonPath + "/menuButton_mainmenu_hover.png");
    mainmenuButton = new MenuButton(button_static, button_clicked, button_hover);
    mainmenuButton->setPos(this->width()/2-mainmenuButton->boundingRect().width()/2, this->height()*2/3-mainmenuButton->boundingRect().height()/2);
    func = std::bind(&GameEngine::displayMainMenu_load, this);
    mainmenuButton->setCallback(func);

    m_loadMenu->addToGroup(mainmenuButton);
    m_loadMenu->setHandlesChildEvents(false);
    m_loadMenu->setPos(0, 0);
    //*** End Instantiation of Load Menu

    this->addItem(m_initialMenu);
}

//*** Functions related to displaying the various menus

void GameEngine::displayLoadMenu() {
    this->removeItem(m_initialMenu);
    this->addItem(m_loadMenu);

    DatabaseController* m_table = DatabaseController::Instance();

    std::vector<QString> names = {"name", "clocks", "score", "coins", "checkpoint", "powerups", "gametime", "items"};
    std::vector<QString> values = {"Bob", "3", "1001", "20", "4", "255", "1208", "255"};
    m_table->addSave(names, values);
    values.clear();

    values = {"Atley", "7", "2000", "120", "44", "513", "321", "513"};
    m_table->addSave(names, values);
    values.clear();

    values = {"Jesse", "7", "2000", "120", "44", "513", "321", "513"};
    m_table->addSave(names, values);

    m_table->readVals("*", "save_table");

    qDebug() << "------------------------------------------------------";

    m_table->readVals("*", "save_table");
}

void GameEngine::displayOptionsMenu() {
    this->removeItem(m_initialMenu);
    this->addItem(m_optionsMenu);
}

void GameEngine::displayMainMenu_load() {
    this->removeItem(m_loadMenu);
    this->addItem(m_initialMenu);
}

void GameEngine::displayMainMenu_option() {
    this->removeItem(m_optionsMenu);
    this->addItem(m_initialMenu);
}
//***************************************************************************

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
        this->saveSettings();
        break;
    case QMessageBox::Discard:
        this->displayMainMenu_option();
        break;
    }
}

void GameEngine::saveSettings() {
    for (auto itr = m_optionsMenu->childItems().begin(); itr != m_optionsMenu->childItems().end(); ++itr) {
        IMenuItem *itm = dynamic_cast<IMenuItem*>(*itr);
        if (itm) {
            itm->saveValue();
        }
    }
    this->displayMainMenu_option();
    SFXManager::Instance()->setMute(m_selectedOptions.muteSFX);
}

void GameEngine::QuitGame() {
    exit(0);
}

