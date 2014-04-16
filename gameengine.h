#ifndef GAMEENGINE_H
#define GAMEENGINE_H

/* Author: George Mitchell
 * Use Case: *
 */

#include <QObject>
#include <functional>
#include <vector>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsItemGroup>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QTimer>
#include <QDateTime>
#include <QtSql>
#include <QMessageBox>
#include <QMediaPlayer>

#include "animatedsprite.h"
#include "animatedcollideablesprite.h"
#include "maincharacter.h"
#include "staticbackground.h"
#include "global.h"
#include "hud.h"
#include "level.h"
#include "menubutton.h"
#include "optionbutton.h"
#include "enemy1.h"
#include "enemyboss.h"
#include "databasecontroller.h"
#include "optionbutton.h"
#include "optionslider.h"
#include "networkmanager.h"
#include "networkplayer.h"

#include "Box2D.h"

class Level;
class MainCharacter;
class MenuButton;
class OptionButton;
class IMenuItem;

class GameEngine : public QGraphicsScene
{
    Q_OBJECT
private:
    /* Arbitrary Constants, chosen by fair dice roll */
    static const float m_gravity; // The enemy's gate is down.

    b2World *m_world;

    double m_timeDivider { 1 };
    bool m_gamePaused { true };
    bool m_gamePausedDueToDamage { false };
    bool m_levelPanning { false };

    const float panSpeed { .3f };

    QMediaPlayer *m_mediaPlayer { nullptr };
    QMediaPlayer *m_mediaPlayerReverse { nullptr };

    qint64 m_prevTime { 0 };
    qint64 m_gameTime { 0 };
    qint64 m_totalGameTime { 0 };

    MainCharacter * m_mainActor { nullptr };
    NetworkPlayer *m_networkPlayer { nullptr };
    StaticBackground *m_bkg { nullptr };

    QGraphicsItemGroup * m_initialMenu { nullptr };
    QGraphicsItemGroup * m_loadMenu { nullptr };
    QGraphicsItemGroup * m_mpMenu { nullptr };
    QGraphicsItemGroup * m_optionsMenu { nullptr };

    QGraphicsSimpleTextItem *m_waitingForNetworkPlayer { nullptr };

    NetworkManager *m_networkManager { nullptr };
    QHostAddress m_peerAddress;

    std::vector<std::function<void(long)>> m_stepHandlerVector;

    // This is a vector of sprites that can be interacted with
    std::vector<Sprite*> m_spriteArray;
    std::vector<Sprite*> m_deletedItems;
    std::vector<Level*> m_levels;

    HUD* m_hud { nullptr };
    QGraphicsSimpleTextItem* m_hudGameTime { nullptr };

    //int m_keyRecentPress;
    bool m_timeReversed;

    int m_coinCount;

    QTimer *heartbeat { nullptr };

    struct ge_opts {
        bool muteBGM;
        bool muteSFX;
        float volumeBGM;
        float volumeSFX;

        ge_opts() {
            muteBGM = muteSFX = false;
            volumeBGM = volumeSFX = 1.0f;
        }
    } m_selectedOptions;

    void removeDeletedItems();
    void initBGM(QString bgmFileName, QString revBgmFileName);
    void panLevel();

public:
    explicit GameEngine(QObject *parent = 0);
    GameEngine(int width, int height, QObject *parent = 0);

    /* Iterate over m_stepHandlerVector and call each object so that they may update
     * 0. Calculate dt and update m_prevTime
     * 1. Call update on all child sprites by iterating over m_stepHandlerVector
     * 2. Check m_mainActor position to see if we need to update the viewport
     */
    void step(qint64 time);
    void displayBackground(QColor mycolor);
    void displayBackground(QPixmap &bkgPix);
    void displayInitialMenu();
    void displayLoadMenu();
    void displayMultiplayerMenu();
    void displayOptionsMenu();
    void displayMainMenu_load();
    void displayMainMenu_mp();
    void displayMainMenu_option();
    void displayMainMenu_sp();
    void startSinglePlayer();
    void saveSettings();
    void QuitGame();
    void modifiedOptionsWarning();

    void createMPPressed();
    void joinMPPressed();

    inline void addHandler(std::function<void(long)> callback) {
        m_stepHandlerVector.push_back(callback);
    }

    inline void addHUD(Sprite* sprite) {
        if (!m_hud) {
            m_hud = new HUD(this->views().at(0), QPointF(0, 0));
            this->addItem(m_hud);
        }
        this->addItem(sprite);
        m_hud->addToGroup(sprite);
    }

    inline void addHUDText(QGraphicsSimpleTextItem* text, bool isGameTime = false) {
        if (!m_hud) {
            m_hud = new HUD(this->views().at(0), QPointF(0, 0));
            this->addItem(m_hud);
        }
        this->addItem(text);
        m_hud->addToGroup(text);
        if (isGameTime)
            m_hudGameTime = text;
    }

    inline size_t addSprite(Sprite* sprite, bool mainActor = false) {
        this->addItem(sprite);
        if (mainActor) {
            m_mainActor = (MainCharacter*)(sprite);
        }
        m_spriteArray.push_back(sprite);
        return m_spriteArray.size();
    }

    inline void addLevel(Level* level) {
        m_levels.push_back(level);
        this->addItem((QGraphicsItemGroup*)level);
    }

    inline void addNetworkSprite(NetworkPlayer *np) {
        this->addSprite(np);
    }

    float getGravity() { return m_gravity; }

    virtual bool event(QEvent *event);
    virtual void removeItem(QGraphicsItem *item);
    virtual void removeItem(Sprite *item);

    virtual void incrementCoins() { m_coinCount++; }

    virtual void characterDamaged();

    void saveGame(std::vector<QString> values);

    b2World* getWorld() { return m_world; }

    virtual ~GameEngine();

protected:
    virtual void keyPressEvent(QKeyEvent * keyEvent);
    virtual void keyReleaseEvent(QKeyEvent * keyEvent);

signals:
    /// Emitted when a player connects to us
    void myExit();

private slots:
    void invalidateTimer() {
        qint64 nMS = QDateTime::currentMSecsSinceEpoch();
        step(nMS);
    }
    void playBGM(QMediaPlayer::MediaStatus status) { m_mediaPlayer->play(); qDebug() << "****************"; qDebug() << status;}
    void forwardPositionChanged(qint64 pos);
    void reversePositionChanged(qint64 pos);
    void handleError(QMediaPlayer::Error er) {
        qDebug() << "ERROR!!! " << er;
    }

    void networkPlayerConnected();
    void networkPlayerDisconnected();
};

#endif // GAMEENGINE_H
