#pragma once
#include <Core.hpp>
#include <Application/IApplication.hpp>
#include <State/State.hpp>
#include <Utils/Utils.hpp>
#include <Shape/UI.hpp>

class PauseMenu : public State
{
public:
    PauseMenu(IApplication* _app)
        : data(*static_cast<GameplayData*>(_app->getCTX()))
    {
        app = _app;
        label = "PauseMenu";
        font.createFromFile(FONT_PATH "BoldPixels.ttf");
        playBtnTexture.loadFromFile(ASSETS_PATH "Start button.png", true);
        saveBtnTexture.loadFromFile(ASSETS_PATH "Save button.png", true);
        loadBtnTexture.loadFromFile(ASSETS_PATH "Load button.png", true);
        menuBtnTexture.loadFromFile(ASSETS_PATH "Menu button.png", true);
        settingsBtnTexture.loadFromFile(ASSETS_PATH "Setting button.png", true);

        pauseSFX.load(app->getAudioEngine(), SFX_PATH "pauseSFX.mp3");
    }

    void onEnter()
    {
        renderData = RenderData();
        background = renderData.addShape<Rect>(Rect({ 0,0,1200,805 }, { 0, 0, 0, 0.7 }));
        playBtn = renderData.addShape<Button>(Button(ui, { 300, 300, glm::vec2(27, 26) * 4.0f }, playBtnTexture, Colors_White, { 2, 1 }));
        saveBtn = renderData.addShape<Button>(Button(ui, playBtn, { 150, 0, glm::vec2(27, 26) * 4.0f }, saveBtnTexture, Colors_White, { 2, 1 }));
        loadBtn = renderData.addShape<Button>(Button(ui, saveBtn, { 150, 0, glm::vec2(27, 26) * 4.0f }, loadBtnTexture, Colors_White, { 2, 1 }));
        menuBtn = renderData.addShape<Button>(Button(ui, loadBtn, { 150, 0, glm::vec2(27, 26) * 4.0f }, menuBtnTexture, Colors_White, { 2, 1 }));
        settingsBtn = renderData.addShape<Button>(Button(ui, menuBtn, { 150, 0, glm::vec2(27, 26) * 4.0f }, settingsBtnTexture, Colors_White, { 2, 1 }));

        if (pauseSFX.isFinished()) pauseSFX.play();
    }

    void onExit()
    {
        if (!pauseSFX.isFinished()) pauseSFX.stop();
        renderData.reset();
        m_renderBuffer.getReadBuffer() = {};
    }

    void handleInput(const KeyInputState& in)
    {
        MousePos mousePos = app->getMousePos();

        vp = app->getViewportScale();

        ui.processUI(in, mousePos, vp);

        if (playBtn->clicked() || in.keyReleased[GLFW_KEY_ESCAPE])
        {
            app->getStateStack().queueTransit(this, "");
            app->getStateStack().queueSuspend("GameplayState", false, false, false);
        }

        if (saveBtn->clicked())
        {
            app->getStateStack().queueTransit(this, "SaveLoad");
            data.saveloadMode = 1;
        }

        if (loadBtn->clicked())
        {
            app->getStateStack().queueTransit(this, "SaveLoad");
            data.saveloadMode = 0;
        }

        if (menuBtn->clicked())
        {
            app->getStateStack().queueTransit(this, "");
            app->getStateStack().queueTransit("GameplayState", "GameMenu");
            app->getStateStack().queueSuspend("GameplayState", false, false, false);
            data.changed = false;

            data.playerPos = {};
            data.enemyPos.clear();

            data.stepHistory.clear();
            data.enemyStepHistory.clear();
            data.enemyStep.clear();
            data.enemyMaxStep.clear();
        }

        if (settingsBtn->clicked())
        {
            app->getStateStack().queueTransit(this, "SettingsMenu");
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

        vp = app->getViewportScale();

        renderer.clearScreen({ 0, 0, 0, 1 });

        for (auto& a : renderData.shapes)
        {
            if (a) a->render(&renderer, vp);
        }
    }
private:
    RenderData renderData;

    ViewportScale vp = {};

    GameplayData& data;

    UI ui;

    Sound pauseSFX{ AudioCategory::InteractSFX };

    gl2d::Font font;

    gl2d::Texture playBtnTexture;
    gl2d::Texture saveBtnTexture;
    gl2d::Texture loadBtnTexture;
    gl2d::Texture menuBtnTexture;
    gl2d::Texture settingsBtnTexture;

    Rect* background = nullptr;

    Button* playBtn = nullptr;
    Button* saveBtn = nullptr;
    Button* loadBtn = nullptr;
    Button* menuBtn = nullptr;
    Button* settingsBtn = nullptr;

    DoubleBuffer<RenderData> m_renderBuffer;
};