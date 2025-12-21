#pragma once

#include <Core.hpp>
#include <Shape/Shape.hpp>
#include <InputEvent/InputEvent.hpp>
#include <Utils/Utils.hpp>

class UI
{
public:
    void processUI(const KeyInputState& _states, const MousePos& _mousePos, const ViewportScale& _vp)
    {
        prevPos = mousePos;
        states = _states;
        mousePos.x = (_mousePos.x - _vp.offset.x) / _vp.scale;
        mousePos.y = (_mousePos.y - _vp.offset.y) / _vp.scale;
    }

    KeyInputState states;
    MousePos mousePos = {};
    MousePos prevPos = {};
};

class Button : public Shape
{
public:
    Button(UI& _ui) : ui(_ui) {}
    Button(UI& _ui, gl2d::Rect rect, gl2d::Color4f _color = Colors_White) : ui(_ui)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = _color;
        noTexture = true;
    }

    Button(UI& _ui, gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 }) : ui(_ui)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = _color;
        atlas = _atlas;
        texture = _texture;
        noTexture = false;
    }

    Button(UI& _ui, Shape* _parent, gl2d::Rect rect, gl2d::Color4f _color = Colors_White) : ui(_ui)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        parent = _parent;
        color = _color;
        noTexture = true;
    }

    Button(UI& _ui, Shape* _parent, gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 }) : ui(_ui)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        parent = _parent;
        color = _color;
        atlas = _atlas;
        texture = _texture;
        noTexture = false;
    }

    void render(gl2d::Renderer2D* renderer, const ViewportScale& _vp) override
    {
        if (!allowRender) return;
        gl2d::Color4f finalColor = color * (hovered() ? 0.85f : 1.0f);
        finalColor.a = color.a;

        glm::vec2 p = getViewportPos(_vp);
        glm::vec2 s = getViewportSize(_vp);

        if (!noTexture)
            renderer->renderRectangle({ p, s }, texture, finalColor, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
        else
            renderer->renderRectangle({ p, s }, finalColor, {}, 0);
    }

    Shape* cloneToArena(Arena& a) const { return a.make<Button>(*this); }

    bool hovered() { return Utils::inRect({ ui.mousePos.x, ui.mousePos.y }, getAbsPos(), size); }

    bool clicked()
    {
        if (!allowRender) return false;
        bool isClicked = hovered() && ui.states.mouseReleased[GLFW_MOUSE_BUTTON_LEFT];
        if (hovered() && ui.states.mouseDown[GLFW_MOUSE_BUTTON_LEFT]) atlasPos.x = 1;
        else atlasPos.x = 0;
        return isClicked;
    }

    bool holded()
    {
        if (!allowRender) return false;
        bool isClicked = hovered() && ui.states.mouseDown[GLFW_MOUSE_BUTTON_LEFT];
        if (hovered() && ui.states.mouseDown[GLFW_MOUSE_BUTTON_LEFT]) atlasPos.x = 1;
        else atlasPos.x = 0;
        return isClicked;
    }

    bool rightClicked()
    {
        if (!allowRender) return false;
        return hovered() && ui.states.mouseReleased[GLFW_MOUSE_BUTTON_RIGHT];
    }
private:
    UI& ui;
};

