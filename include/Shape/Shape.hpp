#pragma once

#include <Core.hpp>

class Shape
{
public:
    Shape() = default;
    Shape(const Shape&) = default;

    virtual void render(gl2d::Renderer2D* renderer) = 0;
    virtual Shape* cloneToArena(Arena& a) const = 0;

    glm::vec2 getAnchor()
    {
        if (parent) return parent->getAbsPos();
        return {};
    }

    glm::vec2& getRelPos() { return pos; }
    glm::vec2 getAbsPos() { return getAnchor() + pos; }

    void setAnimation(int frame, double duration, double dt, bool _flip = false)
    {
        this->frame = frame;
        this->duration = duration;
        this->dt = dt;
        this->flip = _flip;
    }

    void runAnimation()
    {
        if (frameSize.size() <= frame) return;
        accumulator += dt;
        if (frame != atlasPos.y) atlasPos.x = 0;

        atlasPos.y = frame;

        if (accumulator >= (duration / (double)frameSize[frame]))
        {
            accumulator -= (duration / (double)frameSize[frame]);
            atlasPos.x = (++atlasPos.x) % frameSize[frame];
        }
    }

    glm::vec2 pos = { 0, 0 };
    glm::vec2 size = { 1, 1 };
    Shape* parent = nullptr;
    gl2d::Texture texture;
    gl2d::Color4f color = Colors_White;
    gl2d::TextureAtlas atlas = { 1, 1 };
    glm::uvec2 atlasPos = { 0, 0 };

    int frame = 0;
    std::vector<int> frameSize;
    double accumulator = 0;
    double duration = 0;
    double dt = 0;

    bool flip = false;
    bool noTexture = false;
};

class Rect : public Shape
{
public:
    Rect();
    Rect(const Rect& src);
    
    Rect(gl2d::Rect rect, gl2d::Color4f _color = Colors_White);
    Rect(gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 }, const std::vector<int>& _frameSize = {});
    
    Rect(Shape* _parent, gl2d::Rect rect, gl2d::Color4f _color = Colors_White);
    Rect(Shape* _parent, gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 }, const std::vector<int>& _frameSize = {});

    void render(gl2d::Renderer2D* renderer) override;

    Shape* cloneToArena(Arena& a) const override { return a.make<Rect>(*this); }
};