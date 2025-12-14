#include <GameMechanics/GameplayState.hpp>

GameplayState::GameplayState(IApplication* _app)
    : data(*static_cast<GameplayData*>(_app->getCTX()))
{
    app = _app;
    label = "GameplayState";
    playerTexture.loadFromFile("resources\\Main2.png", true);
    mazeTexture.loadFromFile("resources\\Maze1.png", true);
    font.createFromFile("resources\\BoldPixels.ttf");
}

void GameplayState::handleInput(const KeyInputState& in)
{
    MousePos mousePos = app->getMousePos();

    UI::processUI(in, mousePos);

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

    if (in.keyPressed[GLFW_KEY_P] || menuBtn->clicked())
    {
        app->getStateStack().queueTransit(this, "MainMenu");
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
        scoreDisplay->getText() = std::to_string(data.score);
        std::cout << "Loaded!\n";
    }

    if ((in.keyDown[GLFW_KEY_LEFT_CONTROL] && in.keyPressed[GLFW_KEY_Z]) || undoBtn->clicked())
    {
        if (!data.stepHistory.empty() && !data.enemyStepHistory.empty())
        {
            data.playerPos = data.stepHistory.back();
            data.enemyPos = data.enemyStepHistory.back();
            data.stepHistory.pop_back();
            data.enemyStepHistory.pop_back();
        }
    }

    if (resetBtn->clicked())
    {
        std::cout << "Button pressed!\n";

        data.playerPos = maze->pos;
        data.enemyPos = maze->pos + glm::vec2(6, 6) * glm::vec2(64, 64);

        player->pos = data.playerPos;
        enemy->pos = data.enemyPos;

        data.score = 0;
        data.time = 0;
        data.stepHistory.clear();
        data.enemyStepHistory.clear();
    }
}

void GameplayState::onEnter()
{
    renderData = RenderData();
      maze = renderData.addShape<Maze>(Maze({ 0, 0 }, { 8, 8 }, { 64, 64 }));
    player = renderData.addShape<Rect>(Rect(maze, { 0, 0, 64, 64 }, playerTexture, Colors_White, {6, 4}, { 6, 6, 6, 4 }));
     enemy = renderData.addShape<Rect>(Rect(maze, { glm::vec2(6, 6) * glm::vec2(64, 64), 64, 64}, Colors_Red));

     timeDisplay = renderData.addShape<TextBox>(TextBox({ 600, 100, 150, 50 }, Colors_Transparent, font,  50));
     scoreDisplay = renderData.addShape<TextBox>(TextBox(timeDisplay, { 0, 100, 150, 50 }, Colors_Transparent, font, 50));

     undoBtn = renderData.addShape<Button>(Button(scoreDisplay, {0, 100, 150, 50}, Colors_Gray));
    resetBtn = renderData.addShape<Button>(Button(undoBtn, {0, 100, 150, 50}, Colors_White));
     menuBtn = renderData.addShape<Button>(Button(resetBtn, {0, 100, 150, 50}, Colors_Green));

    maze->mazeEncode = maze2;
    maze->texture = mazeTexture;
    if (data.changed)
    {
        player->pos = data.playerPos;
        enemy->pos = data.enemyPos;
        timeDisplay->getText() = Utils::timeToDate(data.time);
        scoreDisplay->getText() = "Score: " + std::to_string(data.score);
    }
    else
    {
        data.playerPos = player->pos;
        data.enemyPos = enemy->pos;
    }
    data.changed = true;
}

void GameplayState::onExit()
{
    renderData.reset();
    m_renderBuffer.getReadBuffer() = {};
    data.changed = true;
}

void GameplayState::update(double dt)
{
    glm::vec2& playerPos = player->pos;
    glm::vec2& playerSize = player->size;
    glm::vec2& enemyPos = enemy->pos;
    glm::vec2& enemySize = enemy->size;

    data.time += dt;
    timeDisplay->getText() = "Time: " + Utils::timeToDate(data.time);

    scoreDisplay->getText() = "Score: " + std::to_string(data.score);

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
        data.stepHistory.push_back(data.playerPos);
        data.enemyStepHistory.push_back(data.enemyPos);

        if (move.y > 0) player->setAnimation(0, 1.5, dt);
        if (move.y < 0) player->setAnimation(1, 1.5, dt);
        if (move.x > 0) player->setAnimation(2, 1.5, dt);
        if (move.x < 0) player->setAnimation(2, 1.5, dt, true);

        move *= 64;
        data.playerPos += move;
        move = {};

        data.score += 150;

        glm::ivec2 mazePos = (data.playerPos - maze->pos) / maze->tileSize.x;
        glm::ivec2 enemyMZPos = (data.enemyPos - maze->pos) / maze->tileSize.x;

        data.enemyPath = PathFinding<BFS_Heuristic>::run({ enemyMZPos.x, enemyMZPos.y }, { mazePos.x, mazePos.y }, { 8, 8 }, maze2);
        data.enemyMaxStep = std::min((unsigned long long)2, data.enemyPath.size() - 1);
        data.enemyStep = 0;
    }

    if (enemyPos == data.enemyPos && data.enemyStep < data.enemyMaxStep)
    {
        glm::ivec2 enemyMZPos = (data.enemyPos - maze->pos) / maze->tileSize.x;
        int step = data.enemyStep + 1;
        data.enemyPos += (glm::ivec2(data.enemyPath[step].first, data.enemyPath[step].second) - enemyMZPos) * 64;
        data.enemyStep++;
    }

    if (playerPos == data.playerPos) player->setAnimation(3, 3, dt);

    playerPos = Utils::lerp2(playerPos, data.playerPos, 0.1f, 3.0f);

    enemyPos = Utils::lerp2(enemyPos, data.enemyPos, 0.1f, 3.0f);

    for (auto& a : renderData.shapes)
    {
        a->runAnimation();
    }

    //Get buffer to send to render thread
    RenderData& base = m_renderBuffer.getWriteBuffer();
    base = renderData;
    m_renderBuffer.swap();
}

void GameplayState::render()
{
    RenderData renderData(m_renderBuffer.getReadBuffer());

    gl2d::Renderer2D& renderer = app->getRenderer();

    renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

    for (auto& a : renderData.shapes)
    {
        if (a) a->render(&renderer);
    }
}