#include <Window/Window.hpp>

Window::Window(const WindowSpecs& specs) : m_specs(specs)
{ }

void Window::create()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_handle = glfwCreateWindow(m_specs.width, m_specs.height, m_specs.title.c_str(), nullptr, nullptr);

    if (!m_handle)
    {
        std::cerr << "Failed to create GLFW window!\n";
        throw std::runtime_error("");
    }

    glfwMakeContextCurrent(m_handle);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(m_specs.vSync ? 1 : 0);
}

Window::~Window()
{
    destroy();
}

void Window::destroy()
{
    if (m_handle)
        glfwDestroyWindow(m_handle);

    m_handle = nullptr;
}

void Window::update()
{
    glfwSwapBuffers(m_handle);
}

glm::vec2 Window::getFramebufferSize()
{
    int width, height;
    glfwGetFramebufferSize(m_handle, &width, &height);
    return { width, height };
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_handle) != 0;
}