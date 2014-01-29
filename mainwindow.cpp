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

    ui->graphicsView->setScene(gameEngine);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonPress()));

    QPixmap myPix1(":Simple_Sprite/1.png");
    QPixmap myPix2(":Simple_Sprite/2.png");
    QPixmap myPix3(":Simple_Sprite/3.png");
    QPixmap myPix4(":Simple_Sprite/4.png");
    QPixmap myPix5(":Simple_Sprite/5.png");

    std::vector<QPixmap> pixmapList;
    pixmapList.push_back(myPix1);
    pixmapList.push_back(myPix2);
    pixmapList.push_back(myPix3);
    pixmapList.push_back(myPix4);
    pixmapList.push_back(myPix5);

    std::vector<QPixmap> pixmapList2;
    pixmapList2.push_back(myPix3);
    pixmapList2.push_back(myPix5);
    pixmapList2.push_back(myPix2);
    pixmapList2.push_back(myPix4);
    pixmapList2.push_back(myPix1);

    testSprite = new AnimatedCollideableSprite(48, 64);
    testSprite->addAnimation(pixmapList, Forward_Reverse_Loop);
    testSprite->setPos(0, 300);
    testSprite->setVelocity(QPointF(0, 0));
    testSprite->setSolid(true);
    testSprite->triggerAnimation(0);

    testSprite2 = new AnimatedCollideableSprite(48, 64);
    testSprite2->addAnimation(pixmapList2, Forward_Reverse_Loop);
    testSprite2->setPos(gameEngine->width()/2 - 48, 300);
    testSprite2->setVelocity(QPointF(0, 0));
    testSprite2->setSolid(true);
    testSprite2->triggerAnimation(0);

    gameEngine->addSprite(testSprite, true);
    gameEngine->addSprite(testSprite2);

    heartbeat = new QTimer(this);
    connect(heartbeat, SIGNAL(timeout()), this, SLOT(invalidateTimer()));
    heartbeat->start(10); // 20fps
}

void MainWindow::buttonPress() {
    if(testSprite) {
        testSprite->jump();
    }
}

void MainWindow::invalidateTimer() {
    qint64 nMS = QDateTime::currentMSecsSinceEpoch();
    gameEngine->step(nMS);
    if (testSprite->pos().x() > gameEngine->width()) testSprite->setPos(-64, testSprite->pos().y());
    if (testSprite->pos().y() > 300) {
        testSprite->setVelocity(QPointF(testSprite->getVelocity().x(), 0));
        testSprite->setPos(testSprite->pos().x(), 300);
    }

    if (testSprite2->pos().x() < 0) testSprite2->setPos(gameEngine->width(), testSprite2->pos().y());
    if (testSprite2->pos().y() > 300) {
        testSprite2->setVelocity(QPointF(testSprite2->getVelocity().x(), 0));
        testSprite2->setPos(testSprite2->pos().x(), 300);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
