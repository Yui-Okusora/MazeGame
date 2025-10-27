#include <Application/Application.hpp>


Application::Application(const ApplicationSpecs& specs)
	: m_specs(specs)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW\n");
    }
    m_running = true;
    m_window = std::make_shared<Window>(specs.windowsSpecs);
    m_window->create();
}

Application::~Application()
{
    glfwTerminate();
}

void Application::run()
{
    while (!m_window->shouldClose() && m_running)
    {
        glm::vec2 clientRect = m_window->getFramebufferSize();
        int width = clientRect.x;
        int height = clientRect.y;

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        m_window->update();
    }
}

void Application::stop()
{
    m_running = false;  
}