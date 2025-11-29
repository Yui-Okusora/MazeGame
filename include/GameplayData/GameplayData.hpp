#pragma once

#include <Core.hpp>
#include <Shape/Shape.hpp>

struct GameplayData
{
	glm::vec2 playerPos = { 0, 0 };
    glm::vec2 playerSize = { 64, 64 };

    glm::vec2 enemyPos = { 0, 448 };
    glm::vec2 enemySize = { 64, 64 };

    glm::uvec2 atlasPos = { 0, 0 };
    bool isLeft = true;
    std::shared_ptr<Shape> maze;
};

