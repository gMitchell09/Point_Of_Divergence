#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T13:54:22
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

# Silly warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder -Wsequence-point

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Point_Of_Divergence
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameengine.cpp \
    sprite.cpp \
    animatedsprite.cpp \
    staticbackground.cpp \
    animatedcollideablesprite.cpp \
    tilemap.cpp \
    maincharacter.cpp \
    staticplatform.cpp \
    movingplatform.cpp \
    hudsprite.cpp

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
    hudsprite.h

FORMS    += mainwindow.ui

RESOURCES += \
    Simple_Sprite.qrc \
    Backgrounds.qrc \
    MarioSprites.qrc

OTHER_FILES +=
