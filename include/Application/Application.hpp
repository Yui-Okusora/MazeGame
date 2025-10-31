#pragma once
#include <Application/IApplication.hpp>
#include <Processor/Processor.hpp>
#include <IO/IO.hpp>


struct ApplicationSpecs
{
    std::string appTitle = "Game";
    WindowSpecs windowsSpecs;
    size_t quadCount = 2000;
};

class Application : public IApplication
{
public:
    Application(const ApplicationSpecs& specs = ApplicationSpecs());
    ~Application();
    void run();
    void stop();

    glm::vec2 getFramebufferSize() const;

    bool getRunningStat() { return m_running; }

    double getTime();

    CircularBuffer<InputEvent>& getInputBuffer() { return m_inputBuffer; }

    DoubleBuffer<GameplayData>& getRenderBuffer() { return m_renderBuffer; }

private:
    ApplicationSpecs m_specs;
    std::shared_ptr<Window> m_window;

    //gl2d render texture
    gl2d::Renderer2D renderer;
    gl2d::Texture player_texture;

    //Worker threads
    std::thread m_procThread;
    std::thread m_ioThread;

    std::unique_ptr<Processor> m_processor;
    std::unique_ptr<IO> m_io;

    CircularBuffer<InputEvent> m_inputBuffer;
    DoubleBuffer<GameplayData> m_renderBuffer;

    bool m_running = false;

    clock::time_point m_startTimePoint;
};