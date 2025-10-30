#include <Application/Application.hpp>


Application::Application(const ApplicationSpecs& specs)
    : m_specs(specs), m_inputBuffer(512)
{
    m_startTimePoint = clock::now();
    m_processor = std::make_unique<Processor>(this);
    m_io = std::make_unique<IO>(this);

    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW\n");
    }
    m_running = true;

    if (m_specs.windowsSpecs.title.empty())
        m_specs.windowsSpecs.title = m_specs.appTitle;

    m_window = std::make_shared<Window>(m_specs.windowsSpecs);
    m_window->create(this);
    
    gl2d::init();
    renderer.create();

    player_texture.loadFromFile(RESOURCES_PATH"2D Pixel Dungeon Asset Pack\\Character_animation\\monsters_idle\\skeleton1\\v1\\skeleton_v1_1.png", true);
}

Application::~Application()
{
    if (m_procThread.joinable()) m_procThread.join();
    if (m_ioThread.joinable()) m_ioThread.join();
    m_window->destroy();
    glfwTerminate();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    auto& buf = app->getInputBuffer();
    buf.push({ key, scancode, action, mods });
}

void Application::run()
{
    m_ioThread = std::thread(*m_io);
    m_procThread = std::thread(*m_processor);

    std::this_thread::sleep_for(std::chrono::duration<float>(0.01));

    glfwSetKeyCallback(m_window->getHandle(), key_callback);

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
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.updateWindowMetrics(width, height);

        // Clear screen
        renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

        // Render objects
        //std::cout << gameplayData.player_pos.x<<"\n";
        renderer.renderRectangle({gameplayData.player_pos, 64, 64 }, player_texture);
        // Add more rendering here...

        // Flush renderer (dump your rendering into the screen)
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
