#include <Core.hpp>
#include <Application/Application.hpp>
#include <Utils/Utils.hpp>

class GameplayState : public State
{
public:
    GameplayState(IApplication* _app)
    {
        app = _app;
    }
    gl2d::Texture playerTexture;
    gl2d::Texture mazeTexture;

    glm::vec2 move = {};

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

    std::shared_ptr<Maze> maze = std::make_shared<Maze>();

    GameplayData gameplayData;

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

        MousePos mousePos = app->getMousePos();

        //std::cout << "MousePos x: " << mousePos.x << " y: " << mousePos.y << "                             \n";
        
        if (Utils::inRect({mousePos.x, mousePos.y}, {600, 300}, {100, 100}) && in.mousePressed[GLFW_MOUSE_BUTTON_LEFT])
        {
            std::cout << "Button pressed!\n";
            gameplayData = GameplayData();
            gameplayData.maze = maze;
        }
    }

    void onEnter() override
    {
        playerTexture.loadFromFile("resources\\Main.png", true);
        mazeTexture.loadFromFile("resources\\Maze1.png", true);

        maze->mazeEncode = maze2;

        gameplayData.maze = maze;
    }

    void update(double dt) override
    {
        //Get buffer to send to render thread
        GameplayData& base = app->getRenderBuffer().getWriteBuffer();

        glm::vec2& playerPos = gameplayData.playerPos;

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
        if (Utils::inRect(playerPos, gameplayData.playerSize, maze->pos, maze->size))
        {
            glm::ivec2 mazePos = (playerPos - maze->pos) / 32.0f + 1.0f;
            if (move.x == 1)   move.x *= maze2[mazePos.x + 17 * mazePos.y + 1];
            if (move.x == -1)  move.x *= maze2[mazePos.x + 17 * mazePos.y - 1];
            if (move.y == 1)   move.y *= maze2[mazePos.x + 17 * (mazePos.y + 1)];
            if (move.y == -1)  move.y *= maze2[mazePos.x + 17 * (mazePos.y - 1)];
        }

        //Movement handling
        if (move.x != 0 || move.y != 0)
        {
            move *= 64;
            playerPos += move;
        }

        move = {};

        base = gameplayData;
        app->getRenderBuffer().swap();
    }

    void render() override
    {
        GameplayData gameplayData = app->getRenderBuffer().getReadBuffer();

        gl2d::Renderer2D& renderer = app->getRenderer();

        renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

        if (gameplayData.maze != nullptr)
        {
            gameplayData.maze->texture = mazeTexture;
            gameplayData.maze->render(&renderer);
        }

        renderer.renderRectangle({ gameplayData.playerPos, gameplayData.playerSize }, playerTexture, Colors_White, {}, 0);//, atlas.get(gameplayData.atlasPos.x, gameplayData.atlasPos.y, gameplayData.isLeft));

        renderer.renderRectangle({ 600, 300, 100, 100 }, Colors_White);
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

    app.pushState<GameplayState>();

    app.run();

    return 0;
}
