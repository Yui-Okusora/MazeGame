#pragma once

#include <Core.hpp>

struct WindowSpecs
{
    std::string title;
    int width = 800;
    int height = 600;
    bool resizable = true;
    bool vSync = true;
};

class Window
{
public:
    Window(const WindowSpecs& specs = WindowSpecs());
    ~Window();

    void create(void* applicationCtx);
    void destroy();
    void update();

    glm::vec2 getFramebufferSize();

    bool shouldClose() const;

    GLFWwindow* getHandle() const { return m_handle; }
private:
    WindowSpecs m_specs;
    GLFWwindow* m_handle = nullptr;
};