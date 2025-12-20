#pragma once

#include <Core.hpp>
#include <Utils/Utils.hpp>
#include <State/State.hpp>
#include <GameMechanics/PathFinding.hpp>
#include <Application/IApplication.hpp>
#include <Shape/UI.hpp>
#include <Shape/Maze.hpp>
#include <SaveFile/SaveManager.hpp>


class GameplayState : public State
{
public:
    GameplayState(IApplication* _app);

    void handleInput(const KeyInputState& in) override;

    void onEnter() override;

    void onExit() override;

    void update(double dt) override;

    void render() override;

private:
    gl2d::Texture gameplayBgTexture;
    gl2d::Texture statusBgTexture;
    gl2d::Texture statBgTexture;
    gl2d::Texture goalTexture;
    gl2d::Texture playerTexture;
    gl2d::Texture enemy1Texture;
    gl2d::Texture enemy2Texture;
    gl2d::Texture enemy3Texture;
    gl2d::Texture mazeTexture;
    gl2d::Texture mazeBgTexture;
    gl2d::Texture undoTexture;
    gl2d::Texture resetTexture;
    gl2d::Texture pauseTexture;

    glm::vec2 move = {};
    const glm::vec2 charSize = { 64, 64 };

    GameplayData& data;

    Maze* maze = nullptr;
    Rect* mazeBg = nullptr;
    Rect* background = nullptr;
    Rect* goal = nullptr;
    Rect* statusBg = nullptr;
    Rect* timeBg = nullptr;
    Rect* scoreBg = nullptr;
    Rect* player = nullptr;
    std::vector<Rect*> enemies{};

    Button* resetBtn = nullptr;
    Button* undoBtn = nullptr;
    Button* pauseBtn = nullptr;

    TextBox* timeDisplay = nullptr;
    TextBox* scoreDisplay = nullptr;

    gl2d::Font font;

    RenderData renderData;

    ViewportScale vp = {};

    bool undoing = false;
    bool doneMove = false;
    bool doneUndo = false;

    Sound bgm{AudioCategory::Music};

    UI ui;

    DoubleBuffer<RenderData> m_renderBuffer;
};