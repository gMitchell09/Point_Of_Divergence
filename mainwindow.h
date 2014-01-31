#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>

#include "gameengine.h"
#include "animatedsprite.h"
#include "animatedcollideablesprite.h"
#include "staticbackground.h"
#include "tilemap.h"
#include "maincharacter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    AnimatedCollideableSprite *testSprite, *testSprite2;
    StaticBackground *bkg;

    MainCharacter *mainChar;

    GameEngine *gameEngine;

    QTimer *heartbeat;

private slots:
    void buttonPress();
    void invalidateTimer();
};

#endif // MAINWINDOW_H
