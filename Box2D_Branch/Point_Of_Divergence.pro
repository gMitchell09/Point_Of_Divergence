#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T13:54:22
#
#-------------------------------------------------

QT       += core gui xml sql multimedia network
CONFIG   += c++11
CONFIG   -= app_bundle

DESTDIR = $$PWD

copy_resources.path = $$OUT_PWD
copy_resources.files = resources

# Silly warning
# QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder -Wsequence-point -Wno-unused-parameter

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Point_Of_Divergence
TEMPLATE = app

INCLUDEPATH += ./Box2D

box2d_lib = $$PWD/Box2D/Build/libbox2d.a

box_2d.target = box2d_lib
box_2d.commands = pushd $$PWD/Box2D && make -f ./Makefile box2d && cd $$OUT_PWD

QMAKE_EXTRA_TARGETS += box_2d

PRE_TARGETDEPS += box2d_lib

LIBS += $$box2d_lib

SOURCES += main.cpp \
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
    databasecontroller.cpp \
    sfxmanager.cpp \
    networkmanager.cpp \
    optionslider.cpp \
    networkplayer.cpp \
    characterclone.cpp \
    switchobject.cpp \
    door.cpp

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
    databasecontroller.h \
    sfxmanager.h \
    networkmanager.h \
    optionslider.h \
    IMenuItem.h \
    networkplayer.h \
    characterclone.h \
    switchobject.h \
    ITriggerable.h \
    door.h

FORMS    += mainwindow.ui

INSTALLS += copy_resources
