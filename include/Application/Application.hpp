#pragma once
#include <Application/IApplication.hpp>
#include <Processor/Processor.hpp>
#include <State/State.hpp>


struct ApplicationSpecs
{
    std::string appTitle = "Game";
    WindowSpecs windowsSpecs;
    size_t quadCount = 2000;
    size_t keyInputBufSize = 512;
    size_t mouseKeyBufSize = 512;
    void*  sharedCTX = nullptr;
};

class Application : public IApplication
{
public:
    Application(const ApplicationSpecs& specs = ApplicationSpecs());
    ~Application();
    void run();

    //Command application to stop
    void stop();

    //Get window client size
    glm::vec2 getFramebufferSize() const;

    //Get flag for main loop running
    bool getRunningStat() { return m_running; }

    //Get time point
    double getTime();

    //Get shared context
    void* getCTX() { return sharedCTX; }

    //Get Mouse position
    MousePos getMousePos() { return m_mousePos.load(std::memory_order_relaxed); }
    
    //Get viewport scale
    ViewportScale getViewportScale() { return m_viewportScale.load(std::memory_order_relaxed); }

    //Get Audio Engine
    AudioEngine& getAudioEngine() { return m_audioEngine; }

    //Get keyboard input buffer for consuming
    CircularBuffer<KeyInputEvent>& getKeyInputBuffer() { return m_keyInputBuffer; }

    CircularBuffer<MouseEvent>& getMouseKeyBuffer() { return m_mouseKeyBuffer; }

    //Get/Apply window specs
    WindowSpecs& getWindowSpecs() { return m_specs.windowsSpecs; }

    void applyWindowSetting() { m_window->applySettings(); }

    //Get renderer
    gl2d::Renderer2D& getRenderer() { return renderer; }

    StateStack& getStateStack() { return m_stateStack; }

    //Add states
    template<typename T>
        requires(std::is_base_of_v<State, T>)
    void pushState()
    {
        m_stateStack.addTrack(std::make_unique<T>(this));
    }

    template<typename T>
        requires(std::is_base_of_v<State, T>)
    void pushInactive()
    {
        m_stateStack.addInactive(std::make_unique<T>(this));
    }

private:
    static void keyInputCb(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseKeyInputCb(GLFWwindow* window, int button, int action, int mods);
    static void mousePosInputCb(GLFWwindow* window, double xpos, double ypos);

    //Application Specification descriptor
    ApplicationSpecs m_specs;

    //Window handle owner
    std::shared_ptr<Window> m_window;

    //gl2d render texture
    const glm::vec2 DESIGN_RESOLUTION = { 1200.0f, 805.0f };
    gl2d::Renderer2D renderer;

    //Worker threads
    std::thread m_procThread;

    std::unique_ptr<Processor> m_processor;

    AudioEngine m_audioEngine;

    CircularBuffer<KeyInputEvent> m_keyInputBuffer;
    CircularBuffer<MouseEvent> m_mouseKeyBuffer;

    std::atomic<MousePos> m_mousePos;
    std::atomic<ViewportScale> m_viewportScale;

    DoubleBuffer<RenderData> m_renderBuffer;

    StateStack m_stateStack;

    CRC32_64 m_crcEngine;

    void* sharedCTX = nullptr;

    clock::time_point m_startTimePoint;

    bool m_running = false;
};