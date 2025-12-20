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
        titleTexture.loadFromFile(ASSETS_PATH "text box.png", true);
    }

    void onEnter() override
    {
        renderData = RenderData();

        title = renderData.addShape<TextBox>(TextBox(ui, { 470, 80, 512, 64 }, Colors_White, font, 64, false));
        title->getText() = "New Game";

        border = renderData.addShape<Rect>(Rect({280, 300, 273 * 2.5f, 78 * 2.0f}, titleTexture, Colors_White));
        textbox = renderData.addShape<TextBox>(TextBox(ui, border, { 105, 45, 480, 64 }, Colors_Transparent, font, 64, true, 12));

        levelSelect = renderData.addShape<TextBox>(TextBox(ui, {550, 350, 50, 50}, Colors_Gray, font, 50));
        levelSelect->allowRender = false;
        
        menuBtn = renderData.addShape<Button>(Button(ui, { 100, 700, glm::vec2(27, 26) * 2.0f }, menuBtnTexture, Colors_White, {2, 1}));

        vp = app->getViewportScale();

        if (data.changed)
        {
            textbox->getText() = data.name;
            textbox->allowRender = false;
            border->allowRender = false;
            levelSelect->allowRender = true;
            title->getText() = "Select Map";
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

        if (in.keyPressed[GLFW_KEY_P])
        {
            app->getStateStack().queueTransit(this, "GameplayState");
        }

        if (in.keyReleased[GLFW_KEY_ENTER])
        {
            textbox->unfocus();
            textbox->allowRender = false;
            border->allowRender = false;
            levelSelect->allowRender = true;
            title->getText() = "Select Map";
        }

        if (levelSelect->allowRender)
        {
            if (in.keyReleased[GLFW_KEY_RIGHT]) data.level++;
            if (in.keyReleased[GLFW_KEY_LEFT]) data.level--;

            levelSelect->getText() = std::to_string(data.level);
        }
        if(in.mouseReleased[GLFW_MOUSE_BUTTON_LEFT])
        if (levelSelect->clicked()) //Load map
        {
            std::string mapDir = MAPS_PATH"map" + std::to_string(data.level + 1) + ".txt";

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

            app->getStateStack().queueTransit(this, "GameplayState");
        }

        if (menuBtn->clicked())
        {
            app->getStateStack().queueTransit("GameMenu", "MainMenu");
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

    gl2d::Texture menuBtnTexture;
    gl2d::Texture titleTexture;

    TextBox* title = nullptr;
    TextBox* textbox = nullptr;
    TextBox* levelSelect = nullptr;

    Button* menuBtn = nullptr;

    Rect* border = nullptr;

    gl2d::Font font;

    DoubleBuffer<RenderData> m_renderBuffer;
};