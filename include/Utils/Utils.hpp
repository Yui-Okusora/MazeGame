#pragma once

#include <Core.hpp>

class Utils
{
public:
    static bool inRect(glm::vec2 pos, glm::vec2 rectPos, glm::vec2 rectDim)
    {
        return glm::all(glm::greaterThanEqual(pos, rectPos)) && glm::all(glm::lessThanEqual(pos, rectPos + rectDim));
    }
    static bool inRect(glm::vec2 pos, glm::vec2 objDim, glm::vec2 rectPos, glm::vec2 rectDim)
    {
        return glm::all(glm::greaterThanEqual(pos, rectPos)) && glm::all(glm::lessThanEqual(pos, rectPos + rectDim - objDim));
    }
    static glm::vec2 lerp2(glm::vec2 a, glm::vec2 b, float c, float distSnapThreshold = -1.0f)
    {
        a.x = std::lerp(a.x, b.x, c);
        a.y = std::lerp(a.y, b.y, c);

        if (std::abs(a.x - b.x) <= distSnapThreshold) a.x = b.x;
        if (std::abs(a.y - b.y) <= distSnapThreshold) a.y = b.y;

        return a;
    }
};