class TextBox : public Shape
{
public:
    TextBox(UI& _ui) : ui(_ui) {}
    TextBox(UI& _ui, gl2d::Rect rect, gl2d::Color4f _color, gl2d::Font _font, float _fontSize, bool _clickable = false, int _textLen = -1) : ui(_ui)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = _color;
        font = _font;
        fontSize = _fontSize;
        noTexture = true;
        clickable = _clickable;
        textLen = _textLen;
    }

    TextBox(UI& _ui, Shape* _parent, gl2d::Rect rect, gl2d::Color4f _color, gl2d::Font _font, float _fontSize, bool _clickable = false, int _textLen = -1) : ui(_ui)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        parent = _parent;
        color = _color;
        font = _font;
        fontSize = _fontSize;
        noTexture = true;
        clickable = _clickable;
        textLen = _textLen;
    }

    Shape* cloneToArena(Arena& a) const { return a.make<TextBox>(*this); }

    bool hovered() { return Utils::inRect({ ui.mousePos.x, ui.mousePos.y }, getAbsPos(), size); }

    bool isFocus() { return isFocused; }

    bool focus()
    {
        if (!isFocused) text.push_back('<');
        isFocused = true;
        return isFocused;
    }

    bool unfocus()
    {
        if (isFocused) text.pop_back();
        isFocused = false;
        return isFocused;
    }

    std::string& getText() { return text; }

    std::string getText() const { return text; }

    bool clicked()
    {
        if (!allowRender) return false;
        if (ui.states.keyReleased[GLFW_KEY_ENTER] && isFocused)
        {
            text.pop_back();
            isFocused = false;
        }
        if (ui.states.mouseReleased[GLFW_MOUSE_BUTTON_LEFT])
        {
            if (hovered()) return focus();
            else return unfocus();
        }

        if (!isFocused || !clickable) return false;

        for (int i = 0; i < ui.states.keyPressed.size(); ++i)
        {
            if (textLen <= 0 || text.size() > textLen) break;
            if (!ui.states.keyPressed[i]) continue;
            if (i < 32 || i > 122) continue;
            if (i >= 'A' && i <= 'Z')
            {
                if (!ui.states.keyDown[GLFW_KEY_LEFT_SHIFT])
                {
                    text.pop_back();
                    text.push_back((char)(i + 32));
                    text.push_back('<');
                    continue;
                }
            }
            text.pop_back();
            text.push_back((char)i);
            text.push_back('<');
        }

        if (ui.states.keyPressed[GLFW_KEY_BACKSPACE] && text.size() > 1)
        {
            text.pop_back();
            text.pop_back();
            text.push_back('<');
        }

        return false;
    }

    void render(gl2d::Renderer2D* renderer, const ViewportScale& vp)
    {
        if (!allowRender) return;
        gl2d::Color4f finalColor = color * (hovered() && clickable ? 0.85f : 1.0f);
        finalColor.a = color.a;

        glm::vec2 p = getViewportPos(vp);
        glm::vec2 s = getViewportSize(vp);

        if (!noTexture)
            renderer->renderRectangle({ p, s }, texture, finalColor, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
        else
            renderer->renderRectangle({ p, s }, finalColor, {}, 0);

        glm::vec2 textPosVP =
            p + glm::vec2(5.0f, s.y - fontSize * vp.scale / 6.0f);

        renderer->renderText(textPosVP, text.c_str(), font, textColor, fontSize * vp.scale, 3.5f, 2, false, {});
    }

    gl2d::Color4f textColor = Colors_Black;

private:
    UI& ui;
    std::string text = "";
    gl2d::Font font;
    int textLen = -1;
    float fontSize = 64;
    bool isFocused = false;
    bool clickable = false;
};

class SaveCell : public Shape
{
public:
    //SaveCell(UI& _ui) : ui(_ui) {}
    SaveCell(const SaveCell& src) 
        : ui(src.ui),
        label(src.ui, { src.label.pos, src.label.size }, Colors_Transparent, src.font, src.fontSize),
        timestamp(src.ui, { src.timestamp.pos, src.timestamp.size }, Colors_Transparent, src.font, src.fontSize * 0.8f),
        name(src.ui, { src.name.pos, src.name.size }, Colors_Transparent, src.font, src.fontSize),
        stats(src.ui, { src.stats.pos, src.stats.size }, Colors_Transparent, src.font, src.fontSize * 0.8f)
    {
        pos = src.pos;
        size = src.size;
        color = Colors_White;
        font = src.font;
        fontSize = src.fontSize;
        texture = src.texture;
        noTexture = src.noTexture;
        atlas = { 2, 1 };
        textLen = src.textLen;
        label.parent = this;
        timestamp.parent = this;
        name.parent = this;
        stats.parent = this;
        label.getText() = src.label.getText();
        timestamp.getText() = src.timestamp.getText();
        name.getText() = src.name.getText();
        stats.getText() = src.stats.getText();
    }
    
