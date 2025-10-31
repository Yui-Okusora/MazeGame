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

    double fps = 60.0;

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
        std::cout << "\x1b[H";
        //deltaTime and fixed timestep between frames
        double currentTime = app->getTime();
        double deltaTime = currentTime - lastTime;
        double timestep = std::clamp(deltaTime, 0.001, 0.1);
        lastTime = currentTime;

        //FPS printing
        fps = 0.9 * fps + 0.1 * (1.0 / deltaTime);
        std::cout << "FPS: " << fps << "\n";

        //Get buffer to send to render thread
        GameplayData& base = renderBuffer.getWriteBuffer();

        //Reset variable between frames
        glm::vec2 move = {};

        //Alias
        glm::vec2& playerPos = gameplayData.playerPos;

        // Key inputs handling
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
        
        //Animation handling
        {
            if (move.x == 1) gameplayData.isLeft = false;
            if (move.x == -1)gameplayData.isLeft = true;
            static double accum = 0;
            accum += deltaTime;
            if (accum > 0.22)
            {
                accum = 0;
                gameplayData.atlasPos.x = (gameplayData.atlasPos.x + 1) % 6;
            }
        }

        //Maze mechanism handling
        if (glm::all(glm::greaterThanEqual(playerPos, gameplayData.mazePos)) &&
            glm::all(glm::lessThanEqual(playerPos, gameplayData.mazePos + gameplayData.mazeSize - gameplayData.playerSize)))
        {
            mazePos = (playerPos - gameplayData.mazePos) / 32.0f + 1.0f;
            if (move.x == 1)   move.x *= maze2[mazePos.x + 17 * mazePos.y + 1];
            if (move.x == -1)  move.x *= maze2[mazePos.x + 17 * mazePos.y - 1];
            if (move.y == 1)   move.y *= maze2[mazePos.x + 17 * (mazePos.y + 1)];
            if (move.y == -1)  move.y *= maze2[mazePos.x + 17 * (mazePos.y - 1)];
            //std::cout << mazePos.x << " " << mazePos.y << "\n";
        }

        //Movement handling
        if (move.x != 0 || move.y != 0)
        {
            //move = glm::normalize(move);
            move *= 64;
            playerPos += move;
        }
        
        // Fill new data to buffer and swap
        base = gameplayData;
        renderBuffer.swap();

        auto frameTime = app->getTime() - currentTime;
        auto sleep = std::chrono::milliseconds(8) - std::chrono::duration<double>(frameTime);
        if (sleep > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleep);
        }
    }
}