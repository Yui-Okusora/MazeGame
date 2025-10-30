#include <Processor/Processor.hpp>

Processor::Processor(IApplication* _app) : app(_app)
{

}

void Processor::operator()()
{

    double lastTime = app->getTime();

    auto& inputBuffer = app->getInputBuffer();
    auto& renderBuffer = app->getRenderBuffer();

    GameplayData gameplayData;
    InputState inputState;

    while (app->getRunningStat())
    {
        double currentTime = app->getTime();
        double deltaTime = currentTime - lastTime;
        double timestep = std::clamp(deltaTime, 0.001, 0.1);
        lastTime = currentTime;

        GameplayData& base = renderBuffer.getWriteBuffer();

        glm::vec2 move = {};

        while (!inputBuffer.empty())
        {
            InputEvent event = inputBuffer.pop();

            if (event.action == GLFW_PRESS)
                inputState.keyDown[event.key] = true;
            else if (event.action == GLFW_RELEASE)
                inputState.keyDown[event.key] = false;

            if (event.key == GLFW_KEY_ESCAPE && event.action == GLFW_PRESS)
                app->stop();
            if ((event.key == GLFW_KEY_W || event.key == GLFW_KEY_UP) && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.y = -1;
            if ((event.key == GLFW_KEY_S || event.key == GLFW_KEY_DOWN) && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.y = 1;
            if ((event.key == GLFW_KEY_A || event.key == GLFW_KEY_LEFT) && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.x = -1;
            if ((event.key == GLFW_KEY_D || event.key == GLFW_KEY_RIGHT) && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.x = 1;
            //std::cout << "Key: " << event.key << " action: " << event.action << "\n";
        }

        if (move.x != 0 || move.y != 0)
        {
            move = glm::normalize(move);
            move *= 64;
            gameplayData.player_pos += move;
        }

        base = gameplayData;

        renderBuffer.swap();

        auto frameTime = app->getTime() - currentTime;
        auto sleep = std::chrono::milliseconds(8) - std::chrono::duration<double>(frameTime);
        if (sleep > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleep);
        }
    }
}