    SaveCell(UI& _ui, gl2d::Rect rect, gl2d::Font _font, float _fontSize, int _textLen = -1)
        : ui(_ui),
        label(_ui,        { 10, 15, 92, 25 }, Colors_Transparent, _font, _fontSize),
        timestamp(_ui,    { 140, 15, 204, 25 }, Colors_Transparent, _font, _fontSize * 0.8f),
        name(_ui,         { 10, 60, 230, 25 }, Colors_Transparent, _font, _fontSize),
        stats(_ui,        { 235, 42, 157, 25 }, Colors_Transparent, _font, _fontSize * 0.8f)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = Colors_White;
        font = _font;
        fontSize = _fontSize;
        noTexture = true;
        textLen = _textLen;
        atlas = { 2, 1 };
        label.parent = this;
        timestamp.parent = this;
        name.parent = this;
        stats.parent = this;
        label.getText() = "Save 1";
        timestamp.getText() = Utils::formatTS(398563587943);
        name.getText() = "Yui Okusora";
        stats.getText() = "Score 500000\nLast Map 3";
        
    }
    
    SaveCell(UI& _ui, Shape* _parent, gl2d::Rect rect, gl2d::Font _font, float _fontSize, int _textLen = -1)
        : ui(_ui),
        label(_ui, this,        {}, Colors_Transparent, _font, _fontSize),
        timestamp(_ui, this,    {}, Colors_Transparent, _font, _fontSize),
        name(_ui, this,         {}, Colors_Transparent, _font, _fontSize),
        stats(_ui, this,        {}, Colors_Transparent, _font, _fontSize)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = Colors_White;
        font = _font;
        fontSize = _fontSize;
        noTexture = true;
        textLen = _textLen;
        atlas = { 2, 1 };
    }
    
    SaveCell(UI& _ui, gl2d::Rect rect, gl2d::Texture _texture, gl2d::Font _font, float _fontSize, int _textLen = -1)
        : ui(_ui),
        label(_ui,        { 10, 15, 92, 25 }, Colors_Transparent, _font, _fontSize),
        timestamp(_ui,    { 145, 15, 204, 25 }, Colors_Transparent, _font, _fontSize * 0.8f),
        name(_ui,         { 10, 60, 230, 25 }, Colors_Transparent, _font, _fontSize),
        stats(_ui,        { 235, 42, 157, 25 }, Colors_Transparent, _font, _fontSize * 0.8f)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = Colors_White;
        font = _font;
        fontSize = _fontSize;
        texture = _texture;
        noTexture = false;
        textLen = _textLen;
        atlas = { 2, 1 };
        label.parent = this;
        timestamp.parent = this;
        name.parent = this;
        stats.parent = this;
        label.getText() = "Save 1";
        timestamp.getText() = Utils::formatTS(398563587943);
        name.getText() = "Yui Okusora";
        stats.getText() = "Score 500000\nLast Map 3";
        
    }
    
    SaveCell(UI& _ui, Shape* _parent, gl2d::Rect rect, gl2d::Texture _texture, gl2d::Font _font, float _fontSize, int _textLen = -1)
        : ui(_ui),
        label(_ui, this,        {}, Colors_Transparent, _font, _fontSize),
        timestamp(_ui, this,    {}, Colors_Transparent, _font, _fontSize),
        name(_ui, this,         {}, Colors_Transparent, _font, _fontSize),
        stats(_ui, this,        {}, Colors_Transparent, _font, _fontSize)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = Colors_White;
        font = _font;
        fontSize = _fontSize;
        texture = _texture;
        noTexture = false;
        textLen = _textLen;
        atlas = { 2, 1 };
        label.getText() = "Save 1";
        timestamp.getText() = Utils::formatTS(398563587943);
        name.getText() = "Yui Okusora";
        stats.getText() = "Score 500000\nLast Map 3";
    }

    Shape* cloneToArena(Arena& a) const { return a.make<SaveCell>(*this); }

    bool hovered() { return Utils::inRect({ ui.mousePos.x, ui.mousePos.y }, getAbsPos(), size); }

    bool clicked()
    {
        if (!allowRender) return false;
        bool isClicked = hovered() && ui.states.mouseReleased[GLFW_MOUSE_BUTTON_LEFT];
        //if (hovered() && ui.states.mouseDown[GLFW_MOUSE_BUTTON_LEFT]) atlasPos.x = 1;
        //else atlasPos.x = 0;
        return isClicked;
    }

    void render(gl2d::Renderer2D* renderer, const ViewportScale& vp)
    {
        if (!allowRender) return;
        gl2d::Color4f finalColor = color * (hovered() ? 0.85f : 1.0f);
        finalColor.a = color.a;

        glm::vec2 p = getViewportPos(vp);
        glm::vec2 s = getViewportSize(vp);

        if (!noTexture)
            renderer->renderRectangle({ p, s }, texture, finalColor, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
        else
            renderer->renderRectangle({ p, s }, finalColor, {}, 0);

        label.render(renderer, vp);
        timestamp.render(renderer, vp);
        name.render(renderer, vp);
        stats.render(renderer, vp);
    }

    std::string& getLabelText() { return label.getText(); }
    std::string& getTimestampText() { return timestamp.getText(); }
    std::string& getNameText() { return name.getText(); }
    std::string& getStatsText() { return stats.getText(); }

private:
    UI& ui;

    TextBox label;
    TextBox timestamp;
    TextBox name;
    TextBox stats;

    gl2d::Font font;
    int textLen = -1;
    float fontSize = 64;
    bool isFocused = false;
};

