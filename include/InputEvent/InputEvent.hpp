#pragma once

#include <Core.hpp>

struct InputEvent
{
    int key;
    int scancode;
    int action;
    int mods;
};

struct InputState {
    std::array<bool, GLFW_KEY_LAST + 1> keyDown{};
};