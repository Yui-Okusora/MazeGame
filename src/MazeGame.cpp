#include <Core.hpp>
#include <Application/Application.hpp>
#include <Utils/Utils.hpp>
#include <GameMechanics/PathFinding.hpp>

class GameplayState : public State
{
public:
    GameplayState(IApplication* _app)
    {
        app = _app;
        label = "GameplayState";
        maze = std::make_shared<Maze>();
    }

    void handleInput(const KeyInputState& in) override
    {
        if (in.keyPressed[GLFW_KEY_ESCAPE])
            app->stop();
        if (in.keyPressed[GLFW_KEY_W] || in.keyPressed[GLFW_KEY_UP])
        {
            move = {};
            move.y = -1;
        }
        else if (in.keyPressed[GLFW_KEY_S] || in.keyPressed[GLFW_KEY_DOWN])
        {
            move = {};
            move.y = 1;
        }
        else if (in.keyPressed[GLFW_KEY_A] || in.keyPressed[GLFW_KEY_LEFT])
        {
            move = {};
            move.x = -1;
        }
        else if (in.keyPressed[GLFW_KEY_D] || in.keyPressed[GLFW_KEY_RIGHT])
        {
            move = {};
            move.x = 1;
        }

        if (in.keyPressed[GLFW_KEY_P])
        {
            app->getStateStack().queueTransit(this, "PauseMenu");
        }

        MousePos mousePos = app->getMousePos();

        //std::cout << "MousePos x: " << mousePos.x << " y: " << mousePos.y << "                             \n";
        
        if (Utils::inRect({mousePos.x, mousePos.y}, {600, 300}, {100, 100}) && in.mousePressed[GLFW_MOUSE_BUTTON_LEFT])
        {
            std::cout << "Button pressed!\n";
            gameplayData = GameplayData();
            gameplayData.maze = maze;
            targetPos = gameplayData.playerPos;
            enemyTargetPos = gameplayData.enemyPos;
        }
    }

    void onEnter() override
    {
        playerTexture.loadFromFile("resources\\Main.png", true);
        mazeTexture.loadFromFile("resources\\Maze1.png", true);

        maze->mazeEncode = maze2;

        gameplayData.maze = maze;
        targetPos = gameplayData.playerPos;
        enemyTargetPos = gameplayData.enemyPos;
    }