class Slider : public Shape
{
public:
    Slider(const Slider& src)
        : ui(src.ui),
        knob(src.ui, {src.knob.pos, src.knob.size}, src.knob.texture, Colors_White, {2, 1})
    {
        pos = src.pos;
        size = src.size;
        color = Colors_White;
        texture = src.texture;
        noTexture = false;
        knob.parent = this;
        prevPos = {};
    }

    Slider(UI& _ui, gl2d::Rect rect, gl2d::Texture _texture, gl2d::Texture _knobTexture)
        : ui(_ui),
        knob(_ui, {0, 0, glm::vec2(70, 27) * 0.75f}, _knobTexture, Colors_White, {2, 1})
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
        color = Colors_White;
        texture = _texture;
        noTexture = false;
        knob.parent = this;
        prevPos = {};
    }

    Shape* cloneToArena(Arena& a) const { return a.make<Slider>(*this); }

    bool hovered() { return knob.hovered(); }

    bool clicked()
    {
        if (!allowRender) return false;
        bool isClicked = hovered() && ui.states.mouseReleased[GLFW_MOUSE_BUTTON_LEFT];
        //if (hovered() && ui.states.mouseDown[GLFW_MOUSE_BUTTON_LEFT]) atlasPos.x = 1;
        //else atlasPos.x = 0;
        return isClicked;
    }

    bool holded()
    {
        if (!allowRender) return false;
        return knob.holded();
    }

    float getValue() { return value; }

    void render(gl2d::Renderer2D* renderer, const ViewportScale& vp)
    {
        if (!allowRender) return;
        gl2d::Color4f finalColor = color;
        finalColor.a = color.a;

        glm::vec2 p = getViewportPos(vp);
        glm::vec2 s = getViewportSize(vp);

        if (!noTexture)
            renderer->renderRectangle({ p, s }, texture, finalColor, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
        else
            renderer->renderRectangle({ p, s }, finalColor, {}, 0);

        if (holded()) knob.pos.x += ui.mousePos.x - ui.prevPos.x;
        knob.pos.x = std::clamp(knob.pos.x, 0.0f, size.x);

        value = knob.pos.x / knob.size.x;

        knob.render(renderer, vp);

        prevPos = ui.mousePos;
    }

private:
    UI& ui;
    MousePos prevPos;
    Button knob;
    float value = 0;
};