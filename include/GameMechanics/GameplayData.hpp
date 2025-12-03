#pragma once

#include <Core.hpp>

struct GameplayData
{
    double time = 0;
    int score = 0;
    int level = 0;
    glm::vec2 playerPos = {};
    glm::vec2 enemyPos = {};
    int enemyStep = 0;
    int enemyMaxStep = 2;
    std::vector<std::pair<int, int>> enemyPath;
    std::vector<std::pair<int, int>> stepHistory;
    std::string name = "";
};