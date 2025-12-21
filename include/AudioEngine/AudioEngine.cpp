#include <AudioEngine/AudioEngine.hpp>

void AudioEngine::setCategoryVolume(AudioCategory cat, float v)
{
    m_volumeControl[cat] = v;
    //applyVolumes();
}

float AudioEngine::categoryVolume(AudioCategory cat) const
{
    return m_volumeControl[cat];
}

//void AudioEngine::applyVolumes()
//{
//    ma_engine_set_volume(&m_engine, m_masterVolume);
//}