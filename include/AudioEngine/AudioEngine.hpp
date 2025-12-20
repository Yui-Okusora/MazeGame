#pragma once

#include <miniaudio.h>
#include <AudioEngine/Sound.hpp>
#include <array>

class AudioEngine
{
public:
    AudioEngine()
    {
        ma_engine_init(nullptr, &m_engine);
        m_volumeControl.fill(1.0f);
    }
    ~AudioEngine()
    {
        ma_engine_uninit(&m_engine);
    }

    void setCategoryVolume(AudioCategory cat, float v);

    float categoryVolume(AudioCategory cat) const;

    ma_engine* raw() { return &m_engine; }
private:
    ma_engine m_engine;
    std::array<float, AudioCategory::CATEGORIES_COUNT> m_volumeControl = {0.0f};
};