#pragma once
#include <Application/IApplication.hpp>
#include <Processor/Processor.hpp>
#include <IO/IO.hpp>


struct ApplicationSpecs
{
    std::string appTitle = "Game";
    WindowSpecs windowsSpecs;
    size_t quadCount = 2000;
    size_t keyInputBufSize = 512;
    size_t mouseKeyBufSize = 512;
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

    MousePos getMousePos() { return m_mousePos.load(std::memory_order_relaxed); }

    //Get keyboard input buffer for consuming
    CircularBuffer<KeyInputEvent>& getKeyInputBuffer() { return m_keyInputBuffer; }

    CircularBuffer<MouseEvent>& getMouseKeyBuffer() { return m_mouseKeyBuffer; }

    //Get render buffer for consuming
    DoubleBuffer<GameplayData>& getRenderBuffer() { return m_renderBuffer; }

private:
    static void keyInputCb(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseKeyInputCb(GLFWwindow* window, int button, int action, int mods);
    static void mousePosInputCb(GLFWwindow* window, double xpos, double ypos);

    //Application Specification descriptor
    ApplicationSpecs m_specs;

    //Window handle owner
    std::shared_ptr<Window> m_window;

    //gl2d render texture
    gl2d::Renderer2D renderer;

    //Worker threads
    std::thread m_procThread;
    std::thread m_ioThread;


    std::unique_ptr<Processor> m_processor;
    std::unique_ptr<IO> m_io;

    CircularBuffer<KeyInputEvent> m_keyInputBuffer;
    CircularBuffer<MouseEvent> m_mouseKeyBuffer;

    std::atomic<MousePos> m_mousePos;

    DoubleBuffer<GameplayData> m_renderBuffer;

    bool m_running = false;

    clock::time_point m_startTimePoint;
};