#pragma once

#include <Core.hpp>
#include <Utils/Utils.hpp>
#include <Application/IApplication.hpp>
#include <Shape/Shape.hpp>
#include <Shape/UI.hpp>

class MainMenu : public State
{
public:
    MainMenu(IApplication* _app)
        : data(*static_cast<GameplayData*>(_app->getCTX()))
    {
        app = _app;
        label = "MainMenu";
        font.createFromFile(FONT_PATH "calibril.ttf");
        bgTexture.loadFromFile("resources\\start menu game.png");
        playTexture.loadFromFile("resources\\Play button.png");
        continuesTexture.loadFromFile("resources\\Continue button.png");
        settingsTexture.loadFromFile("resources\\Option button.png");
        quitTexture.loadFromFile("resources\\Exit button.png");
    }

    void onEnter() override
    {
        renderData = RenderData();

        background = renderData.addShape<Rect>(Rect({ 0,0,app->getFramebufferSize().x,app->getFramebufferSize().x }, bgTexture, Colors_White));


        start = renderData.addShape<Button>(Button({ 100, 370, 192, 72 }, playTexture, Colors_White, { 2, 1 }));
        continues = renderData.addShape<Button>(Button(start, { 200, 0, 192, 72 }, continuesTexture, Colors_White, { 2, 1 }));
        settings = renderData.addShape<Button>(Button(start, { 0, 80, 192, 72 }, settingsTexture, Colors_White, { 2, 1 }));
        quit = renderData.addShape<Button>(Button(start, { 200, 80, 192, 72 }, quitTexture, Colors_White, { 2, 1 }));
        
        if (data.changed)
        {
            
        }
        else
        {

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

        UI::processUI(in, mousePos);

        if (in.keyPressed[GLFW_KEY_P])
        {
            app->getStateStack().queueTransit(this, "PauseMenu");
        }
        if (start->clicked()) app->getStateStack().queueTransit(this, "PauseMenu");
        if (quit->clicked()) app->stop();
    }

    void update(double dt) override
    {
        RenderData& base = m_renderBuffer.getWriteBuffer();
        base = renderData;
        m_renderBuffer.swap();
    }

    void render() override
    {
        RenderData renderData(m_renderBuffer.getReadBuffer());

        gl2d::Renderer2D& renderer = app->getRenderer();

        renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

        for (auto& a : renderData.shapes)
        {
            if(a) a->render(&renderer);
        }
    }

    RenderData renderData;

    GameplayData& data;

    Rect* background = nullptr;

    Button* start = nullptr;
    Button* continues = nullptr;
    Button* settings = nullptr;
    Button* quit = nullptr;

    gl2d::Texture bgTexture;
    gl2d::Texture playTexture;
    gl2d::Texture continuesTexture;
    gl2d::Texture settingsTexture;
    gl2d::Texture quitTexture;


    gl2d::Font font;

    DoubleBuffer<RenderData> m_renderBuffer;
};