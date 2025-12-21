#include <GameMechanics/GameplayState.hpp>

GameplayState::GameplayState(IApplication* _app)
    : data(*static_cast<GameplayData*>(_app->getCTX()))
{
    app = _app;
    label = "GameplayState";
    playerTexture.loadFromFile(ASSETS_PATH "Main.png", true);
    gameplayBgTexture.loadFromFile(ASSETS_PATH "game background.png", true);
    statusBgTexture.loadFromFile(ASSETS_PATH "status board.png", true);
    statBgTexture.loadFromFile(ASSETS_PATH "text box.png", true);

    enemy1Texture.loadFromFile(ASSETS_PATH "TTung.png", true);
    enemy2Texture.loadFromFile(ASSETS_PATH "TTinh.png", true);
    enemy3Texture.loadFromFile(ASSETS_PATH "TQuan.png", true);
    goalTexture.loadFromFile(ASSETS_PATH "goal.png", true);

    mazeTexture.loadFromFile(ASSETS_PATH "Maze.png", true);
    mazeBgTexture.loadFromFile(ASSETS_PATH "wall.png", true);
    undoTexture.loadFromFile(ASSETS_PATH "Undo button.png", true);
    resetTexture.loadFromFile(ASSETS_PATH "Reset button.png", true);
    pauseTexture.loadFromFile(ASSETS_PATH "Pause button.png", true);

    font.createFromFile(FONT_PATH "BoldPixels.ttf");
    bgm.load(app->getAudioEngine(), SFX_PATH "BGM.mp3", true);
}

void GameplayState::handleInput(const KeyInputState& in)
{
    MousePos mousePos = app->getMousePos();
    vp = app->getViewportScale();

    ui.processUI(in, mousePos, vp);

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

    if (pauseBtn->clicked() || in.keyReleased[GLFW_KEY_ESCAPE])
    {
        app->getStateStack().queueTransit(nullptr, "PauseMenu");
        if (!bgm.isFinished()) bgm.stop();
        app->getStateStack().queueSuspend(this, true, false, true);
    }

    if ((in.keyDown[GLFW_KEY_LEFT_CONTROL] && in.keyPressed[GLFW_KEY_Z]) || undoBtn->clicked())
    {
        if (!data.stepHistory.empty())
        {
            data.playerPos = data.stepHistory.back();
            data.stepHistory.pop_back();
            undoing = true;
        }
    }

    if (resetBtn->clicked())
    {
        std::cout << "Button pressed!\n";

        data.playerPos = data.startPos * charSize;
        for(int i = 0; i < data.enemyStartPos.size(); ++i)
            data.enemyPos[i] = data.enemyStartPos[i] * charSize;

        player->pos = data.playerPos;

        for (int i = 0; i < enemies.size(); ++i)
        {
            enemies[i]->pos = data.enemyPos[i];
        }

        data.score = 0;
        data.time = 0;
        data.stepHistory.clear();
        data.enemyStepHistory.clear();
        data.enemyStep.clear();
        data.enemyMaxStep.clear();

        data.enemyStepHistory.resize(data.enemyStartPos.size());
        data.enemyStep.resize(data.enemyStartPos.size());
        data.enemyMaxStep.resize(data.enemyStartPos.size());
    }
}

