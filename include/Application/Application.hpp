#pragma once
#include <Window/Window.hpp>
#include <Application/IApplication.hpp>
#include <Processor/Processor.hpp>
#include <IO/IO.hpp>

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

    template<typename T>
        requires(std::is_base_of_v<State, T>)
    void pushState()
    {
        m_stateStack.push_back(std::make_unique<T>());
    }

    glm::vec2 getFramebufferSize() const;

    bool getRunningStat() { return m_running; }

    float getTime();

    DoubleBuffer<RenderData>& getRenderBuffer() { return m_renderBuffer; }

    std::vector<std::unique_ptr<State>>& getStateStack() { return m_stateStack; }

private:
    ApplicationSpecs m_specs;
    std::shared_ptr<Window> m_window;

    gl2d::Renderer2D renderer;
    gl2d::Texture player_texture;

    std::thread m_procThread;
    std::thread m_ioThread;

    std::unique_ptr<Processor> m_processor;
    std::unique_ptr<IO> m_io;

    DoubleBuffer<RenderData> m_renderBuffer;

    bool m_running = false;

    std::chrono::steady_clock::time_point m_startTimePoint;

    std::vector<std::unique_ptr<State>> m_stateStack;
};