    void update(double dt) override
    {
        //Get buffer to send to render thread
        GameplayData& base = m_renderBuffer.getWriteBuffer();

        glm::vec2& playerPos = gameplayData.playerPos;
        glm::vec2& enemyPos = gameplayData.enemyPos;

        {
            if (move.x == 1) gameplayData.isLeft = false;
            if (move.x == -1)gameplayData.isLeft = true;
            static double accum = 0;
            accum += dt;
            if (accum > 0.22)
            {
                accum -= 0.22;
                gameplayData.atlasPos.x = (gameplayData.atlasPos.x + 1) % 6;
            }
        }

        //Maze mechanism handling
        if (Utils::inRect(targetPos, gameplayData.playerSize, maze->pos, maze->size))
        {
            glm::ivec2 mazePos = (targetPos - maze->pos) / 32.0f + 1.0f;
            if (move.x == 1)   move.x *= maze2[mazePos.x + 17 * mazePos.y + 1];
            if (move.x == -1)  move.x *= maze2[mazePos.x + 17 * mazePos.y - 1];
            if (move.y == 1)   move.y *= maze2[mazePos.x + 17 * (mazePos.y + 1)];
            if (move.y == -1)  move.y *= maze2[mazePos.x + 17 * (mazePos.y - 1)];

            mazePos = (mazePos - 1) / 2;

            if (glm::length(targetPos - playerPos) < 0.1f)
            {
                if (mazePos.x == 4 && mazePos.y == 7) std::cout << "Win\n";
            }
        }

        //Movement handling
        if (move.x != 0 || move.y != 0)
        {
            move *= 64;
            targetPos += move;

            glm::ivec2 mazePos = (targetPos - maze->pos) / 64.0f;
            glm::ivec2 enemyMZPos = (enemyPos - maze->pos) / 64.0f;

            auto path = PathFinding::run({enemyMZPos.x, enemyMZPos.y}, {mazePos.x, mazePos.y}, {8, 8}, maze2);

            for (const auto& p : path) {
                std::cout << "(" << p.first << "," << p.second << ") -> ";
            }
            std::cout << "End\n";
            if(path.size() - 1)
            enemyTargetPos += (glm::ivec2(path[1].first, path[1].second) - enemyMZPos) * 64;
        }

        playerPos.x = std::lerp((double)playerPos.x, (double)targetPos.x, 0.5);
        playerPos.y = std::lerp((double)playerPos.y, (double)targetPos.y, 0.5);

        if (std::abs(playerPos.x - targetPos.x) < 3.0f) playerPos.x = targetPos.x;
        if (std::abs(playerPos.y - targetPos.y) < 3.0f) playerPos.y = targetPos.y;

        enemyPos.x = std::lerp((double)enemyPos.x, (double)enemyTargetPos.x, 0.5);
        enemyPos.y = std::lerp((double)enemyPos.y, (double)enemyTargetPos.y, 0.5);

        if (std::abs(enemyPos.x - enemyPos.x) < 3.0f) enemyPos.x = enemyTargetPos.x;
        if (std::abs(enemyPos.y - enemyPos.y) < 3.0f) enemyPos.y = enemyTargetPos.y;

        move = {};

        base = gameplayData;
        m_renderBuffer.swap();
    }

    void render() override
    {
        GameplayData gameplayData = m_renderBuffer.getReadBuffer();

        gl2d::Renderer2D& renderer = app->getRenderer();

        renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

        if (gameplayData.maze != nullptr)
        {
            gameplayData.maze->texture = mazeTexture;
            gameplayData.maze->render(&renderer);
        }

        renderer.renderRectangle({ gameplayData.playerPos, gameplayData.playerSize }, playerTexture, Colors_White, {}, 0);//, atlas.get(gameplayData.atlasPos.x, gameplayData.atlasPos.y, gameplayData.isLeft));

        renderer.renderRectangle({ gameplayData.enemyPos, gameplayData.enemySize }, Colors_Red, {}, 0);

        renderer.renderRectangle({ 600, 300, 100, 100 }, Colors_White);
    }

private:
    gl2d::Texture playerTexture;
    gl2d::Texture mazeTexture;

    glm::vec2 move = {};
    glm::vec2 targetPos = {};

    glm::vec2 enemyTargetPos = {};

    std::vector<int> maze2 =
    {// 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 
        //   1   2   3   4   5   6   7   8
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//1
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

    std::shared_ptr<Maze> maze;

    GameplayData gameplayData;

    DoubleBuffer<GameplayData> m_renderBuffer;
};

class PauseMenu : public State
{
public:
    PauseMenu(IApplication* _app)
    {
        app = _app;
        label = "PauseMenu";
    }

    void onEnter()
    {

    }

    void handleInput(const KeyInputState& in) override
    {
        if (in.keyPressed[GLFW_KEY_P])
        {
            app->getStateStack().queueTransit(this, "GameplayState");
        }
    }

    void update()
    {

    }

    void render() override
    {
        gl2d::Renderer2D& renderer = app->getRenderer();

        renderer.clearScreen({ 1, 1, 1, 1 });
    }
};

int main() {
    ApplicationSpecs specs = {
        .appTitle = "Test",
        .windowsSpecs = {
            .width = 800,
            .height = 600,
            .resizable = true,
            .vSync = true
        }
    };

    Application app(specs);

    //app.getStateStack().resizeStack(2);

    app.pushState<GameplayState>();

    app.getStateStack().addInactive(std::make_unique<PauseMenu>(&app));

    app.run();

    return 0;
}