void GameplayState::onEnter()
{
    renderData = RenderData();
    background = renderData.addShape<Rect>(Rect({ 0,0,1200,805 }, gameplayBgTexture, Colors_White));
    
    mazeBg = renderData.addShape<Rect>(Rect({ 0,0, 690, 805 }, mazeBgTexture, Colors_White));
      maze = renderData.addShape<Maze>(Maze(mazeBg, { 29, 140 }, data.mazeSize, charSize));
    player = renderData.addShape<Rect>(Rect(maze, { data.startPos * charSize, charSize }, playerTexture, Colors_White, {6, 4}, { 6, 6, 6, 4 }));

    goal = renderData.addShape<Rect>(Rect(maze, {data.goalPos * charSize, charSize}, goalTexture, Colors_White));

    enemies.resize(data.enemyStartPos.size());
    data.enemyStepHistory.resize(data.enemyStartPos.size());

    gl2d::Texture& enemyTexture = (data.level == 0 ? enemy1Texture : (data.level == 1 ? enemy2Texture : enemy3Texture));

    for (int i = 0; i < data.enemyStartPos.size(); ++i)
    {
        enemies[i] = renderData.addShape<Rect>(Rect(maze, {data.enemyStartPos[i] * charSize, charSize }, enemyTexture, Colors_White, {6, 4}, {6, 6, 6, 2}));
    }

     statusBg = renderData.addShape<Rect>(Rect({ 700, 60, glm::vec2(432, 549) * 1.1f }, statusBgTexture, Colors_White));

     timeBg = renderData.addShape<Rect>(Rect(statusBg, { 60, 120, 273 * 1.3f, 78 }, statBgTexture, Colors_White));
     timeDisplay = renderData.addShape<TextBox>(TextBox(ui, timeBg, { 50, 8, 150, 50 }, Colors_Transparent, font, 40));

     scoreBg = renderData.addShape<Rect>(Rect(timeBg, { 0, 90, 273 * 1.3f, 78 }, statBgTexture, Colors_White));
     scoreDisplay = renderData.addShape<TextBox>(TextBox(ui, scoreBg, { 50, 8, 150, 50 }, Colors_Transparent, font, 40));

     undoBtn = renderData.addShape<Button>(Button(ui, scoreDisplay, { 10, 100, glm::vec2(27, 26) * 4.0f }, undoTexture, Colors_White, { 2, 1 }));
     resetBtn = renderData.addShape<Button>(Button(ui, undoBtn, { 120, 0, glm::vec2(27, 26) * 4.0f }, resetTexture, Colors_White, { 2, 1 }));
     pauseBtn = renderData.addShape<Button>(Button(ui, undoBtn, { 60, 120, glm::vec2(27, 26) * 4.0f }, pauseTexture, Colors_White, { 2, 1 }));

    vp = app->getViewportScale();

    maze->mazeEncode = data.mazeEncode;
    maze->texture = mazeTexture;

    data.time = 0;

    if (data.changed)
    {
        player->pos = data.playerPos;
        for (int i = 0; i < enemies.size(); ++i)
        {
            enemies[i]->pos = data.enemyPos[i];
        }
        //timeDisplay->getText() = Utils::timeToDate(data.time);
        scoreDisplay->getText() = "Score: " + std::to_string(data.score);
    }
    else
    {
        data.playerPos = player->pos;
        data.enemyPos.resize(enemies.size());
        for (int i = 0; i < enemies.size(); ++i)
        {
            data.enemyPos[i] = enemies[i]->pos;
        }
        data.changed = true;
    }
}

void GameplayState::onExit()
{
    if (!bgm.isFinished()) bgm.stop();
    renderData.reset();
    m_renderBuffer.getReadBuffer() = {};
    data.changed = true;
}

