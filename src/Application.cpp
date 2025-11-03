#include <Application/Application.hpp>


Application::Application(const ApplicationSpecs& specs)
    : m_specs(specs), m_keyInputBuffer(m_specs.keyInputBufSize), m_mouseKeyBuffer(m_specs.mouseKeyBufSize)
{
    m_startTimePoint = clock::now();
    m_processor = std::make_unique<Processor>(this);
    m_io = std::make_unique<IO>(this);

    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW\n");

    m_running = true;

    if (m_specs.windowsSpecs.title.empty())
        m_specs.windowsSpecs.title = m_specs.appTitle;

    m_window = std::make_shared<Window>(m_specs.windowsSpecs);
    m_window->create(this);
    
    gl2d::init();
    renderer.create(0, m_specs.quadCount);
}

Application::~Application()
{
    if (m_procThread.joinable()) m_procThread.join();
    if (m_ioThread.joinable()) m_ioThread.join();
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
    m_ioThread = std::thread(*m_io);
    m_procThread = std::thread(*m_processor);

    glfwSetKeyCallback(m_window->getHandle(), keyInputCb);
    glfwSetMouseButtonCallback(m_window->getHandle(), mouseKeyInputCb);
    glfwSetCursorPosCallback(m_window->getHandle(), mousePosInputCb);

    gl2d::Texture background;
    gl2d::Texture player_texture;

    player_texture.loadFromFile("resources\\Main.png", true);

    background.loadFromFile("resources\\Mazemap1.png");

    gl2d::TextureAtlas atlas(6, 1);

    while (m_running)
    {
        if (m_window->shouldClose())
        {
            stop();
            break;
        }

        GameplayData& gameplayData = getRenderBuffer().getReadBuffer();

        glm::vec2 clientRect = getFramebufferSize();
        int width = (float)clientRect.x;
        int height = (float)clientRect.y;

        glViewport(0, 0, width, height);

        renderer.updateWindowMetrics(width, height);

        renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

        renderer.renderRectangle({600, 300, 100, 100}, Colors_White);

        renderer.renderRectangle({ gameplayData.mazePos, gameplayData.mazeSize }, background);
        
        renderer.renderRectangle({ gameplayData.playerPos, gameplayData.playerSize }, player_texture, Colors_White, {}, 0);//, atlas.get(gameplayData.atlasPos.x, gameplayData.atlasPos.y, gameplayData.isLeft));
        
        renderer.flush();

        m_window->update();
        getRenderBuffer().swap();
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
