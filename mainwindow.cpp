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

    gameEngine = new GameEngine(ui->graphicsView->width(), ui->graphicsView->height());
    gameEngine->setSceneRect(0, 0, 3840, 1200);

    ui->graphicsView->setScene(gameEngine);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonPress()));

    //
    mainChar = new MainCharacter(16, 32);
    mainChar->setPos(0, 1020);

    floater = new StaticPlatform(0);
    floater->setPixmap(QPixmap(":Simple_Sprite/1.png"));
    floater->setPos(300, 900);

    QPixmap myPix1(":Simple_Sprite/1.png");
    QPixmap myPix2(":Simple_Sprite/2.png");
    QPixmap myPix3(":Simple_Sprite/3.png");
    QPixmap myPix4(":Simple_Sprite/4.png");
    QPixmap myPix5(":Simple_Sprite/5.png");

    QPixmap bkgPix(":Backgrounds/background.jpg");

    std::vector<QPixmap> pixmapList2;
    pixmapList2.push_back(myPix3);
    pixmapList2.push_back(myPix5);
    pixmapList2.push_back(myPix2);
    pixmapList2.push_back(myPix4);
    pixmapList2.push_back(myPix1);

    testSprite2 = new AnimatedCollideableSprite(48, 64);
    testSprite2->addAnimation(pixmapList2, Forward_Reverse_Loop);
    testSprite2->setPos(gameEngine->sceneRect().width()/2 - 48, 1020);
    testSprite2->setVelocity(QPointF(0, 0));
    testSprite2->setAcceleration(QPointF(0, 0));
    testSprite2->setSolid(true);
    testSprite2->triggerAnimation(0);
    testSprite2->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);

    bkg = new StaticBackground(QPoint(0, 0));
    bkg->setPixmap(bkgPix);
    bkg->setPos(0, 0);

    gameEngine->addItem(bkg);
    gameEngine->addSprite(mainChar, true);
    gameEngine->addSprite(testSprite2);
    gameEngine->addItem(floater);

    heartbeat = new QTimer(this);
    connect(heartbeat, SIGNAL(timeout()), this, SLOT(invalidateTimer()));
    heartbeat->start(1); // 20fps
}

void MainWindow::buttonPress() {

}

void MainWindow::invalidateTimer() {
    qint64 nMS = QDateTime::currentMSecsSinceEpoch();
    gameEngine->step(nMS);
    if (mainChar->pos().x() > gameEngine->width()) mainChar->setPos(-64, mainChar->pos().y());
    else if (mainChar->pos().x() < -64) mainChar->setPos(gameEngine->sceneRect().width(), mainChar->pos().y());

    if (mainChar->pos().y() > 1020) {
        mainChar->setVelocity(QPointF(mainChar->getVelocity().x(), 0));
        mainChar->setPos(mainChar->pos().x(), 1020);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
