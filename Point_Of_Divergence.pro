#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T13:54:22
#
#-------------------------------------------------

QT       += core gui xml sql multimedia
CONFIG   += c++11

# Silly warning
# QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder -Wsequence-point -Wno-unused-parameter

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Point_Of_Divergence
TEMPLATE = app


SOURCES += main.cpp\
    gameengine.cpp \
    sprite.cpp \
    animatedsprite.cpp \
    staticbackground.cpp \
    animatedcollideablesprite.cpp \
    tilemap.cpp \
    maincharacter.cpp \
    staticplatform.cpp \
    movingplatform.cpp \
    statearray.cpp \
    enemy1.cpp \
    objectitem.cpp \
    mainwindow.cpp \
    tile.cpp \
    hud.cpp \
    menubutton.cpp \
    level.cpp \
    optionbutton.cpp \
    databasecontroller.cpp

HEADERS  += mainwindow.h \
    gameengine.h \
    animatedsprite.h \
    sprite.h \
    staticbackground.h \
    animatedcollideablesprite.h \
    tilemap.h \
    maincharacter.h \
    staticplatform.h \
    movingplatform.h \
    statearray.h \
    enemy1.h \
    objectitem.h \
    tile.h \
    global.h \
    hud.h \
    menubutton.h \
    level.h \
    optionbutton.h \
    databasecontroller.h

FORMS    += mainwindow.ui

RESOURCES += \
    Simple_Sprite.qrc \
    Backgrounds.qrc \
    MarioSprites.qrc \
    Goomba.qrc \
    Life.qrc \
    Items.qrc \
    LevelTest.qrc \
    MenuImages.qrc

OTHER_FILES += \
    ../../Desktop/Game Assets/LevelTest.tmx
