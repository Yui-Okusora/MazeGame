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
protected:
    inline static KeyInputState states;
    inline static MousePos mousePos;
};

class Button : public Shape, UI
{
public:
    Button() {}
    Button(gl2d::Rect rect, gl2d::Color4f _color = Colors_White)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = _color;
        noTexture = true;
    }

    Button(gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 })
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = _color;
        atlas = _atlas;
        texture = _texture;
        noTexture = false;
    }

    Button(Shape* _parent, gl2d::Rect rect, gl2d::Color4f _color = Colors_White)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        parent = _parent;
        color = _color;
        noTexture = true;
    }

    Button(Shape* _parent, gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 })
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        parent = _parent;
        color = _color;
        atlas = _atlas;
        texture = _texture;
        noTexture = false;
    }

    void render(gl2d::Renderer2D* renderer) override
    {
        gl2d::Color4f finalColor = color * (hovered() ? 0.85f : 1.0f);
        finalColor.a = color.a;

        if (!noTexture)
            renderer->renderRectangle({ getAbsPos(), size}, texture, finalColor, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
        else
            renderer->renderRectangle({ getAbsPos(), size }, finalColor, {}, 0);
    }

    Shape* cloneToArena(Arena& a) const { return a.make<Button>(*this); }

    bool hovered() { return Utils::inRect({ mousePos.x, mousePos.y }, getAbsPos(), size); }

    bool clicked()
    {
        bool isClicked = hovered() && states.mouseReleased[GLFW_MOUSE_BUTTON_LEFT];
        if (hovered() && states.mouseDown[GLFW_MOUSE_BUTTON_LEFT]) atlasPos.x = 1;
        else atlasPos.x = 0;
        return isClicked;
    }
    bool rightClicked()
    {
        return hovered() && states.mouseReleased[GLFW_MOUSE_BUTTON_RIGHT];
    }
private:
    
};

class TextBox : public Shape, UI
{
public:
    TextBox() {}
    TextBox(gl2d::Rect rect, gl2d::Color4f _color, gl2d::Font _font, float _fontSize, bool _clickable = false)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = _color;
        font = _font;
        fontSize = _fontSize;
        noTexture = true;
        clickable = _clickable;
    }

    TextBox(Shape* _parent, gl2d::Rect rect, gl2d::Color4f _color, gl2d::Font _font, float _fontSize, bool _clickable = false)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        parent = _parent;
        color = _color;
        font = _font;
        fontSize = _fontSize;
        noTexture = true;
        clickable = _clickable;
    }

    Shape* cloneToArena(Arena& a) const { return a.make<TextBox>(*this); }

    bool hovered() { return Utils::inRect({ mousePos.x, mousePos.y }, getAbsPos(), size); }

    bool isFocus() { return isFocused; }

    std::string& getText() { return text; }

    bool clicked()
    {
        if (states.mouseReleased[GLFW_MOUSE_BUTTON_LEFT])
        {
            if (hovered()) isFocused = true;
            else isFocused = false;
            return true;
        }

        if (!isFocused) return false;

        for (int i = 0; i < states.keyPressed.size(); ++i)
        {
            if (!states.keyPressed[i]) continue;
            if(states.keyPressed[GLFW_KEY_SPACE]) text.push_back((char)i);
            if (i < 33 || i > 122) continue;
            if (i >= 'A' && i <= 'Z')
            {
                if (!states.keyDown[GLFW_KEY_LEFT_SHIFT])
                {
                    text.push_back((char)(i + 32));
                    continue;
                }
            }
            text.push_back((char)i);
            std::cout << (char)i;
        }

        if (states.keyPressed[GLFW_KEY_BACKSPACE] && !text.empty()) text.pop_back();

        return false;
    }

    void render(gl2d::Renderer2D* renderer)
    {
        gl2d::Color4f finalColor = color * (hovered() && clickable ? 0.85f : 1.0f);
        finalColor.a = color.a;

        if (!noTexture)
            renderer->renderRectangle({ getAbsPos(), size}, texture, finalColor, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
        else
            renderer->renderRectangle({ getAbsPos(), size }, finalColor, {}, 0);

        renderer->renderText(getAbsPos() + glm::vec2(5, size.y - fontSize / 6.0f), text.c_str(), font, Colors_Black, fontSize, 4, 3, false, {});
    }
private:
    std::string text = "";
    gl2d::Font font;
    float fontSize = 64;
    bool isFocused = false;
    bool clickable = false;
};