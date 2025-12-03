#include <GameMechanics/GameplayState.hpp>

GameplayState::GameplayState(IApplication* _app)
{
    app = _app;
    label = "GameplayState";
}

void GameplayState::handleInput(const KeyInputState& in)
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

    if (in.keyPressed[GLFW_KEY_U])
    {
        sl.save(data);
        std::cout << "Saved!\n";
    }

    if (in.keyPressed[GLFW_KEY_L])
    {
        data = sl.load();
        player->pos = data.playerPos;
        enemy->pos = data.enemyPos;
        std::cout << "Loaded!\n";
    }

    MousePos mousePos = app->getMousePos();

    UI::processUI(in, mousePos);

    if (resetBtn->clicked())
    {
        std::cout << "Button pressed!\n";

        data.playerPos = maze->pos;
        data.enemyPos = maze->pos + glm::vec2(0, 448);

        player->pos = data.playerPos;
        enemy->pos = data.enemyPos;
    }
}

void GameplayState::onEnter()
{
    playerTexture.loadFromFile("resources\\Main.png", true);
    mazeTexture.loadFromFile("resources\\Maze1.png", true);

    maze = renderData.addShape<Maze>(Maze({ 0, 0 }, { 8, 8 }, { 64, 64 }));
    player = renderData.addShape<Rect>(Rect({ maze->pos, 64, 64 }, playerTexture));
    enemy = renderData.addShape<Rect>(Rect({ maze->pos + glm::vec2(0, 448), 64, 64}, Colors_Red));

    resetBtn = renderData.addShape<Button>(Button(GLFW_MOUSE_BUTTON_LEFT, {600, 300, 100, 100}, Colors_Green));

    maze->mazeEncode = maze2;

    maze->texture = mazeTexture;

    data.playerPos = player->pos;
    data.enemyPos = enemy->pos;
}

void GameplayState::update(double dt)
{
    glm::vec2& playerPos = player->pos;
    glm::vec2& playerSize = player->size;
    glm::vec2& enemyPos = enemy->pos;
    glm::vec2& enemySize = enemy->size;

    //Maze mechanism handling
    if (Utils::inRect(data.playerPos, playerSize, maze->pos, maze->size))
    {
        glm::ivec2 mazePos = (data.playerPos - maze->pos) / maze->tileSize.x * 2.0f + 1.0f;
        if (move.x == 1)   move.x *= maze2[mazePos.x + maze->encodeSize.x * mazePos.y + 1];
        if (move.x == -1)  move.x *= maze2[mazePos.x + maze->encodeSize.x * mazePos.y - 1];
        if (move.y == 1)   move.y *= maze2[mazePos.x + maze->encodeSize.x * (mazePos.y + 1)];
        if (move.y == -1)  move.y *= maze2[mazePos.x + maze->encodeSize.x * (mazePos.y - 1)];

        mazePos = (mazePos - 1) / 2;

        if (glm::length(data.playerPos - playerPos) < 0.1f)
        {
            if (mazePos.x == 4 && mazePos.y == 7) std::cout << "Win\n";
        }
    }

    //Movement handling
    if (move.x != 0 || move.y != 0)
    {
        move *= 64;
        data.playerPos += move;
        move = {};

        glm::ivec2 mazePos = (data.playerPos - maze->pos) / maze->tileSize.x;
        glm::ivec2 enemyMZPos = (data.enemyPos - maze->pos) / maze->tileSize.x;

        auto path = PathFinding::run({ enemyMZPos.x, enemyMZPos.y }, { mazePos.x, mazePos.y }, { 8, 8 }, maze2);

        if (path.size() > 1)
            data.enemyPos += (glm::ivec2(path[1].first, path[1].second) - enemyMZPos) * 64;
    }

    playerPos = Utils::lerp2(playerPos, data.playerPos, 0.5f, 3.0f);

    enemyPos = Utils::lerp2(enemyPos, data.enemyPos, 0.5f, 3.0f);

    //Get buffer to send to render thread
    RenderData& base = m_renderBuffer.getWriteBuffer();
    base = renderData;

    m_renderBuffer.swap();
}

void GameplayState::render()
{
    RenderData renderData = m_renderBuffer.getReadBuffer();

    gl2d::Renderer2D& renderer = app->getRenderer();

    renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

    for (auto& a : renderData.shapes)
    {
        a->render(&renderer);
    }
}