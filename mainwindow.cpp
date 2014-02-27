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


    gameEngine = new GameEngine(ui->graphicsView->width(), ui->graphicsView->height());
    gameEngine->setSceneRect(0, 0, 1275, 643);//ui->graphicsView->width(), ui->graphicsView->height());
    gameEngine->displayInitMenu();

    ui->graphicsView->setScene(gameEngine);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


}

MainWindow::~MainWindow() {
    delete ui;
}
