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
    std::array<bool, GLFW_KEY_LAST + 1> keyDown = { false };
    std::array<bool, GLFW_KEY_LAST + 1> keyPressed = { false };
    std::array<bool, GLFW_KEY_LAST + 1> keyReleased = { false };
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouseDown = { false };
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mousePressed = { false };
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouseReleased = { false };
    std::vector<short> keyBuffer;
};