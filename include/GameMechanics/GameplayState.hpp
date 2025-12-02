#pragma once

#include <Core.hpp>
#include <Utils/Utils.hpp>
#include <State/State.hpp>
#include <GameMechanics/PathFinding.hpp>
#include <Application/IApplication.hpp>
#include <Shape/UI.hpp>
#include <SaveFile/SaveLoad.hpp>


class GameplayState : public State
{
public:
    GameplayState(IApplication* _app);

    void handleInput(const KeyInputState& in) override;

    void onEnter() override;

    void update(double dt) override;

    void render() override;

private:
    gl2d::Texture playerTexture;
    gl2d::Texture mazeTexture;

    glm::vec2 move = {};

    GameplayData data;

    SaveLoad sl;

    std::vector<int> maze2 =
    {// 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 
        //   1   2   3   4   5   6   7   8
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//1
        0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,//2 1
        0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,//3
        0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,//4 2
        0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,//5
        0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,//6 3
        0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,//7
        0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,//8 4
        0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,//9
        0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,//0 5
        0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,//1
        0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,//2 6
        0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,//3
        0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,//4 7
        0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,//5
        0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,//6 8
        0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0 //7
    };

    Maze* maze = nullptr;
    Rect* player = nullptr;
    Rect* enemy = nullptr;

    Button* resetBtn = nullptr;

    RenderData renderData;

    DoubleBuffer<RenderData> m_renderBuffer;
};