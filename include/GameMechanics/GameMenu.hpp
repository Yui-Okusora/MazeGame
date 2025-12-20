#pragma once

#include <Core.hpp>
#include <Utils/Utils.hpp>
#include <Application/IApplication.hpp>
#include <Shape/UI.hpp>

class GameMenu : public State
{
public:
    GameMenu(IApplication* _app)
        : data(*static_cast<GameplayData*>(_app->getCTX()))
    {
        app = _app;
        label = "GameMenu";
        font.createFromFile(FONT_PATH "BoldPixels.ttf");
        menuBtnTexture.loadFromFile(ASSETS_PATH "Menu button.png", true);
        gameplayBgTexture.loadFromFile(ASSETS_PATH "Backgroundinput name.png", true);
        titleTexture.loadFromFile(ASSETS_PATH "text box.png", true);

        easyTexture.loadFromFile(ASSETS_PATH "easy.png", true);
        medTexture.loadFromFile(ASSETS_PATH "med.png", true);
        hardTexture.loadFromFile(ASSETS_PATH "hard.png", true);
    }

    void onEnter() override
    {
        renderData = RenderData();

        background = renderData.addShape<Rect>(Rect({ 0,0,1200,805 }, gameplayBgTexture, Colors_White));

        title1 = renderData.addShape<TextBox>(TextBox(ui, { 470, 80, 512, 64 }, Colors_Transparent, font, 64, false));
        title2 = renderData.addShape<TextBox>(TextBox(ui, { 430, 80, 512, 64 }, Colors_Transparent, font, 64, false));
        title1->textColor = Colors_White;
        title2->textColor = Colors_White;
        title1->getText() = "New Game";
        title2->getText() = "Select Map";
        title2->allowRender = false;
        
        border = renderData.addShape<Rect>(Rect({280, 300, 273 * 2.5f, 78 * 2.0f}, titleTexture, Colors_White));
        textbox = renderData.addShape<TextBox>(TextBox(ui, border, { 105, 45, 480, 64 }, Colors_Transparent, font, 64, true, 12));
        
        menuBtn = renderData.addShape<Button>(Button(ui, { 100, 700, glm::vec2(27, 26) * 2.0f }, menuBtnTexture, Colors_White, {2, 1}));

        easyBtn = renderData.addShape<Button>(Button(ui, { 135, 200, glm::vec2(366, 450) * 0.5f}, easyTexture, Colors_White, {2, 1}));
        medBtn = renderData.addShape<Button>(Button(ui, easyBtn, { 380, 0, glm::vec2(366, 450) * 0.5f }, medTexture, Colors_White, {2, 1}));
        hardBtn = renderData.addShape<Button>(Button(ui, medBtn, { 380, 0, glm::vec2(366, 450) * 0.5f }, hardTexture, Colors_White, {2, 1}));
        easyBtn->allowRender = false;
        medBtn->allowRender = false;
        hardBtn->allowRender = false;


        vp = app->getViewportScale();

        if (data.changed)
        {
            textbox->getText() = data.name;
            textbox->allowRender = false;
            border->allowRender = false;

            easyBtn->allowRender = true;
            medBtn->allowRender = true;
            hardBtn->allowRender = true;
            
            title1->allowRender = false;
            title2->allowRender = true;
        }
    }

    void onExit() override
    {
        renderData.reset();
        m_renderBuffer.getReadBuffer() = {};
    }

