#include <Processor/Processor.hpp>

Processor::Processor(IApplication* _app) : app(_app)
{

}

void Processor::operator()()
{
    float lastTime = app->getTime();

    auto& inputBuffer = app->getInputBuffer();
    auto& renderBuffer = app->getRenderBuffer();

    GameplayData gameplayData;

    while (app->getRunningStat())
    {
        float currentTime = app->getTime();
        std::cout << 1.0 / ((double)currentTime - (double)lastTime) << "\n";
        float timestep = std::clamp(currentTime - lastTime, 0.001f, 0.1f);
        lastTime = currentTime;

        GameplayData& base = renderBuffer.getWriteBuffer();

        glm::vec2 move = {};

        while (!inputBuffer.empty())
        {
            InputEvent event = inputBuffer.pop();
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
            move *= timestep * 20000;
            gameplayData.player_pos += move;
        }
        base = gameplayData;


        

        renderBuffer.swap();
    }
}