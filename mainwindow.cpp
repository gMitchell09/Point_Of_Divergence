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
    gameEngine->setSceneRect(0, 0, 1275, 643);
    gameEngine->displayInitialMenu();//displayLoadMenu

    ui->graphicsView->setScene(gameEngine);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    connect(gameEngine, SIGNAL(myExit()), this, SLOT(newGraphicsScene()));
}

void MainWindow::newGraphicsScene() {

    delete gameEngine;
    gameEngine = nullptr;

    qDebug() << "trying to create new scene";

    gameEngine = new GameEngine(ui->graphicsView->width(), ui->graphicsView->height());
    gameEngine->setSceneRect(0, 0, 1275, 643);
    gameEngine->displayInitialMenu();//displayLoadMenu
    connect(gameEngine, SIGNAL(myExit()), this, SLOT(newGraphicsScene()));

    ui->graphicsView->setScene(gameEngine);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow() {
    delete ui;
}
