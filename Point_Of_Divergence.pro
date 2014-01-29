#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T13:54:22
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Point_Of_Divergence
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameengine.cpp \
    animatedsprite.cpp \
    sprite.cpp \
    staticbackground.cpp \
    animatedcollideablesprite.cpp \
    collideablesprite.cpp

HEADERS  += mainwindow.h \
    gameengine.h \
    animatedsprite.h \
    sprite.h \
    staticbackground.h \
    animatedcollideablesprite.h \
    collideablesprite.h

FORMS    += mainwindow.ui

RESOURCES += \
    Simple_Sprite.qrc

OTHER_FILES +=
