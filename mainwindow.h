#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "gameengine.h"
#include "animatedsprite.h"
#include "animatedcollideablesprite.h"
#include "staticbackground.h"
#include "tilemap.h"
#include "maincharacter.h"
#include "enemy1.h"
#include "global.h"
#include "level.h"
#include "menubutton.h"

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


    GameEngine *gameEngine;

private slots:
    void newGraphicsScene();


};

#endif // MAINWINDOW_H
