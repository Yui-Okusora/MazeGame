#pragma once
#include <glm/glm.hpp>

struct ViewportScale
{
    float scale;
    glm::vec2 offset;
};

static ViewportScale computeViewportScale(
    glm::vec2 windowSize,
    glm::vec2 designSize)
{
    float sx = windowSize.x / designSize.x;
    float sy = windowSize.y / designSize.y;

    float scale = std::min(sx, sy); // keep aspect ratio

    glm::vec2 scaled = designSize * scale;
    glm::vec2 offset = (windowSize - scaled) * 0.5f;

    return { scale, offset };
}