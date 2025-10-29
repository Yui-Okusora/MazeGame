#include <Application/Application.hpp>


Application::Application(const ApplicationSpecs& specs)
    : m_specs(specs)
{
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
    m_window->create();
    
    gl2d::init();
    renderer.create();

    player_texture.loadFromFile("C:\\Users\\Lenovo\\Desktop\\Maze\\resources\\2D Pixel Dungeon Asset Pack\\Character_animation\\monsters_idle\\skeleton1\\v1\\skeleton_v1_1.png", true);
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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Vertex shader source
const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;

uniform vec2 uOffset; // dynamic position offset

void main() {
    gl_Position = vec4(aPos + uOffset, 0.0, 1.0);
}
)";

// Fragment shader source
const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;

void main() {
    FragColor = vec4(uColor, 1.0);
}
)";

// Check for shader compile errors
static void checkCompile(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::cerr << "Shader compile error: " << info << std::endl;
    }
}

void Application::run()
{
    m_ioThread = std::thread(*m_io);
    m_procThread = std::thread(*m_processor);

    glfwSetKeyCallback(m_window->getHandle(), key_callback);


    while (m_running)
    {
        if (m_window->shouldClose())
        {
            stop();
            break;
        }
        
        for (const std::unique_ptr<State>& state : m_stateStack)
            state->OnRender();


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
        renderer.renderRectangle({100, 250, 100, 100 }, player_texture);
        //renderer.renderRectangle({100, 100, 100, 100}, texture, Colors_White, {}, 0);
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

float Application::getTime()
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_startTimePoint).count();
}
