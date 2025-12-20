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
        menuBtnTexture.loadFromFile(ASSETS_PATH "Menu button.png", true);
        sliderBodyTexture.loadFromFile(ASSETS_PATH "volume slider.png", true);
        knobTexture.loadFromFile(ASSETS_PATH "button volume.png", true);
    }

    void onEnter()
    {
        renderData = RenderData();
        background = renderData.addShape<Rect>(Rect({0,0,1200,805}, {0, 0, 0, 0.7}));
        menuBtn = renderData.addShape<Button>(Button(ui, { 100, 700, glm::vec2(27, 26) * 2.0f }, menuBtnTexture, Colors_White, { 2, 1 }));

        masterVolume = renderData.addShape<Slider>(Slider(ui, {100, 100, glm::vec2(500, 100) * 0.7f}, sliderBodyTexture, knobTexture));
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

        if (menuBtn->clicked())
        {
            if (app->getStateStack().findActiveState("MainMenu"))
            {
                app->getStateStack().queueTransit(this, "");
                app->getStateStack().queueSuspend("MainMenu", false, false, false);
            }
            else if (app->getStateStack().findInactiveState("PauseMenu"))
            {
                app->getStateStack().queueTransit(this, "PauseMenu");
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

    gl2d::Texture menuBtnTexture;
    gl2d::Texture sliderBodyTexture;
    gl2d::Texture knobTexture;

    Rect* background = nullptr;

    Button* menuBtn = nullptr;

    Slider* masterVolume = nullptr;
    Slider* musicVolume = nullptr;
    Slider* gameVolume = nullptr;
    Slider* interactVolume = nullptr;

    DoubleBuffer<RenderData> m_renderBuffer;
};