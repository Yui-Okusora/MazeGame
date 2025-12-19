#pragma once

#include <Core.hpp>
#include <Utils/Utils.hpp>
#include <Application/IApplication.hpp>
#include <Shape/UI.hpp>

class PauseMenu : public State
{
public:
    PauseMenu(IApplication* _app)
        : data(*static_cast<GameplayData*>(_app->getCTX()))
    {
        app = _app;
        label = "PauseMenu";
        font.createFromFile(FONT_PATH "calibril.ttf");
    }

    void onEnter() override
    {
        renderData = RenderData();
        textbox = renderData.addShape<TextBox>(TextBox({100, 100, 512, 64}, Colors_White, font, 64, true));
        if (data.changed)
        {
            textbox->getText() = data.name;
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
            app->getStateStack().queueTransit(this, "GameplayState");
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

        renderer.clearScreen({ 0.1, 0.2, 0.6, 1 });

        for (auto& a : renderData.shapes)
        {
            if (a) a->render(&renderer);
        }
    }

    RenderData renderData;

    GameplayData& data;

    TextBox* textbox = nullptr;
    gl2d::Font font;

    DoubleBuffer<RenderData> m_renderBuffer;
};