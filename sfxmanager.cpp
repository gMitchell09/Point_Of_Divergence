#include "sfxmanager.h"

static const QString sfxPath =  "/resources/sfx";

SFXManager* SFXManager::m_singleton = 0;

SFXManager::SFXManager() {
    qDebug() << "SFX Path: " << QApplication::applicationDirPath() + sfxPath + "/mcJump.wav";
    QSoundEffect *mcJump = new QSoundEffect();
    mcJump->setSource(QUrl::fromLocalFile(QApplication::applicationDirPath() + sfxPath + "/mcJump.wav"));
    mcJump->setVolume(1.0f);

    QSoundEffect *mcHurt = new QSoundEffect();
    mcHurt->setSource(QUrl::fromLocalFile(QApplication::applicationDirPath() + sfxPath + "/mcHurt.wav"));
    mcHurt->setVolume(1.0f);

    QSoundEffect *mcPushBox = new QSoundEffect();
    mcPushBox->setSource(QUrl::fromLocalFile(QApplication::applicationDirPath() + sfxPath + "/mcPushBox.wav"));
    mcPushBox->setVolume(1.0f);

    QSoundEffect *enemySquish = new QSoundEffect();
    enemySquish->setSource(QUrl::fromLocalFile(QApplication::applicationDirPath() + sfxPath + "/enemySquish.wav"));
    enemySquish->setVolume(1.0f);

    QSoundEffect *coinGrab = new QSoundEffect();
    coinGrab->setSource(QUrl::fromLocalFile(QApplication::applicationDirPath() + sfxPath + "/coinGrab.wav"));
    coinGrab->setVolume(1.0f);

    m_sfxArray[(size_t)SFX::MainChar_Jump] = mcJump;
    m_sfxArray[(size_t)SFX::MainChar_Damaged] = mcHurt;
    m_sfxArray[(size_t)SFX::MainChar_PushBox] = mcPushBox;

    m_sfxArray[(size_t)SFX::Enemy_Squish] = enemySquish;

    m_sfxArray[(size_t)SFX::Coin_Grab] = coinGrab;
}
