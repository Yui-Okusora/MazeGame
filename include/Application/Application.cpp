#include <Application/Application.hpp>
#include <Shape/Shape.hpp>


Application::Application(const ApplicationSpecs& specs)
    : m_specs(specs), m_keyInputBuffer(m_specs.keyInputBufSize), m_mouseKeyBuffer(m_specs.mouseKeyBufSize)
{
    m_startTimePoint = clock::now();
    m_processor = std::make_unique<Processor>(this);

    m_audioEngine.setCategoryVolume(AudioCategory::Master, 1.0f);
    m_audioEngine.setCategoryVolume(AudioCategory::Music, 0.5f);
    m_audioEngine.setCategoryVolume(AudioCategory::GameplaySFX, 1.0f);
    m_audioEngine.setCategoryVolume(AudioCategory::InteractSFX, 1.0f);

    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW\n");

    m_running = true;

    sharedCTX = m_specs.sharedCTX;

    if (m_specs.windowsSpecs.title.empty())
        m_specs.windowsSpecs.title = m_specs.appTitle;

    m_window = std::make_shared<Window>(m_specs.windowsSpecs);
    m_window->create(this);

    glfwSetKeyCallback(m_window->getHandle(), keyInputCb);
    glfwSetMouseButtonCallback(m_window->getHandle(), mouseKeyInputCb);
    glfwSetCursorPosCallback(m_window->getHandle(), mousePosInputCb);
    
    gl2d::init();
    renderer.create(0, m_specs.quadCount);
}

Application::~Application()
{
    if (m_procThread.joinable()) m_procThread.join();
    gl2d::cleanup();
    m_window->destroy();
    glfwTerminate();
}

void Application::keyInputCb(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    auto& buf = app->getKeyInputBuffer();
    buf.push({ key, scancode, action, mods });
}

void Application::mouseKeyInputCb(GLFWwindow* window, int button, int action, int mods)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    auto& buf = app->getMouseKeyBuffer();
    buf.push({ {button, action, mods} , app->getMousePos()});
}

void Application::mousePosInputCb(GLFWwindow* window, double xpos, double ypos)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    auto& mousePos = app->m_mousePos;

    mousePos.store(MousePos(xpos, ypos), std::memory_order_relaxed);
}

void Application::run()
{
    //m_ioThread = std::thread(*m_io);
    m_procThread = std::thread(*m_processor);

    while (m_running)
    {
        if (m_window->shouldClose())
        {
            stop();
            break;
        }
        glfwPollEvents();

        glm::vec2 clientRect = getFramebufferSize();
        int width = (float)clientRect.x;
        int height = (float)clientRect.y;

        glViewport(0, 0, width, height);

        renderer.updateWindowMetrics(width, height);

        m_viewportScale.store(computeViewportScale(clientRect, DESIGN_RESOLUTION), std::memory_order_release);

        m_stateStack.render();
        
        renderer.flush();

        m_window->update();

        glfwPollEvents();
    }
}

void Application::stop()
{
    m_running = false;  
}

glm::vec2 Application::getFramebufferSize() const
{
    return m_window->getFramebufferSize();
}

double Application::getTime()
{
    return std::chrono::duration<double>(clock::now() - m_startTimePoint).count();
}
