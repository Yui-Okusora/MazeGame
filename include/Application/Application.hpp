#pragma once
#include <Window/Window.hpp>
#include <Application/IApplication.hpp>
#include <Processor/Processor.hpp>
#include <IO/IO.hpp>
#include <GameplayData/GameplayData.hpp>

struct ApplicationSpecs
{
    std::string appTitle = "Game";
    WindowSpecs windowsSpecs;
};

class Application : public IApplication
{
public:
    Application(const ApplicationSpecs& specs = ApplicationSpecs());
    ~Application();
    void run();
    void stop();

    glm::vec2 getFramebufferSize() const;

    bool getRunningStat() override { return m_running; }

    float getTime();

    DoubleBuffer<GameplayData>& getRenderBuffer() override { return m_renderBuffer; }

private:
    ApplicationSpecs m_specs;
    std::shared_ptr<Window> m_window;

    //gl2d render texture
    gl2d::Renderer2D renderer;
    gl2d::Texture player_texture;

    std::thread m_procThread;
    std::thread m_ioThread;

    std::unique_ptr<Processor> m_processor;
    std::unique_ptr<IO> m_io;

    DoubleBuffer<GameplayData> m_renderBuffer;

    bool m_running = false;

    std::chrono::steady_clock::time_point m_startTimePoint;
};