    void handleInput(const KeyInputState& in) override
    {
        MousePos mousePos = app->getMousePos();
        vp = app->getViewportScale();

        ui.processUI(in, mousePos, vp);

        if (in.keyReleased[GLFW_KEY_ENTER])
        {
            textbox->unfocus();
            textbox->allowRender = false;
            border->allowRender = false;

            easyBtn->allowRender = true;
            medBtn->allowRender = true;
            hardBtn->allowRender = true;

            title1->allowRender = false;
            title2->allowRender = true;
        }

        if (easyBtn->clicked()) //Load map
        {
            std::string mapDir = MAPS_PATH"map" + std::to_string(1) + ".txt";

            std::fstream file(mapDir, std::ios::in);
            MazeData mazeData;
            file >> mazeData;

            data.difficulty = mazeData.difficulty;
            data.mazeSize = mazeData.mazeSize;
            data.mazeEncode = mazeData.mazeEncode;
            data.startPos = mazeData.startPos;
            data.goalPos = mazeData.goalPos;

            data.enemyStartPos = mazeData.enemyPosList;
            data.enemyStep.resize(mazeData.enemyNum);
            data.enemyMaxStep.resize(mazeData.enemyNum);
            data.enemyPath.resize(mazeData.enemyNum);

            data.changed = false;
            data.level = 0;
            data.score = 0;

            app->getStateStack().queueTransit(this, "GameplayState");
        }

        if (medBtn->clicked()) //Load map
        {
            std::string mapDir = MAPS_PATH"map" + std::to_string(2) + ".txt";

            std::fstream file(mapDir, std::ios::in);
            MazeData mazeData;
            file >> mazeData;

            data.difficulty = mazeData.difficulty;
            data.mazeSize = mazeData.mazeSize;
            data.mazeEncode = mazeData.mazeEncode;
            data.startPos = mazeData.startPos;
            data.goalPos = mazeData.goalPos;

            data.enemyStartPos = mazeData.enemyPosList;
            data.enemyStep.resize(mazeData.enemyNum);
            data.enemyMaxStep.resize(mazeData.enemyNum);
            data.enemyPath.resize(mazeData.enemyNum);

            data.changed = false;
            data.level = 1;
            data.score = 0;

            app->getStateStack().queueTransit(this, "GameplayState");
        }

        if (hardBtn->clicked()) //Load map
        {
            std::string mapDir = MAPS_PATH"map" + std::to_string(3) + ".txt";

            std::fstream file(mapDir, std::ios::in);
            MazeData mazeData;
            file >> mazeData;

            data.difficulty = mazeData.difficulty;
            data.mazeSize = mazeData.mazeSize;
            data.mazeEncode = mazeData.mazeEncode;
            data.startPos = mazeData.startPos;
            data.goalPos = mazeData.goalPos;

            data.enemyStartPos = mazeData.enemyPosList;
            data.enemyStep.resize(mazeData.enemyNum);
            data.enemyMaxStep.resize(mazeData.enemyNum);
            data.enemyPath.resize(mazeData.enemyNum);

            data.changed = false;
            data.level = 2;
            data.score = 0;

            app->getStateStack().queueTransit(this, "GameplayState");
        }

        if (menuBtn->clicked())
        {
            app->getStateStack().queueTransit("GameMenu", "MainMenu");
            app->getStateStack().queueSuspend("MainMenu");
            data.changed = false;
        }

        textbox->clicked();
    }

    void update(double dt) override
    {
        if (textbox->getText() != data.name && !textbox->isFocus())
        {
            data.name = textbox->getText();
        }

        RenderData& base = m_renderBuffer.getWriteBuffer();
        base = renderData;
        m_renderBuffer.swap();
    }

    void render() override
    {
        RenderData renderData(m_renderBuffer.getReadBuffer());

        gl2d::Renderer2D& renderer = app->getRenderer();

        renderer.clearDrawData();

        renderer.clearScreen({ 1, 1, 1, 1 });

        for (auto& a : renderData.shapes)
        {
            if (a) a->render(&renderer, vp);
        }
    }

    RenderData renderData;
    ViewportScale vp = {};

    UI ui;

    GameplayData& data;

    gl2d::Texture gameplayBgTexture;
    gl2d::Texture menuBtnTexture;
    gl2d::Texture titleTexture;
    gl2d::Texture easyTexture;
    gl2d::Texture medTexture;
    gl2d::Texture hardTexture;

    TextBox* title1 = nullptr;
    TextBox* title2 = nullptr;
    TextBox* textbox = nullptr;
    TextBox* levelSelect = nullptr;

    Button* menuBtn = nullptr;
    Button* easyBtn = nullptr;
    Button* medBtn = nullptr;
    Button* hardBtn = nullptr;

    Rect* background = nullptr;
    Rect* border = nullptr;

    gl2d::Font font;

    DoubleBuffer<RenderData> m_renderBuffer;
};