#pragma once

#include <Core.hpp>
#include <Shape/Shape.hpp>
#include <InputEvent/InputEvent.hpp>
#include <Utils/Utils.hpp>

class UI
{
public:
    static void processUI(const KeyInputState& _states, const MousePos& _mousePos)
    {
        states = _states;
        mousePos = _mousePos;
    }

    inline static KeyInputState states;
    inline static MousePos mousePos;
};

class Button : public Shape, UI
{
public:
    Button() {}
    Button(int _button, gl2d::Rect rect, gl2d::Color4f _color)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = _color;
        noTexture = true;
    }

    void render(gl2d::Renderer2D* renderer) override
    {
        if (!noTexture)
            renderer->renderRectangle({ pos, size }, texture, color, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
        else
            renderer->renderRectangle({ pos, size }, color, {}, 0);
    }

    Shape* cloneToArena(Arena& a) const { return a.make<Button>(*this); }

    bool hovered() { return Utils::inRect({ mousePos.x, mousePos.y }, pos, size); }

    bool clicked()
    {
        return hovered() && states.mouseReleased[button];
    }
private:
    int button = 0;
};