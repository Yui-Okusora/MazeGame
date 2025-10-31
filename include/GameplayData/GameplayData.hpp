#pragma once

#include<Core.hpp>

struct GameplayData
{
	glm::vec2 playerPos = { 0, 0 };
    glm::vec2 playerSize = { 64, 64 };
    glm::vec2 mazePos = { 0, 0 };
    glm::vec2 mazeSize = { 512, 512 };
    glm::uvec2 atlasPos = { 0, 0 };
    bool isLeft = true;
	//std::vector<Maze> maze;
};

struct MazeCell
{
    int top = 0;
    int bottom = 0;
    int left = 0;
    int right = 0;
};