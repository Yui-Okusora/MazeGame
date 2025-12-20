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

        font.createFromFile(FONT_PATH "BoldPixels.ttf");

        bgTexture.loadFromFile(ASSETS_PATH "start menu game.png", true);
        playTexture.loadFromFile(ASSETS_PATH "Play button.png", true);
        continuesTexture.loadFromFile(ASSETS_PATH "Continue button.png", true);
        settingsTexture.loadFromFile(ASSETS_PATH "Option button.png", true);
        quitTexture.loadFromFile(ASSETS_PATH "Exit button.png", true);

        entrySFX.load(app->getAudioEngine(), SFX_PATH "entry.mp3");
    }

    void onEnter() override
    {
        renderData = RenderData();

        background = renderData.addShape<Rect>(Rect({ 0, 0, 1200, 810 }, bgTexture, Colors_White));

        

        start = renderData.addShape<Button>(Button(ui, { 100, 370, 192, 72 }, playTexture, Colors_White, { 2, 1 }));
        continues = renderData.addShape<Button>(Button(ui, start, { 200, 0, 192, 72 }, continuesTexture, Colors_White, { 2, 1 }));
        settings = renderData.addShape<Button>(Button(ui, start, { 0, 80, 192, 72 }, settingsTexture, Colors_White, { 2, 1 }));
        quit = renderData.addShape<Button>(Button(ui, start, { 200, 80, 192, 72 }, quitTexture, Colors_White, { 2, 1 }));

        vp = app->getViewportScale();

        if (entrySFX.isFinished())
        {
            entrySFX.play();
        }
        
        if (data.changed) data.changed = false;
    }

    void onExit() override
    {
        if (!entrySFX.isFinished()) entrySFX.stop();
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
            app->getStateStack().queueTransit(this, "GameMenu");
            data.changed = false;
        }

        if (start->clicked())
        {
            app->getStateStack().queueTransit(this, "GameMenu");
            data.changed = false;
        }

        if (continues->clicked())
        {
            app->getStateStack().queueTransit(nullptr, "SaveLoad");
            app->getStateStack().queueSuspend(this, true, false, true);
            data.saveloadMode = 0;
            data.changed = false;
        }

        if (settings->clicked())
        {
            app->getStateStack().queueTransit(nullptr, "SettingsMenu");
            app->getStateStack().queueSuspend(this, true, false, true);
        }

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

        renderer.clearScreen({ 0, 0, 0, 1 });

        for (auto& a : renderData.shapes)
        {
            if(a) a->render(&renderer, vp);
        }
    }

    RenderData renderData;
    ViewportScale vp = {};

    UI ui;

    GameplayData& data;

    Sound entrySFX{AudioCategory::GameplaySFX};

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