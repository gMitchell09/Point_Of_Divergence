#ifndef SFXMANAGER_H
#define SFXMANAGER_H

#include <QSoundEffect>
#include <QSound>
#include <QApplication>

#include <array>
#include <QDebug>

class SFXManager
{
public:
    enum class SFX {
        MainChar_Jump = 0,
        MainChar_Damaged,
        MainChar_PushBox,
        Enemy_Squish,
        Coin_Grab,
        SFX_Count
    };

    static SFXManager* Instance() {
        if (!m_singleton) {
            m_singleton = new SFXManager;
        }
        return m_singleton;
    }

    bool playSound(SFX sound) {
        if (!m_mute) {
            m_sfxArray[(size_t)sound]->play();
            qDebug() << "Sound: " << (size_t)sound;
            return true;
        }
        return false;
    }

    void setMute(bool mute) { m_mute = mute; }
    void setVolume(float volume) {
        for (int i = 0; i < (int) SFX::SFX_Count; i++) {
            m_sfxArray[i]->setVolume(volume);
        }
    }

    ~SFXManager() { delete m_singleton; }

private:
    std::array<QSoundEffect*, (size_t)SFX::SFX_Count> m_sfxArray;
    SFXManager();

    bool m_mute;

    static SFXManager *m_singleton;
};

#endif // SFXMANAGER_H