void GameplayState::update(double dt)
{
    if (bgm.isFinished()) bgm.play();

    auto& playerPos = player->pos;
    auto enemyNum = enemies.size();
    auto& mazeEncode = maze->mazeEncode;

    data.time += dt;
    timeDisplay->getText() = "Time: " + Utils::timeToDate(data.time);

    scoreDisplay->getText() = "Score: " + std::to_string(data.score);

    //Maze mechanism handling
    if (Utils::inRect(data.playerPos, charSize, {}, maze->size))
    {
        glm::vec2 mazePos = (data.playerPos) / charSize * 2.0f + 1.0f;
        if (move.x == 1)   move.x *= mazeEncode[mazePos.x + maze->encodeSize.x * mazePos.y + 1];
        if (move.x == -1)  move.x *= mazeEncode[mazePos.x + maze->encodeSize.x * mazePos.y - 1];
        if (move.y == 1)   move.y *= mazeEncode[mazePos.x + maze->encodeSize.x * (mazePos.y + 1)];
        if (move.y == -1)  move.y *= mazeEncode[mazePos.x + maze->encodeSize.x * (mazePos.y - 1)];
    }

    //Win
    if (playerPos == data.goalPos * charSize)
    {
        bgm.stop();
        data.wlState = 1;
        data.maxScore += data.score;
        app->getStateStack().queueTransit(nullptr, "GameWL");
        app->getStateStack().queueSuspend(this, true, false, true);

        data.playerPos = data.startPos * charSize;
        for (int i = 0; i < data.enemyStartPos.size(); ++i)
            data.enemyPos[i] = data.enemyStartPos[i] * charSize;

        player->pos = data.playerPos;

        for (int i = 0; i < enemies.size(); ++i)
        {
            enemies[i]->pos = data.enemyPos[i];
        }

        data.stepHistory.clear();
        data.enemyStepHistory.clear();
        data.enemyStep.clear();
        data.enemyMaxStep.clear();

        data.enemyStepHistory.resize(data.enemyStartPos.size());
        data.enemyStep.resize(data.enemyStartPos.size());
        data.enemyMaxStep.resize(data.enemyStartPos.size());
    }

    //Lose
    else for(int i = 0; i < enemyNum; ++i)
    if (playerPos == enemies[i]->pos)
    {
        bgm.stop();
        data.wlState = 2;
        data.maxScore += data.score;
        app->getStateStack().queueTransit(nullptr, "GameWL");
        app->getStateStack().queueSuspend(this, true, false, true);

        data.playerPos = data.startPos * charSize;
        for (int i = 0; i < data.enemyStartPos.size(); ++i)
            data.enemyPos[i] = data.enemyStartPos[i] * charSize;

        player->pos = data.playerPos;

        for (int i = 0; i < enemies.size(); ++i)
        {
            enemies[i]->pos = data.enemyPos[i];
        }

        data.stepHistory.clear();
        data.enemyStepHistory.clear();
        data.enemyStep.clear();
        data.enemyMaxStep.clear();

        data.enemyStepHistory.resize(data.enemyStartPos.size());
        data.enemyStep.resize(data.enemyStartPos.size());
        data.enemyMaxStep.resize(data.enemyStartPos.size());
    }

    //Movement handling
    if ((move.x != 0 || move.y != 0) && data.playerPos == playerPos && !undoing && data.enemyStep[0] >= data.enemyMaxStep[0])
    {
        data.stepHistory.push_back(data.playerPos);

        move *= 64;
        data.playerPos += move;
        move = {};

        data.score += 150;

        glm::ivec2 mazePos = data.playerPos / charSize;

        for (int i = 0; i < enemyNum; ++i)
        {
            glm::ivec2 enemyMZPos = data.enemyPos[i] / charSize;

            data.enemyPath[i] = (data.difficulty == 0) ?
                PathFinding<BFS_Heuristic>::run({ enemyMZPos.x, enemyMZPos.y }, { mazePos.x, mazePos.y }, { (int)data.mazeSize.x, (int)data.mazeSize.y }, mazeEncode) :
                PathFinding<Dijkstra>::run({enemyMZPos.x, enemyMZPos.y}, {mazePos.x, mazePos.y}, { data.mazeSize.x, data.mazeSize.y }, mazeEncode);
            data.enemyMaxStep[i] = (std::min)((unsigned long long)2, data.enemyPath[i].size() - 1);
            data.enemyStep[i] = 0;
            data.enemyStepHistory[i].emplace_back();
        }
    }
    
    for (int i = 0; i < enemyNum; ++i)
    {
        auto& enemyPos          = enemies[i]->pos;
        auto& enemyTargetPos    = data.enemyPos[i];
        auto& enemyStepHistory  = data.enemyStepHistory[i];
        auto& enemyPath         = data.enemyPath[i];
        auto& enemyStep         = data.enemyStep[i];
        auto& enemyMaxStep      = data.enemyMaxStep[i];

        //Building step history
        if (enemyPos == enemyTargetPos && enemyStep < enemyMaxStep && !undoing)
        {
            glm::vec2 enemyMZPos = enemyTargetPos / charSize;
            int step = enemyStep + 1;
            enemyStepHistory.back().push_back(enemyTargetPos);
            enemyTargetPos += (glm::vec2(enemyPath[step].first, enemyPath[step].second) - enemyMZPos) * charSize;
            enemyStep++;
        }

        //Undoing steps
        if (enemyPos == enemyTargetPos && undoing && !enemyStepHistory.empty())
        {
            if (!enemyStepHistory.back().empty())
            {
                enemyTargetPos = enemyStepHistory.back().back();
                enemyStepHistory.back().pop_back();
            }
            else {
                enemyStepHistory.pop_back();
                undoing = false;
            }
        }
    }

    // Animation handling
    if (data.playerPos.y - playerPos.y > 0) player->setAnimation(0, 1.5, dt);
    if (data.playerPos.y - playerPos.y < 0) player->setAnimation(1, 1.5, dt);
    if (data.playerPos.x - playerPos.x > 0) player->setAnimation(2, 1.5, dt);
    if (data.playerPos.x - playerPos.x < 0) player->setAnimation(2, 1.5, dt, true);
    if (playerPos == data.playerPos) player->setAnimation(3, 3, dt);

    //Enemies animation
    for (int i = 0; i < enemyNum; ++i)
    {
        if (data.enemyPos[i].y - enemies[i]->pos.y > 0) enemies[i]->setAnimation(0, 1.5, dt);
        if (data.enemyPos[i].y - enemies[i]->pos.y < 0) enemies[i]->setAnimation(1, 1.5, dt);
        if (data.enemyPos[i].x - enemies[i]->pos.x > 0) enemies[i]->setAnimation(2, 1.5, dt);
        if (data.enemyPos[i].x - enemies[i]->pos.x < 0) enemies[i]->setAnimation(2, 1.5, dt, true);
        if (enemies[i]->pos == data.enemyPos[i]) enemies[i]->setAnimation(3, 2, dt);
    }

    playerPos = Utils::lerp2(playerPos, data.playerPos, 0.1f, 3.0f);

    for(int i = 0; i < enemyNum; ++i)
        enemies[i]->pos = Utils::lerp2(enemies[i]->pos, data.enemyPos[i], 0.1f, 3.0f);

    for (auto& a : renderData.shapes)
        a->runAnimation();

    //Get buffer to send to render thread
    RenderData& base = m_renderBuffer.getWriteBuffer();
    base = renderData;
    m_renderBuffer.swap();
}

void GameplayState::render()
{
    RenderData renderData(m_renderBuffer.getReadBuffer());

    gl2d::Renderer2D& renderer = app->getRenderer();

    vp = app->getViewportScale();

    renderer.clearScreen({ 0, 0, 0, 1 });

    for (auto& a : renderData.shapes)
    {
        if (a) a->render(&renderer, vp);
    }
}