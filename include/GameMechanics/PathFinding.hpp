#pragma once

#include <Core.hpp>

class DFS
{
public:
    static bool run(std::pair<int, int> current, std::pair<int, int> target,
        std::pair<int, int> size,
        const std::vector<int>& matrix,
        std::vector<std::vector<bool>>& visited,
        std::vector<std::pair<int, int>>& path)
    {
        if (current == target) {
            path.push_back(current);
            return true;
        }

        visited[current.second][current.first] = true;
        path.push_back(current);

        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; ++i) {
            int encX = 2 * current.first + 1;
            int encY = 2 * current.second + 1;

            int isValid = matrix[encX + dc[i] + size.first * (encY + dr[i])];

            int newY = current.second + dr[i];
            int newX = current.first + dc[i];
            if(isValid)
            if (newY >= 0 && newX >= 0 && newY < (size.second - 1) / 2 && newX < (size.first - 1) / 2) {
                if(!visited[newY][newX])
                if (run({ newX, newY }, target, size, matrix, visited, path)) {
                    return true;
                }
            }
        }

        path.pop_back();
        visited[current.second][current.first] = false;
        return false;
    }
};

class PathFinding
{
public:
    static std::vector<std::pair<int, int>> run(std::pair<int, int> current, std::pair<int, int> target, std::pair<int, int> size, const std::vector<int>& matrix)
    {
        std::vector<std::pair<int, int>> path;
        std::vector<std::vector<bool>> visited(size.second, std::vector<bool>(size.first, false));
        DFS::run(current, target, {size.first * 2 + 1, size.second * 2 + 1}, matrix, visited, path);
        return path;
    }
};