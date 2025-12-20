#include <AudioEngine/AudioEngine.hpp>

Sound::Sound(AudioCategory category)
    : m_category(category)
{
}

Sound::~Sound()
{
    if (m_loaded) {
        ma_sound_uninit(&m_sound);
    }
}

bool Sound::load(AudioEngine& engine, const char* path, bool stream)
{
    if (m_loaded)
        unload();

    ma_uint32 flags = stream ? MA_SOUND_FLAG_STREAM : 0;

    if (ma_sound_init_from_file(
        engine.raw(),
        path,
        flags,
        nullptr,
        nullptr,
        &m_sound) != MA_SUCCESS)
    {
        return false;
    }

    ma_sound_set_end_callback(&m_sound, &Sound::endCallback, this);

    m_engine = &engine;
    m_loaded = true;
    m_finished.store(true, std::memory_order_release);

    applyVolume();
    return true;
}

void Sound::unload()
{
    if (m_loaded)
    {
        ma_sound_stop(&m_sound);
        ma_sound_uninit(&m_sound);
        memset(&m_sound, 0, sizeof(m_sound));
        m_loaded = false;
    }
}

void Sound::play()
{
    if (!m_loaded) return;

    m_pending = false;
    m_finished.store(false, std::memory_order_release);
    applyVolume();
    ma_sound_start(&m_sound);
}

void Sound::playDelayed(float seconds)
{
    if (!m_loaded) return;

    m_delay = seconds;
    m_pending = true;
}

void Sound::stop()
{
    if (!m_loaded) return;

    m_pending = false;
    m_finished.store(true, std::memory_order_release);
    ma_sound_stop(&m_sound);
}

void Sound::update(float dt)
{
    if (!m_pending) return;

    m_delay -= dt;
    if (m_delay <= 0.0f) {
        m_pending = false;
        m_finished.store(false, std::memory_order_release);
        applyVolume();
        ma_sound_start(&m_sound);
    }
}

void Sound::setVolume(float v)
{
    m_volume = v;
    applyVolume();
}

bool Sound::isPlaying() const
{
    return m_loaded && ma_sound_is_playing(&m_sound);
}

bool Sound::isFinished() const
{
    return m_finished.load(std::memory_order_acquire);
}

void Sound::applyVolume()
{
    if (!m_loaded || !m_engine)
        return;

    float catVol = m_engine->categoryVolume(m_category);
    float finalVolume = m_volume * catVol;

    ma_sound_set_volume(&m_sound, finalVolume);
}

void Sound::endCallback(void* userData, ma_sound*)
{
    Sound* self = static_cast<Sound*>(userData);
    self->m_finished.store(true, std::memory_order_release);
}