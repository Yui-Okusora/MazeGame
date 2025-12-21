#pragma once

#include <Core.hpp>
#include <Utils/Utils.hpp>
#include <Application/IApplication.hpp>
#include <Shape/UI.hpp>
#include <GameMechanics/GameplayData.hpp>
#include <SaveFile/SaveManager.hpp>

class SaveLoad : public State
{
public:
    SaveLoad(IApplication* _app)
        : data(*static_cast<GameplayData*>(_app->getCTX()))
    {
        app = _app;
        label = "SaveLoad";
        font.createFromFile(FONT_PATH "BoldPixels.ttf");
        menuBtnTexture.loadFromFile(ASSETS_PATH "Menu button.png", true);
        saveTexture.loadFromFile(ASSETS_PATH "savegame box.png", true);
        titleTexture.loadFromFile(ASSETS_PATH "text box.png", true);
    }

    void onEnter() override
    {
        renderData = RenderData();
        vp = app->getViewportScale();

        background = renderData.addShape<Rect>(Rect({ 0,0,1200,805 }, { 0, 0, 0, 0.7 }));
        title = renderData.addShape<TextBox>(TextBox(ui, { 470, 80, 290, 64 }, Colors_White, font, 64, false));
        title->getText() =  std::string(data.saveloadMode ? "Save" : "Load") + " Game";

        menuBtn = renderData.addShape<Button>(Button(ui, { 100, 700, glm::vec2(27, 26) * 2.0f }, menuBtnTexture, Colors_White, { 2, 1 }));

        glm::vec2 origin = { 168, 193 };

        for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 2; ++j)
        {
            savecells[j + i * 2] = renderData.addShape<SaveCell>(SaveCell(ui, { origin + glm::vec2(429.0f * (float)j, 110.0f * (float)i), 425, 100 }, saveTexture, font, 32));
            savecells[j + i * 2]->getLabelText() = std::string("Save ") + std::to_string(j + i * 2 + 1);
            savecells[j + i * 2]->getTimestampText() = "N/A";
            savecells[j + i * 2]->getNameText() = "None";
            savecells[j + i * 2]->getStatsText() = "Score N/A\nLast Map N/A";

            std::string saveDir = SAVE_DIR "save";
            saveDir += std::to_string(j + i * 2 + 1) + ".mzsv";

            auto file = saveMan.load<GameplayData>(saveDir);

            if (file.has_value())
            {
                GameplayData& saveData = file.value();
                savecells[j + i * 2]->getTimestampText() = Utils::formatTS(saveData.timestamp);
                savecells[j + i * 2]->getNameText() = saveData.name;
                savecells[j + i * 2]->getStatsText() = "Score " + std::to_string(saveData.score) + "\nLast Map " + std::to_string(saveData.level);
            }
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
            data.changed = false;
        }

        for (int i = 0; i < 8; ++i)
        {
            SaveCell* savecell = savecells[i];

            std::string saveDir = SAVE_DIR "save";
            saveDir += std::to_string(i + 1) + ".mzsv";

            if (!savecell->clicked()) continue;

            if (data.saveloadMode == 1)
            {
                saveMan.save(saveDir, data);
                savecell->getTimestampText() = Utils::formatTS(data.timestamp);
                savecell->getNameText() = data.name;
                savecell->getStatsText() = "Score " + std::to_string(data.score) + "\nLast Map " + std::to_string(data.level);
            }
            else
            {
                auto file = saveMan.load<GameplayData>(saveDir);

                if (file.has_value())
                {
                    GameplayData& saveData = file.value();
                    data = saveData;
                    if (app->getStateStack().findActiveState("MainMenu"))
                    {
                        app->getStateStack().queueTransit(this, "");
                        app->getStateStack().queueTransit("MainMenu", "GameplayState");
                    }
                    else if (app->getStateStack().findInactiveState("PauseMenu"))
                    {
                        app->getStateStack().queueTransit(this, "");
                        app->getStateStack().queueTransit("GameplayState", "");
                        app->getStateStack().queueSuspend("GameplayState", false, false, false);
                        app->getStateStack().queueTransit(nullptr, "GameplayState");
                    }
                }
            }
        }
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
            if (a) a->render(&renderer, vp);
        }
    }

    RenderData renderData;
    ViewportScale vp = {};

    GameplayData& data;

    UI ui;

    gl2d::Font font;

    gl2d::Texture menuBtnTexture;
    gl2d::Texture saveTexture;
    gl2d::Texture titleTexture;

    TextBox* title = nullptr;
    
    Rect* background = nullptr;

    std::vector<SaveCell*> savecells{8, nullptr};

    Button* menuBtn = nullptr;

    SaveManager saveMan;

    DoubleBuffer<RenderData> m_renderBuffer;
};