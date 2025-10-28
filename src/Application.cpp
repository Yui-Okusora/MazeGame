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
void checkCompile(GLuint shader) {
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

    float lastTime = getTime();

    glfwSetKeyCallback(m_window->getHandle(), key_callback);

    // Rectangle vertices (two triangles)
    float vertices[] = {
        -0.1f, -0.1f,
         0.1f, -0.1f,
         0.1f,  0.1f,
        -0.1f,  0.1f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Setup VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);
    checkCompile(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);
    checkCompile(fragmentShader);

    // Link program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    // Get uniform locations
    GLint offsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (m_running)
    {
        glfwPollEvents();

        if (m_window->shouldClose())
        {
            stop();
            break;
        }

        float currentTime = getTime();
        float timestep = std::clamp(currentTime - lastTime, 0.001f, 0.1f);
        lastTime = currentTime;

        
        for (const std::unique_ptr<State>& state : m_stateStack)
            state->OnUpdate(timestep);

        
        for (const std::unique_ptr<State>& state : m_stateStack)
            state->OnRender();


        glm::vec2 clientRect = m_window->getFramebufferSize();
        int width = (float)clientRect.x;
        int height = (float)clientRect.y;

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        float time = glfwGetTime();
        float x = 0.5f * sin(time);
        float y = 0.3f * cos(time * 1.5f);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform2f(offsetLoc, x, y);
        glUniform3f(colorLoc, fabs(sin(time)), fabs(cos(time)), 1.0f - fabs(sin(time)));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        m_window->update();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
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
