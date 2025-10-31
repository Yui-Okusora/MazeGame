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

    std::vector<int> maze2 = 
    {// 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 
     //   1   2   3   4   5   6   7   8
        0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,//1
        0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,//2 1
        0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,//3
        0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,//4 2
        0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,//5
        0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,//6 3
        0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,//7
        0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,//8 4
        0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,//9
        0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,//0 5
        0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,//1
        0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,//2 6
        0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,//3
        0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,//4 7
        0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,//5
        0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,//6 8
        0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0 //7
    };


    glm::uvec2 mazePos = {};

    while (app->getRunningStat())
    {
        double currentTime = app->getTime();
        double deltaTime = currentTime - lastTime;
        //std::cout << deltaTime << "\n";
        double timestep = std::clamp(deltaTime, 0.001, 0.1);
        lastTime = currentTime;

        GameplayData& base = renderBuffer.getWriteBuffer();

        glm::vec2 move = {};
        glm::vec2& playerPos = gameplayData.playerPos;

        while (!inputBuffer.empty())
        {
            InputEvent event = inputBuffer.pop();

            if (event.action == GLFW_PRESS)
                inputState.keyDown[event.key] = true;
            else if (event.action == GLFW_RELEASE)
                inputState.keyDown[event.key] = false;

            if (event.key == GLFW_KEY_ESCAPE && event.action == GLFW_PRESS)
                app->stop();
            if      ((event.key == GLFW_KEY_W || event.key == GLFW_KEY_UP)    && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.y = -1;
            else if ((event.key == GLFW_KEY_S || event.key == GLFW_KEY_DOWN)  && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.y = 1;
            else if ((event.key == GLFW_KEY_A || event.key == GLFW_KEY_LEFT)  && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.x = -1;
            else if ((event.key == GLFW_KEY_D || event.key == GLFW_KEY_RIGHT) && (event.action == GLFW_REPEAT || event.action == GLFW_PRESS))
                move.x = 1;
        }
        
        if (move.x == 1)
        {
            gameplayData.isLeft = false;
        }
        if (move.x == -1)
        {
            gameplayData.isLeft = true;
        }

        if (playerPos.x >= gameplayData.mazePos.x && playerPos.x <= (gameplayData.mazePos.x + gameplayData.mazeSize.x - 64))
        if (playerPos.y >= gameplayData.mazePos.y && playerPos.y <= (gameplayData.mazePos.y + gameplayData.mazeSize.y - 64))
        {
            mazePos = (playerPos - gameplayData.mazePos) / 32.0f + 1.0f;
            if (move.x == 1)   move.x *= maze2[mazePos.x + 17 * mazePos.y + 1];
            if (move.x == -1)  move.x *= maze2[mazePos.x + 17 * mazePos.y - 1];
            if (move.y == 1)   move.y *= maze2[mazePos.x + 17 * (mazePos.y + 1)];
            if (move.y == -1)  move.y *= maze2[mazePos.x + 17 * (mazePos.y - 1)];
            std::cout << mazePos.x << " " << mazePos.y << "\n";
        }

        if (move.x != 0 || move.y != 0)
        {
            move = glm::normalize(move);
            move *= 64;
            playerPos += move;
        }

        {
            static double accum = 0;
            accum += deltaTime;
            if (accum > 0.22)
            {
                accum = 0;
                gameplayData.atlasPos.x = (gameplayData.atlasPos.x + 1) % 6;
            }
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