#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <fstream>

struct MazeData
{
    glm::vec2 mazeSize = { 8, 8 };
    std::vector<int> mazeEncode = {}; // dim = mazeSize * 2 + 1
    glm::vec2 startPos = {}; // unencode pos
    glm::vec2 goalPos = {};
    int difficulty = 0;
    int enemyNum = 1;
    std::vector<glm::vec2> enemyPosList;
};

inline std::fstream& operator>>(std::fstream& is, MazeData& data)
{
    is >> data.mazeSize.x >> data.mazeSize.y;

    int w = static_cast<int>(data.mazeSize.x);
    int h = static_cast<int>(data.mazeSize.y);
    int encodeSize = (w * 2 + 1) * (h * 2 + 1);

    data.mazeEncode.resize(encodeSize);
    for (int& v : data.mazeEncode)
        is >> v;

    is >> data.startPos.x >> data.startPos.y;
    is >> data.goalPos.x >> data.goalPos.y;

    is >> data.difficulty;
    is >> data.enemyNum;

    data.enemyPosList.resize(data.enemyNum);
    for (int i = 0; i < data.enemyNum; ++i)
    {
        is >> data.enemyPosList[i].x
            >> data.enemyPosList[i].y;
    }

    return is;
}

inline std::fstream& operator<<(std::fstream& os, const MazeData& data)
{
    // Maze size
    os << data.mazeSize.x << " " << data.mazeSize.y << "\n";

    // Maze encode
    for (int v : data.mazeEncode)
        os << v << " ";
    os << "\n";

    // Start & goal
    os << data.startPos.x << " " << data.startPos.y << "\n";
    os << data.goalPos.x << " " << data.goalPos.y << "\n";

    os << data.difficulty << "\n";

    // Enemy count
    os << data.enemyNum << "\n";

    // Enemy positions
    for (const auto& pos : data.enemyPosList)
    {
        os << pos.x << " " << pos.y << "\n";
    }

    return os;
}