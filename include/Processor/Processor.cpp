#include <Processor/Processor.hpp>

Processor::Processor(IApplication* _app) : app(_app)
{

}

void Processor::operator()()
{
    double lastTime = app->getTime();

    auto& keyInputBuffer = app->getKeyInputBuffer();
    auto& mouseKeyBuffer = app->getMouseKeyBuffer();

    KeyInputState inputState;

    double fps = 60.0;
    double accumulator = 0.0;
    double timestep = 1.0 / 60.0;

    while (app->getRunningStat())
    {
        //std::cout << "                                       \n                                              \n                                                 \n                                                 \n                                                          \n                                                           \n                                                          \n                                                      \n                                                \n                                                                  \n                                                  \n                                                           \n                                                              \n                                                       \n\x1b[H";

        //deltaTime and fixed timestep between frames
        double currentTime = app->getTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        accumulator += deltaTime;

        inputState = {};

        // Key inputs handling
        while (!keyInputBuffer.empty())
        {
            auto event = keyInputBuffer.pop();

            if (event.action == GLFW_PRESS)
            {
                inputState.keyDown[event.key] = true;
                inputState.keyPressed[event.key] = true;
            }
            else if (event.action == GLFW_RELEASE)
            {
                inputState.keyDown[event.key] = false;
                inputState.keyReleased[event.key] = true;
            }
        }

        // Mouse button handling
        while (!mouseKeyBuffer.empty())
        {
            auto event = mouseKeyBuffer.pop();

            if (event.key.action == GLFW_PRESS)
            {
                inputState.mouseDown[event.key.button] = true;
                inputState.mousePressed[event.key.button] = true;
            }
            else if (event.key.action == GLFW_RELEASE)
            {
                inputState.mouseDown[event.key.button] = false;
                inputState.mouseReleased[event.key.button] = true;
            }
        }

        app->getStateStack().handleInput(inputState);

        while (accumulator >= timestep)
        {
            app->getStateStack().update(timestep);
            accumulator -= timestep;
        }

        app->getStateStack().processTransit();
    }
}