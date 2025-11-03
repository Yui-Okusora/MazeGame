#pragma once

#include <Core.hpp>

struct KeyInputEvent
{
    int key;
    int scancode;
    int action;
    int mods;
};

struct MousePos
{
    double x;
    double y;
};

struct MouseKey
{
    int button;
    int action;
    int mods;
};

struct MouseEvent
{
    MouseKey key;
    MousePos pos;
};

struct KeyInputState {
    std::array<bool, GLFW_KEY_LAST + 1> keyDown{};
};