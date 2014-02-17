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
#include "staticplatform.h"
#include "movingplatform.h"
#include "hudsprite.h"
#include "hudtext.h"
#include "enemy1.h"
#include "objectitem.h"

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
    StaticPlatform *testSprite, *testSprite2;
    StaticBackground *bkg;
    HUDsprite *life1;
    HUDsprite *life2;
    HUDsprite *life3;
    HUDText *gameTime;
    ObjectItem *object1;

    MainCharacter *mainChar;
    Enemy1 *goomba;

    GameEngine *gameEngine;

    QTimer *heartbeat;
    MovingPlatform *floater;

private slots:
    void invalidateTimer();
};

#endif // MAINWINDOW_H
