#pragma once

#include <Core.hpp>
#include <Application/IApplication.hpp>
#include <State/State.hpp>
#include <Shape/Shape.hpp>
#include <RenderData/RenderData.hpp>
#include <GameMechanics/GameplayData.hpp>

class GameWL : public State
{
public:
    GameWL(IApplication* _app)
        : data(*static_cast<GameplayData*>(_app->getCTX()))
    {
        app = _app;
        label = "GameWL";
        font.createFromFile(FONT_PATH "BoldPixels.ttf");
        winTexture.loadFromFile(ASSETS_PATH "win scene.png", true);
        loseTexture.loadFromFile(ASSETS_PATH "lose scene.png", true);
    }

    void onEnter()
    {
        srand(time(0));
        single = true;
        renderData = RenderData();
        background = renderData.addShape<Rect>(Rect({ 0,0,1200,805 }, { 0, 0, 0, 0.7 }));
        wlPoster = renderData.addShape<Rect>(Rect({ -210, -120, glm::vec2(1200, 805) * 1.2f }, (data.wlState == 1 && data.wlState != 0) ? winTexture : loseTexture, Colors_White));

        wlPoster->atlas = (data.wlState == 1 && data.wlState != 0) ? gl2d::TextureAtlas(1, 1) : gl2d::TextureAtlas(2, 1);
        if(wlPoster->atlas.xCount > 1) wlPoster->atlasPos.x = rand() % 10 >= 5 ? 1 : 0;
    }

    void onExit()
    {
        single = true;
        renderData.reset();
        m_renderBuffer.getReadBuffer() = {};
        if(!wlSFX.isFinished()) wlSFX.stop();
    }

    void handleInput(const KeyInputState& in)
    {
        MousePos mousePos = app->getMousePos();

        vp = app->getViewportScale();

        if (in.mouseReleased[GLFW_MOUSE_BUTTON_LEFT])
        {
            if (data.wlState == 1)
            {
                app->getStateStack().queueTransit(this, "");
                app->getStateStack().queueTransit("GameplayState", "GameMenu");
                app->getStateStack().queueSuspend("GameplayState");
            }
            if (data.wlState == 2)
            {
                app->getStateStack().queueTransit(this, "");
                app->getStateStack().queueTransit("GameplayState", "GameMenu");
                app->getStateStack().queueSuspend("GameplayState");
                data.changed = false;
                data.maxScore -= data.score;
            }
        }
    }

    void update(double dt) override
    {
        if (single && renderFinished)
        {
            single = false;
            renderFinished = false;
            if(data.wlState == 2) wlSFX.load(app->getAudioEngine(), SFX_PATH "loseSFX.mp3");
            if (data.wlState == 1) wlSFX.load(app->getAudioEngine(), SFX_PATH "winSFX.mp3");
            wlSFX.play();
        }

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
            if (a) a->render(&renderer, vp);
        }

        if (!renderFinished)renderFinished = true;
    }

private:
    RenderData renderData;
    ViewportScale vp = {};

    GameplayData& data;

    bool renderFinished = false;
    bool single = true;

    gl2d::Texture winTexture;
    gl2d::Texture loseTexture;

    Rect* background = nullptr;
    Rect* wlPoster = nullptr;

    Sound wlSFX{AudioCategory::GameplaySFX};

    gl2d::Font font;

    DoubleBuffer<RenderData> m_renderBuffer;
};