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
};