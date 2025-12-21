#pragma once
#include <miniaudio.h>
#include <atomic>

class AudioEngine;

enum AudioCategory {
    Master,
    Music,
    GameplaySFX,
    InteractSFX,
    CATEGORIES_COUNT
};

class Sound {
public:
    Sound(AudioCategory category);
    ~Sound();

    bool load(AudioEngine& engine, const char* path, bool stream = false);
    void unload();

    void play();
    void playDelayed(float seconds);
    void stop();

    void update(float dt);

    void setVolume(float v); // individual volume
    bool isPlaying() const;
    bool isFinished() const;

private:
    ma_sound m_sound{};
    AudioEngine* m_engine = nullptr;

    AudioCategory m_category;
    float m_volume = 1.0f;
    float m_delay = 0.0f;

    bool  m_loaded = false;
    bool  m_pending = false;

    std::atomic<bool> m_finished{ true };

    static void endCallback(void* userData, ma_sound*);
    void applyVolume();
};