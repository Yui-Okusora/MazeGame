#pragma once
#include <Core.hpp>
#include <Application/IApplication.hpp>
#include <State/State.hpp>
#include <Utils/Utils.hpp>
#include <Shape/UI.hpp>

class SettingsMenu : public State
{
public:
    SettingsMenu(IApplication* _app)
    {
        app = _app;
        label = "SettingsMenu";
        font.createFromFile(FONT_PATH "BoldPixels.ttf");
        playBtnTexture.loadFromFile(ASSETS_PATH "Start button.png", true);
    }

    void onEnter()
    {
        renderData = RenderData();
        background = renderData.addShape<Rect>(Rect({0,0,1200,805}, {0, 0, 0, 0.7}));
        playBtn = renderData.addShape<Button>(Button(ui, {300, 300, glm::vec2(27, 26) * 4.0f}, playBtnTexture, Colors_White, {2, 1}));
    }

    void onExit()
    {
        renderData.reset();
        m_renderBuffer.getReadBuffer() = {};
    }

    void handleInput(const KeyInputState& in)
    {
        MousePos mousePos = app->getMousePos();

        vp = app->getViewportScale();

        ui.processUI(in, mousePos, vp);

        if (playBtn->clicked())
        {
            if (app->getStateStack().findActiveState("MainMenu"))
            {
                app->getStateStack().queueTransit(this, "");
                app->getStateStack().queueSuspend("MainMenu", false, false, false);
            }
            else if (app->getStateStack().findInactiveState("PauseMenu"))
            {
                app->getStateStack().queueTransit(this, "PauseMenu");
                //app->getStateStack().queueSuspend("GameplayState", false, false, false);
            }
        }
    }

    void update(double dt)
    {
        RenderData& base = m_renderBuffer.getWriteBuffer();
        base = renderData;
        m_renderBuffer.swap();
    }
    void render()
    {
        RenderData renderData(m_renderBuffer.getReadBuffer());

        gl2d::Renderer2D& renderer = app->getRenderer();

        renderer.clearScreen({ 0, 0, 0, 1 });

        for (auto& a : renderData.shapes)
        {
            if (a) a->render(&renderer, vp);
        }
    }
private:
    RenderData renderData;

    ViewportScale vp = {};

    UI ui;

    gl2d::Font font;

    gl2d::Texture playBtnTexture;

    Rect* background = nullptr;

    Button* playBtn = nullptr;
    Button* saveBtn = nullptr;
    Button* loadBtn = nullptr;
    Button* menuBtn = nullptr;

    DoubleBuffer<RenderData> m_renderBuffer;
};