#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QGraphicsItem>
#include <QDebug>
#include <QDateTime>

#include <vector>
#include <functional>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);


    //
    mainChar = new MainCharacter(16, 32);
    mainChar->setPos(120 , 1020);
    mainChar->setSolid(true);

    goomba = new Enemy1(20, 18);
    goomba->setPos(1200, 1020);

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

    gameEngine = new GameEngine(ui->graphicsView->width(), ui->graphicsView->height());
    gameEngine->setSceneRect(0, 0, level->getLevelWidth(), level->getLevelHeight());

    ui->graphicsView->setScene(gameEngine);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gameEngine->addItem(level);
    gameEngine->displayBackground(bkgImg);

    gameEngine->addSprite(mainChar, true);
    gameEngine->addSprite(testSprite2);
    gameEngine->addSprite(floater);
    gameEngine->addHUD(life1);
    gameEngine->addHUD(life2);
    gameEngine->addHUD(life3);
    gameEngine->addHUDText(gameTime);

    gameEngine->addSprite(object1);
    gameEngine->addSprite(goomba); // Add our goomba
}

MainWindow::~MainWindow() {
    delete ui;
}
