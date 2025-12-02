#pragma once

#include <Core.hpp>

struct GameplayData
{
    double time = 0;
    int score = 0;
    int level = 0;
    glm::vec2 playerPos = {};
    glm::vec2 enemyPos = {};
    //std::string name = "";
};