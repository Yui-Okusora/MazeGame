#pragma once

#include <Core.hpp>

class Djikstra
{
public:
    static bool run(std::pair<int, int> current, std::pair<int, int> target,
        std::pair<int, int> size,
        const std::vector<int>& matrix,
        std::vector<std::vector<bool>>& visited,
        std::vector<std::pair<int, int>>& path)
    {
        int width = (size.first - 1) / 2;
        int height = (size.second - 1) / 2;

        const int INF = (std::numeric_limits<int>::max)();

        std::vector<std::vector<int>> dist(height, std::vector<int>(width, INF));
        std::vector<std::vector<std::pair<int, int>>> parent(
            height, std::vector<std::pair<int, int>>(width, { -1, -1 })
        );

        using Node = std::pair<int, std::pair<int, int>>;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

        dist[current.second][current.first] = 0;
        pq.push({ 0, current });

        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };

        while (!pq.empty()) {
            auto [d, pos] = pq.top();
            pq.pop();

            int x = pos.first;
            int y = pos.second;

            if (visited[y][x]) continue;
            visited[y][x] = true;

            if (pos == target) break;

            for (int i = 0; i < 4; ++i) {
                int encX = 2 * x + 1;
                int encY = 2 * y + 1;

                int isValid = matrix[
                    encX + dc[i] + size.first * (encY + dr[i])
                ];

                int nx = x + dc[i];
                int ny = y + dr[i];

                if (!isValid) continue;
                if (nx < 0 || ny < 0 || nx >= width || ny >= height) continue;
                if (visited[ny][nx]) continue;

                int nd = d + 1;
                if (nd < dist[ny][nx]) {
                    dist[ny][nx] = nd;
                    parent[ny][nx] = { x, y };
                    pq.push({ nd, { nx, ny } });
                }
            }
        }

        if (dist[target.second][target.first] == INF)
            return false;

        // Reconstruct path
        path.clear();
        for (std::pair<int, int> p = target; p.first != -1; p = parent[p.second][p.first]) {
            path.push_back(p);
        }
        std::reverse(path.begin(), path.end());

        return true;
    }
};

class BFS_Heuristic
{
public:
    static bool run(std::pair<int, int> current, std::pair<int, int> target,
        std::pair<int, int> size,
        const std::vector<int>& matrix,
        std::vector<std::vector<bool>>& visited,
        std::vector<std::pair<int, int>>& path)
    {
        path.push_back(current);
        auto cur = path.back();
        while (target.first - cur.first != 0)
        {
            auto cur = path.back();
            int dist = target.first - cur.first;

            int encX = 2 * cur.first + 1;
            int encY = 2 * cur.second + 1;

            if (dist == 0) break;
            if (dist > 0)
            {
                if (!matrix[encX + 1 + size.first * encY]) break;
                path.push_back({ cur.first + 1, cur.second });
            }
            else
            {
                if (!matrix[encX - 1 + size.first * encY]) break;
                path.push_back({ cur.first - 1, cur.second });
            }
        }
        while (target.second - cur.second != 0)
        {
            auto cur = path.back();
            int dist = target.second - cur.second;

            int encX = 2 * cur.first + 1;
            int encY = 2 * cur.second + 1;

            if (dist == 0) break;
            if (dist > 0)
            {
                if (!matrix[encX + size.first * (encY + 1)]) break;
                path.push_back({ cur.first, cur.second + 1 });
            }
            else
            {
                if (!matrix[encX + size.first * (encY - 1)]) break;
                path.push_back({ cur.first, cur.second - 1 });
            }
        }
        if (target == cur) return true;
        return false;
    }
};

template<typename T>
class PathFinding
{
public:
    static std::vector<std::pair<int, int>> run(std::pair<int, int> current, std::pair<int, int> target, std::pair<int, int> size, const std::vector<int>& matrix)
    {
        std::vector<std::pair<int, int>> path;
        std::vector<std::vector<bool>> visited(size.second, std::vector<bool>(size.first, false));
        T::run(current, target, {size.first * 2 + 1, size.second * 2 + 1}, matrix, visited, path);
        return path;
    }
};